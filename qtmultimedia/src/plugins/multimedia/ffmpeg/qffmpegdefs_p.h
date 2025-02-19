// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QFFMPEGDEFS_P_H
#define QFFMPEGDEFS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

#define QT_FFMPEG_OLD_CHANNEL_LAYOUT (LIBAVCODEC_VERSION_INT < AV_VERSION_INT(59, 24, 100))
#define QT_FFMPEG_HAS_VULKAN \
  (LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(58, 91, 100)) // since ffmpeg n4.3
#define QT_FFMPEG_HAS_FRAME_TIME_BASE \
  (LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(59, 18, 100)) // since ffmpeg n5.0
#define QT_FFMPEG_HAS_FRAME_DURATION \
  (LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(60, 3, 100)) // since ffmpeg n6.0
#define QT_FFMPEG_STREAM_SIDE_DATA_DEPRECATED \
  (LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(60, 15, 100)) // since ffmpeg n6.1
#define QT_FFMPEG_HAS_D3D12VA \
    (LIBAVUTIL_VERSION_INT >= AV_VERSION_INT(59, 8, 100)) // since ffmpeg n7.0
#define QT_FFMPEG_SWR_CONST_CH_LAYOUT (LIBSWRESAMPLE_VERSION_INT >= AV_VERSION_INT(4, 9, 100))
#define QT_FFMPEG_AVIO_WRITE_CONST \
  (LIBAVFORMAT_VERSION_MAJOR >= 61)

#endif // QFFMPEGDEFS_P_H
