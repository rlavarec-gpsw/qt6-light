// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QSURFACE3DSERIES_P_H
#define QSURFACE3DSERIES_P_H

#include "qabstract3dseries_p.h"
#include "qsurface3dseries.h"

QT_BEGIN_NAMESPACE

class QSurface3DSeriesPrivate : public QAbstract3DSeriesPrivate
{
    Q_DECLARE_PUBLIC(QSurface3DSeries)

public:
    QSurface3DSeriesPrivate();
    ~QSurface3DSeriesPrivate() override;

    void setDataProxy(QAbstractDataProxy *proxy) override;
    void connectGraphAndProxy(QQuickGraphsItem *newGraph) override;
    void createItemLabel() override;

    void setSelectedPoint(const QPoint &position);
    void setFlatShadingEnabled(bool enabled);
    void setDrawMode(QSurface3DSeries::DrawFlags mode);
    void setTexture(const QImage &texture);
    void setWireframeColor(const QColor &color);

private:
    QPoint m_selectedPoint;
    bool m_flatShadingEnabled;
    QSurface3DSeries::DrawFlags m_drawMode;
    QImage m_texture;
    QString m_textureFile;
    QColor m_wireframeColor;
};

QT_END_NAMESPACE

#endif
