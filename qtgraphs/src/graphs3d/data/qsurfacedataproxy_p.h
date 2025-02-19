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

#ifndef QSURFACEDATAPROXY_P_H
#define QSURFACEDATAPROXY_P_H

#include "qabstractdataproxy_p.h"
#include "qsurfacedataproxy.h"

QT_BEGIN_NAMESPACE

class QAbstract3DAxis;

class QSurfaceDataProxyPrivate : public QAbstractDataProxyPrivate
{
    Q_DECLARE_PUBLIC(QSurfaceDataProxy)

public:
    QSurfaceDataProxyPrivate();
    ~QSurfaceDataProxyPrivate() override;

    void resetArray(QSurfaceDataArray &&newArray);
    void setRow(int rowIndex, QSurfaceDataRow &&row);
    void setRows(int rowIndex, QSurfaceDataArray &&rows);
    void setItem(int rowIndex, int columnIndex, QSurfaceDataItem &&item);
    int addRow(QSurfaceDataRow &&row);
    int addRows(QSurfaceDataArray &&rows);
    void insertRow(int rowIndex, QSurfaceDataRow &&row);
    void insertRows(int rowIndex, QSurfaceDataArray &&rows);
    void removeRows(int rowIndex, int removeCount);
    void limitValues(QVector3D &minValues,
                     QVector3D &maxValues,
                     QAbstract3DAxis *axisX,
                     QAbstract3DAxis *axisY,
                     QAbstract3DAxis *axisZ) const;
    bool isValidValue(float value, QAbstract3DAxis *axis) const;

    void setSeries(QAbstract3DSeries *series) override;

protected:
    QSurfaceDataArray m_dataArray;

private:
    void clearRow(int rowIndex);
    void clearArray();
};

QT_END_NAMESPACE

#endif
