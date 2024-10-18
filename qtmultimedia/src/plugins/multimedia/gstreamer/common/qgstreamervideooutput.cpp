// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtMultimedia/qvideosink.h>

#include <QtCore/qloggingcategory.h>
#include <QtCore/qthread.h>

#include <common/qgstreamervideooutput_p.h>
#include <common/qgstreamervideosink_p.h>
#include <common/qgstsubtitlesink_p.h>

static Q_LOGGING_CATEGORY(qLcMediaVideoOutput, "qt.multimedia.videooutput")

QT_BEGIN_NAMESPACE

static QGstElement makeVideoConvertScale(const char *name)
{
    QGstElementFactoryHandle factory = QGstElement::findFactory("videoconvertscale");
    if (factory) // videoconvertscale is only available in gstreamer 1.20
        return QGstElement::createFromFactory(factory, name);

    return QGstBin::createFromPipelineDescription("videoconvert ! videoscale", name,
                                                  /*ghostUnlinkedPads=*/true);
}

QMaybe<QGstreamerVideoOutput *> QGstreamerVideoOutput::create(QObject *parent)
{
    QGstElementFactoryHandle factory = QGstElement::findFactory("videoconvertscale");

    static std::optional<QString> elementCheck = []() -> std::optional<QString> {
        std::optional<QString> error = qGstErrorMessageIfElementsNotAvailable("fakesink", "queue");
        if (error)
            return error;

        QGstElementFactoryHandle factory = QGstElement::findFactory("videoconvertscale");
        if (factory)
            return std::nullopt;

        return qGstErrorMessageIfElementsNotAvailable("videoconvert", "videoscale");
    }();

    if (elementCheck)
        return *elementCheck;

    return new QGstreamerVideoOutput(parent);
}

QGstreamerVideoOutput::QGstreamerVideoOutput(QObject *parent)
    : QObject(parent),
      m_outputBin(QGstBin::create("videoOutput")),
      m_videoQueue{
          QGstElement::createFromFactory("queue", "videoQueue"),
      },
      m_videoConvertScale{
          makeVideoConvertScale("videoConvertScale"),
      },
      m_videoSink{
          QGstElement::createFromFactory("fakesink", "fakeVideoSink"),
      }
{
    m_videoSink.set("sync", true);
    m_videoSink.set("async", false); // no asynchronous state changes

    m_outputBin.add(m_videoQueue, m_videoConvertScale, m_videoSink);
    qLinkGstElements(m_videoQueue, m_videoConvertScale, m_videoSink);

    m_outputBin.addGhostPad(m_videoQueue, "sink");
}

QGstreamerVideoOutput::~QGstreamerVideoOutput()
{
    m_outputBin.setStateSync(GST_STATE_NULL);
}

void QGstreamerVideoOutput::setVideoSink(QVideoSink *sink)
{
    auto *gstVideoSink = sink ? static_cast<QGstreamerVideoSink *>(sink->platformVideoSink()) : nullptr;
    if (gstVideoSink == m_platformVideoSink)
        return;

    if (m_platformVideoSink)
        m_platformVideoSink->setPipeline({});

    m_platformVideoSink = gstVideoSink;
    if (m_platformVideoSink) {
        m_platformVideoSink->setPipeline(m_pipeline);
        if (m_nativeSize.isValid())
            m_platformVideoSink->setNativeSize(m_nativeSize);
    }
    QGstElement gstSink;
    if (m_platformVideoSink) {
        gstSink = m_platformVideoSink->gstSink();
    } else {
        gstSink = QGstElement::createFromFactory("fakesink", "fakevideosink");
        Q_ASSERT(gstSink);
        gstSink.set("sync", true);
        gstSink.set("async", false); // no asynchronous state changes
    }

    if (m_videoSink == gstSink)
        return;

    m_pipeline.modifyPipelineWhileNotRunning([&] {
        if (!m_videoSink.isNull())
            m_outputBin.stopAndRemoveElements(m_videoSink);

        m_videoSink = gstSink;
        m_outputBin.add(m_videoSink);

        qLinkGstElements(m_videoConvertScale, m_videoSink);

        GstEvent *event = gst_event_new_reconfigure();
        gst_element_send_event(m_videoSink.element(), event);
        m_videoSink.syncStateWithParent();

        doLinkSubtitleStream();
    });

    qCDebug(qLcMediaVideoOutput) << "sinkChanged" << gstSink.name();

    m_pipeline.dumpGraph(m_videoSink.name().constData());
}

void QGstreamerVideoOutput::setPipeline(const QGstPipeline &pipeline)
{
    m_pipeline = pipeline;
    if (m_platformVideoSink)
        m_platformVideoSink->setPipeline(m_pipeline);
}

void QGstreamerVideoOutput::linkSubtitleStream(QGstElement src)
{
    qCDebug(qLcMediaVideoOutput) << "link subtitle stream" << src.isNull();
    if (src == m_subtitleSrc)
        return;

    m_pipeline.modifyPipelineWhileNotRunning([&] {
        m_subtitleSrc = src;
        doLinkSubtitleStream();
    });
}

void QGstreamerVideoOutput::unlinkSubtitleStream()
{
    if (m_subtitleSrc.isNull())
        return;
    qCDebug(qLcMediaVideoOutput) << "unlink subtitle stream";
    m_subtitleSrc = {};
    if (!m_subtitleSink.isNull()) {
        m_pipeline.modifyPipelineWhileNotRunning([&] {
            m_pipeline.stopAndRemoveElements(m_subtitleSink);
            return;
        });
        m_subtitleSink = {};
    }
    if (m_platformVideoSink)
        m_platformVideoSink->setSubtitleText({});
}

void QGstreamerVideoOutput::doLinkSubtitleStream()
{
    if (!m_subtitleSink.isNull()) {
        m_pipeline.stopAndRemoveElements(m_subtitleSink);
        m_subtitleSink = {};
    }
    if (!m_platformVideoSink || m_subtitleSrc.isNull())
        return;
    if (m_subtitleSink.isNull()) {
        m_subtitleSink = m_platformVideoSink->subtitleSink();
        m_pipeline.add(m_subtitleSink);
    }
    qLinkGstElements(m_subtitleSrc, m_subtitleSink);
}

void QGstreamerVideoOutput::updateNativeSize()
{
    if (!m_platformVideoSink)
        return;

    m_platformVideoSink->setNativeSize(qRotatedFrameSize(m_nativeSize, m_rotation));
}

void QGstreamerVideoOutput::setIsPreview()
{
    // configures the queue to be fast and lightweight for camera preview
    // also avoids blocking the queue in case we have an encodebin attached to the tee as well
    m_videoQueue.set("leaky", 2 /*downstream*/);
    m_videoQueue.set("silent", true);
    m_videoQueue.set("max-size-buffers", uint(1));
    m_videoQueue.set("max-size-bytes", uint(0));
    m_videoQueue.set("max-size-time", quint64(0));
}

void QGstreamerVideoOutput::flushSubtitles()
{
    if (!m_subtitleSink.isNull()) {
        auto pad = m_subtitleSink.staticPad("sink");
        auto *event = gst_event_new_flush_start();
        pad.sendEvent(event);
        event = gst_event_new_flush_stop(false);
        pad.sendEvent(event);
    }
}

void QGstreamerVideoOutput::setNativeSize(QSize sz)
{
    m_nativeSize = sz;
    updateNativeSize();
}

void QGstreamerVideoOutput::setRotation(QtVideo::Rotation rot)
{
    m_rotation = rot;
    updateNativeSize();
}

QT_END_NAMESPACE

#include "moc_qgstreamervideooutput_p.cpp"
