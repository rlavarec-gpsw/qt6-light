// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACT3DAXIS_H
#define QABSTRACT3DAXIS_H

#if 0
#  pragma qt_class(QAbstract3DAxis)
#endif

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QStringList>
#include <QtCore/qtclasshelpermacros.h>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QAbstract3DAxisPrivate;

class QT_TECH_PREVIEW_API Q_GRAPHS_EXPORT QAbstract3DAxis : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstract3DAxis)
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QStringList labels READ labels WRITE setLabels NOTIFY labelsChanged)
    Q_PROPERTY(
        QAbstract3DAxis::AxisOrientation orientation READ orientation NOTIFY orientationChanged)
    Q_PROPERTY(QAbstract3DAxis::AxisType type READ type CONSTANT)
    Q_PROPERTY(float min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(float max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(bool autoAdjustRange READ isAutoAdjustRange WRITE setAutoAdjustRange NOTIFY
                   autoAdjustRangeChanged)
    Q_PROPERTY(float labelAutoRotation READ labelAutoRotation WRITE setLabelAutoRotation NOTIFY
                   labelAutoRotationChanged)
    Q_PROPERTY(
        bool titleVisible READ isTitleVisible WRITE setTitleVisible NOTIFY titleVisibilityChanged)
    Q_PROPERTY(bool titleFixed READ isTitleFixed WRITE setTitleFixed NOTIFY titleFixedChanged)

public:
    enum class AxisOrientation { None, X, Y, Z };
    Q_ENUM(AxisOrientation)

    enum class AxisType { None, Category, Value };
    Q_ENUM(AxisType)

protected:
    explicit QAbstract3DAxis(QAbstract3DAxisPrivate &d, QObject *parent = nullptr);

public:
    ~QAbstract3DAxis() override;

    void setTitle(const QString &title);
    QString title() const;

    void setLabels(const QStringList &labels);
    QStringList labels() const;

    QAbstract3DAxis::AxisOrientation orientation() const;
    QAbstract3DAxis::AxisType type() const;

    void setMin(float min);
    float min() const;

    void setMax(float max);
    float max() const;

    void setAutoAdjustRange(bool autoAdjust);
    bool isAutoAdjustRange() const;

    void setRange(float min, float max);

    void setLabelAutoRotation(float angle);
    float labelAutoRotation() const;

    void setTitleVisible(bool visible);
    bool isTitleVisible() const;

    void setTitleFixed(bool fixed);
    bool isTitleFixed() const;

Q_SIGNALS:
    void titleChanged(const QString &newTitle);
    void labelsChanged();
    void orientationChanged(QAbstract3DAxis::AxisOrientation orientation);
    void minChanged(float value);
    void maxChanged(float value);
    void rangeChanged(float min, float max);
    void autoAdjustRangeChanged(bool autoAdjust);
    void labelAutoRotationChanged(float angle);
    void titleVisibilityChanged(bool visible);
    void titleFixedChanged(bool fixed);


private:
    Q_DISABLE_COPY(QAbstract3DAxis)

    friend class QQuickGraphsItem;
    friend class QScatterDataProxyPrivate;
    friend class QSurfaceDataProxyPrivate;
};

QT_END_NAMESPACE

#endif
