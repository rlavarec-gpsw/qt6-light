// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGSTREAMERVIDEOOUTPUT_P_H
#define QGSTREAMERVIDEOOUTPUT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qobject.h>
#include <private/qtmultimediaglobal_p.h>
#include <private/qmultimediautils_p.h>
#include <common/qgst_p.h>
#include <common/qgstpipeline_p.h>
#include <common/qgstreamervideosink_p.h>
#include <qwaitcondition.h>
#include <qmutex.h>
#include <qpointer.h>

QT_BEGIN_NAMESPACE

class QVideoSink;

class QGstreamerVideoOutput : public QObject
{
    Q_OBJECT

public:
    static QMaybe<QGstreamerVideoOutput *> create(QObject *parent = nullptr);
    ~QGstreamerVideoOutput();

    void setVideoSink(QVideoSink *sink);
    QGstreamerVideoSink *gstreamerVideoSink() const { return m_platformVideoSink; }

    void setPipeline(const QGstPipeline &pipeline);

    QGstElement gstElement() const { return m_outputBin; }
    void linkSubtitleStream(QGstElement subtitleSrc);
    void unlinkSubtitleStream();

    void setIsPreview();
    void flushSubtitles();

    void setNativeSize(QSize);
    void setRotation(QtVideo::Rotation);

private:
    explicit QGstreamerVideoOutput(QObject *parent);

    void doLinkSubtitleStream();
    void updateNativeSize();

    QPointer<QGstreamerVideoSink> m_platformVideoSink;

    // Gst elements
    QGstPipeline m_pipeline;

    QGstBin m_outputBin;
    QGstElement m_videoQueue;
    QGstElement m_videoConvertScale;
    QGstElement m_videoSink;

    QGstElement m_subtitleSrc;
    QGstElement m_subtitleSink;

    QSize m_nativeSize;
    QtVideo::Rotation m_rotation{};
};

QT_END_NAMESPACE

#endif
