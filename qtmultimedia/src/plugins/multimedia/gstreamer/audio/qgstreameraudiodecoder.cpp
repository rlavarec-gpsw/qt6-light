// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
//#define DEBUG_DECODER

#include <audio/qgstreameraudiodecoder_p.h>

#include <common/qgstreamermessage_p.h>
#include <common/qgst_debug_p.h>
#include <common/qgstutils_p.h>

#include <gst/gstvalue.h>
#include <gst/base/gstbasesrc.h>

#include <QtCore/qdatetime.h>
#include <QtCore/qdebug.h>
#include <QtCore/qsize.h>
#include <QtCore/qtimer.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qstandardpaths.h>
#include <QtCore/qurl.h>
#include <QtCore/qloggingcategory.h>

QT_BEGIN_NAMESPACE

static Q_LOGGING_CATEGORY(qLcGstreamerAudioDecoder, "qt.multimedia.gstreameraudiodecoder");

typedef enum {
    GST_PLAY_FLAG_VIDEO         = 0x00000001,
    GST_PLAY_FLAG_AUDIO         = 0x00000002,
    GST_PLAY_FLAG_TEXT          = 0x00000004,
    GST_PLAY_FLAG_VIS           = 0x00000008,
    GST_PLAY_FLAG_SOFT_VOLUME   = 0x00000010,
    GST_PLAY_FLAG_NATIVE_AUDIO  = 0x00000020,
    GST_PLAY_FLAG_NATIVE_VIDEO  = 0x00000040,
    GST_PLAY_FLAG_DOWNLOAD      = 0x00000080,
    GST_PLAY_FLAG_BUFFERING     = 0x000000100
} GstPlayFlags;


QMaybe<QPlatformAudioDecoder *> QGstreamerAudioDecoder::create(QAudioDecoder *parent)
{
    static const auto error = qGstErrorMessageIfElementsNotAvailable("audioconvert", "playbin");
    if (error)
        return *error;

    return new QGstreamerAudioDecoder(parent);
}

QGstreamerAudioDecoder::QGstreamerAudioDecoder(QAudioDecoder *parent)
    : QPlatformAudioDecoder(parent),
      m_playbin{
          QGstPipeline::adopt(GST_PIPELINE_CAST(
                  QGstElement::createFromFactory("playbin", "playbin").element())),
      },
      m_audioConvert{
          QGstElement::createFromFactory("audioconvert", "audioconvert"),
      }
{
    // Sort out messages
    m_playbin.installMessageFilter(this);

    // Set the rest of the pipeline up
    setAudioFlags(true);

    m_outputBin = QGstBin::create("audio-output-bin");
    m_outputBin.add(m_audioConvert);

    // add ghostpad
    m_outputBin.addGhostPad(m_audioConvert, "sink");

    g_object_set(m_playbin.object(), "audio-sink", m_outputBin.element(), NULL);

    m_deepNotifySourceConnection = m_playbin.connect(
            "deep-notify::source", (GCallback)&configureAppSrcElement, (gpointer)this);

    // Set volume to 100%
    gdouble volume = 1.0;
    m_playbin.set("volume", volume);
}

QGstreamerAudioDecoder::~QGstreamerAudioDecoder()
{
    stop();

    m_playbin.removeMessageFilter(this);

#if QT_CONFIG(gstreamer_app)
    delete m_appSrc;
#endif
}

#if QT_CONFIG(gstreamer_app)
void QGstreamerAudioDecoder::configureAppSrcElement([[maybe_unused]] GObject *object, GObject *orig,
                                                    [[maybe_unused]] GParamSpec *pspec,
                                                    QGstreamerAudioDecoder *self)
{
    // In case we switch from appsrc to not
    if (!self->m_appSrc)
        return;

    QGstElementHandle appsrc;
    g_object_get(orig, "source", &appsrc, NULL);

    auto *qAppSrc = self->m_appSrc;
    qAppSrc->setExternalAppSrc(QGstAppSrc{
            qGstSafeCast<GstAppSrc>(appsrc.get()),
            QGstAppSrc::NeedsRef, // CHECK: can we `release()`?
    });
    qAppSrc->setup(self->mDevice);
}
#endif

bool QGstreamerAudioDecoder::processBusMessage(const QGstreamerMessage &message)
{
    qCDebug(qLcGstreamerAudioDecoder) << "received bus message:" << message;

    GstMessage *gm = message.message();

    switch (message.type()) {
    case GST_MESSAGE_DURATION: {
        updateDuration();
        return false;
    }

    case GST_MESSAGE_ERROR: {
        qCDebug(qLcGstreamerAudioDecoder) << "    error" << QCompactGstMessageAdaptor(message);

        QUniqueGErrorHandle err;
        QGString debug;
        gst_message_parse_error(gm, &err, &debug);

        if (message.source() == m_playbin) {
            if (err.get()->domain == GST_STREAM_ERROR
                && err.get()->code == GST_STREAM_ERROR_CODEC_NOT_FOUND)
                processInvalidMedia(QAudioDecoder::FormatError,
                                    tr("Cannot play stream of type: <unknown>"));
            else
                processInvalidMedia(QAudioDecoder::ResourceError,
                                    QString::fromUtf8(err.get()->message));
        } else {
            QAudioDecoder::Error qerror = QAudioDecoder::ResourceError;
            if (err.get()->domain == GST_STREAM_ERROR) {
                switch (err.get()->code) {
                case GST_STREAM_ERROR_DECRYPT:
                case GST_STREAM_ERROR_DECRYPT_NOKEY:
                    qerror = QAudioDecoder::AccessDeniedError;
                    break;
                case GST_STREAM_ERROR_FORMAT:
                case GST_STREAM_ERROR_DEMUX:
                case GST_STREAM_ERROR_DECODE:
                case GST_STREAM_ERROR_WRONG_TYPE:
                case GST_STREAM_ERROR_TYPE_NOT_FOUND:
                case GST_STREAM_ERROR_CODEC_NOT_FOUND:
                    qerror = QAudioDecoder::FormatError;
                    break;
                default:
                    break;
                }
            } else if (err.get()->domain == GST_CORE_ERROR) {
                switch (err.get()->code) {
                case GST_CORE_ERROR_MISSING_PLUGIN:
                    qerror = QAudioDecoder::FormatError;
                    break;
                default:
                    break;
                }
            }

            processInvalidMedia(qerror, QString::fromUtf8(err.get()->message));
        }
        break;
    }

    default:
        if (message.source() == m_playbin)
            return handlePlaybinMessage(message);
    }

    return false;
}

bool QGstreamerAudioDecoder::handlePlaybinMessage(const QGstreamerMessage &message)
{
    GstMessage *gm = message.message();

    switch (GST_MESSAGE_TYPE(gm)) {
    case GST_MESSAGE_STATE_CHANGED: {
        GstState oldState;
        GstState newState;
        GstState pending;

        gst_message_parse_state_changed(gm, &oldState, &newState, &pending);

        bool isDecoding = false;
        switch (newState) {
        case GST_STATE_VOID_PENDING:
        case GST_STATE_NULL:
        case GST_STATE_READY:
            break;
        case GST_STATE_PLAYING:
            isDecoding = true;
            break;
        case GST_STATE_PAUSED:
            isDecoding = true;

            // gstreamer doesn't give a reliable indication the duration
            // information is ready, GST_MESSAGE_DURATION is not sent by most elements
            // the duration is queried up to 5 times with increasing delay
            m_durationQueries = 5;
            updateDuration();
            break;
        }

        setIsDecoding(isDecoding);
        break;
    };

    case GST_MESSAGE_EOS:
        m_playbin.setState(GST_STATE_NULL);
        finished();
        break;

    case GST_MESSAGE_ERROR:
        Q_UNREACHABLE_RETURN(false); // handled in processBusMessage

    case GST_MESSAGE_WARNING:
        qCWarning(qLcGstreamerAudioDecoder) << "Warning:" << QCompactGstMessageAdaptor(message);
        break;

    case GST_MESSAGE_INFO: {
        if (qLcGstreamerAudioDecoder().isDebugEnabled())
            qCWarning(qLcGstreamerAudioDecoder) << "Info:" << QCompactGstMessageAdaptor(message);
        break;
    }
    default:
        break;
    }

    return false;
}

QUrl QGstreamerAudioDecoder::source() const
{
    return mSource;
}

void QGstreamerAudioDecoder::setSource(const QUrl &fileName)
{
    stop();
    mDevice = nullptr;
    delete m_appSrc;
    m_appSrc = nullptr;

    bool isSignalRequired = (mSource != fileName);
    mSource = fileName;
    if (isSignalRequired)
        sourceChanged();
}

QIODevice *QGstreamerAudioDecoder::sourceDevice() const
{
    return mDevice;
}

void QGstreamerAudioDecoder::setSourceDevice(QIODevice *device)
{
    stop();
    mSource.clear();
    bool isSignalRequired = (mDevice != device);
    mDevice = device;
    if (isSignalRequired)
        sourceChanged();
}

void QGstreamerAudioDecoder::start()
{
    addAppSink();

    if (!mSource.isEmpty()) {
        m_playbin.set("uri", mSource.toEncoded().constData());
    } else if (mDevice) {
        // make sure we can read from device
        if (!mDevice->isOpen() || !mDevice->isReadable()) {
            processInvalidMedia(QAudioDecoder::ResourceError, QLatin1String("Unable to read from specified device"));
            return;
        }

        if (!m_appSrc) {
            auto maybeAppSrc = QGstAppSource::create(this);
            if (maybeAppSrc) {
                m_appSrc = maybeAppSrc.value();
            } else {
                processInvalidMedia(QAudioDecoder::ResourceError, maybeAppSrc.error());
                return;
            }
        }

        m_playbin.set("uri", "appsrc://");
    } else {
        return;
    }

    // Set audio format
    if (m_appSink) {
        if (mFormat.isValid()) {
            setAudioFlags(false);
            auto caps = QGstUtils::capsForAudioFormat(mFormat);
            m_appSink.setCaps(caps);
        } else {
            // We want whatever the native audio format is
            setAudioFlags(true);
            m_appSink.setCaps({});
        }
    }

    if (m_playbin.setState(GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
        qWarning() << "GStreamer; Unable to start decoding process";
        m_playbin.dumpGraph("failed");
        return;
    }
}

void QGstreamerAudioDecoder::stop()
{
    m_playbin.setState(GST_STATE_NULL);
    m_currentSessionId += 1;
    removeAppSink();

    // GStreamer thread is stopped. Can safely access m_buffersAvailable
    if (m_buffersAvailable != 0) {
        m_buffersAvailable = 0;
        bufferAvailableChanged(false);
    }

    if (m_position != invalidPosition) {
        m_position = invalidPosition;
        positionChanged(m_position.count());
    }

    if (m_duration != invalidDuration) {
        m_duration = invalidDuration;
        durationChanged(m_duration.count());
    }

    setIsDecoding(false);
}

QAudioFormat QGstreamerAudioDecoder::audioFormat() const
{
    return mFormat;
}

void QGstreamerAudioDecoder::setAudioFormat(const QAudioFormat &format)
{
    if (mFormat != format) {
        mFormat = format;
        formatChanged(mFormat);
    }
}

QAudioBuffer QGstreamerAudioDecoder::read()
{
    using namespace std::chrono;

    QAudioBuffer audioBuffer;

    if (m_buffersAvailable == 0)
        return audioBuffer;

    m_buffersAvailable -= 1;

    if (m_buffersAvailable == 0)
        bufferAvailableChanged(false);

    QGstSampleHandle sample = m_appSink.pullSample();
    GstBuffer *buffer = gst_sample_get_buffer(sample.get());
    GstMapInfo mapInfo;
    gst_buffer_map(buffer, &mapInfo, GST_MAP_READ);
    const char *bufferData = (const char *)mapInfo.data;
    int bufferSize = mapInfo.size;
    QAudioFormat format = QGstUtils::audioFormatForSample(sample.get());

    if (format.isValid()) {
        // XXX At the moment we have to copy data from GstBuffer into QAudioBuffer.
        // We could improve performance by implementing QAbstractAudioBuffer for GstBuffer.
        nanoseconds position = getPositionFromBuffer(buffer);
        audioBuffer = QAudioBuffer{
            QByteArray(bufferData, bufferSize),
            format,
            round<microseconds>(position).count(),
        };
        milliseconds positionInMs = round<milliseconds>(position);
        if (position != m_position) {
            m_position = positionInMs;
            positionChanged(m_position.count());
        }
    }
    gst_buffer_unmap(buffer, &mapInfo);

    return audioBuffer;
}

qint64 QGstreamerAudioDecoder::position() const
{
    return m_position.count();
}

qint64 QGstreamerAudioDecoder::duration() const
{
    return m_duration.count();
}

void QGstreamerAudioDecoder::processInvalidMedia(QAudioDecoder::Error errorCode, const QString& errorString)
{
    stop();
    error(int(errorCode), errorString);
}

GstFlowReturn QGstreamerAudioDecoder::newSample(GstAppSink *)
{
    // "Note that the preroll buffer will also be returned as the first buffer when calling
    // gst_app_sink_pull_buffer()."

    QMetaObject::invokeMethod(this, [this, sessionId = m_currentSessionId] {
        if (sessionId != m_currentSessionId)
            return; // stop()ed before message is executed

        m_buffersAvailable += 1;
        bufferAvailableChanged(true);
        bufferReady();
    });

    return GST_FLOW_OK;
}

GstFlowReturn QGstreamerAudioDecoder::new_sample(GstAppSink *sink, gpointer user_data)
{
    QGstreamerAudioDecoder *decoder = reinterpret_cast<QGstreamerAudioDecoder *>(user_data);
    qCDebug(qLcGstreamerAudioDecoder) << "QGstreamerAudioDecoder::new_sample";
    return decoder->newSample(sink);
}

void QGstreamerAudioDecoder::setAudioFlags(bool wantNativeAudio)
{
    int flags = m_playbin.getInt("flags");
    // make sure not to use GST_PLAY_FLAG_NATIVE_AUDIO unless desired
    // it prevents audio format conversion
    flags &= ~(GST_PLAY_FLAG_VIDEO | GST_PLAY_FLAG_NATIVE_VIDEO | GST_PLAY_FLAG_TEXT | GST_PLAY_FLAG_VIS | GST_PLAY_FLAG_NATIVE_AUDIO);
    flags |= GST_PLAY_FLAG_AUDIO;
    if (wantNativeAudio)
        flags |= GST_PLAY_FLAG_NATIVE_AUDIO;
    m_playbin.set("flags", flags);
}

void QGstreamerAudioDecoder::addAppSink()
{
    using namespace std::chrono_literals;

    if (m_appSink)
        return;

    qCDebug(qLcGstreamerAudioDecoder) << "QGstreamerAudioDecoder::addAppSink";
    m_appSink = QGstAppSink::create("decoderAppSink");
    GstAppSinkCallbacks callbacks{};
    callbacks.new_sample = new_sample;
    m_appSink.setCallbacks(callbacks, this, nullptr);

#if GST_CHECK_VERSION(1, 24, 0)
    static constexpr auto maxBufferTime = 500ms;
    m_appSink.setMaxBufferTime(maxBufferTime);
#else
    static constexpr int maxBuffers = 16;
    m_appSink.setMaxBuffers(maxBuffers);
#endif

    static constexpr bool sync = false;
    m_appSink.setSync(sync);

    QGstPipeline::modifyPipelineWhileNotRunning(m_playbin.getPipeline(), [&] {
        m_outputBin.add(m_appSink);
        qLinkGstElements(m_audioConvert, m_appSink);
    });
}

void QGstreamerAudioDecoder::removeAppSink()
{
    if (!m_appSink)
        return;

    qCDebug(qLcGstreamerAudioDecoder) << "QGstreamerAudioDecoder::removeAppSink";

    QGstPipeline::modifyPipelineWhileNotRunning(m_playbin.getPipeline(), [&] {
        qUnlinkGstElements(m_audioConvert, m_appSink);
        m_outputBin.stopAndRemoveElements(m_appSink);
    });
    m_appSink = {};
}

void QGstreamerAudioDecoder::updateDuration()
{
    std::chrono::milliseconds duration = m_playbin.durationInMs();

    if (m_duration != duration) {
        m_duration = duration;
        durationChanged(m_duration.count());
    }

    if (m_duration.count() > 0)
        m_durationQueries = 0;

    if (m_durationQueries > 0) {
        //increase delay between duration requests
        int delay = 25 << (5 - m_durationQueries);
        QTimer::singleShot(delay, this, &QGstreamerAudioDecoder::updateDuration);
        m_durationQueries--;
    }
}

std::chrono::nanoseconds QGstreamerAudioDecoder::getPositionFromBuffer(GstBuffer *buffer)
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    nanoseconds position{ GST_BUFFER_TIMESTAMP(buffer) };
    if (position >= 0ns)
        return position;
    else
        return invalidPosition;
}

QT_END_NAMESPACE

#include "moc_qgstreameraudiodecoder_p.cpp"
