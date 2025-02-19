// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QCUSTOMLABELITEM_H
#define QCUSTOMLABELITEM_H

#if 0
#  pragma qt_class(QCustom3DLabel)
#endif

#include <QtGraphs/qcustom3ditem.h>
#include <QtGraphs/qgraphsglobal.h>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGui/QQuaternion>
#include <QtGui/QVector3D>

QT_BEGIN_NAMESPACE

class QCustom3DLabelPrivate;

class QT_TECH_PREVIEW_API Q_GRAPHS_EXPORT QCustom3DLabel : public QCustom3DItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCustom3DLabel)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY
                   backgroundColorChanged)
    Q_PROPERTY(
        bool borderEnabled READ isBorderEnabled WRITE setBorderEnabled NOTIFY borderEnabledChanged)
    Q_PROPERTY(bool backgroundEnabled READ isBackgroundEnabled WRITE setBackgroundEnabled NOTIFY
                   backgroundEnabledChanged)
    Q_PROPERTY(bool facingCamera READ isFacingCamera WRITE setFacingCamera NOTIFY facingCameraChanged)

public:
    explicit QCustom3DLabel(QObject *parent = nullptr);
    explicit QCustom3DLabel(const QString &text,
                            const QFont &font,
                            const QVector3D &position,
                            const QVector3D &scaling,
                            const QQuaternion &rotation,
                            QObject *parent = nullptr);
    ~QCustom3DLabel() override;

    void setText(const QString &text);
    QString text() const;

    void setFont(const QFont &font);
    QFont font() const;

    void setTextColor(const QColor &color);
    QColor textColor() const;

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void setBorderEnabled(bool enabled);
    bool isBorderEnabled() const;

    void setBackgroundEnabled(bool enabled);
    bool isBackgroundEnabled() const;

    void setFacingCamera(bool enabled);
    bool isFacingCamera() const;

Q_SIGNALS:
    void textChanged(const QString &text);
    void fontChanged(const QFont &font);
    void textColorChanged(const QColor &color);
    void backgroundColorChanged(const QColor &color);
    void borderEnabledChanged(bool enabled);
    void backgroundEnabledChanged(bool enabled);
    void facingCameraChanged(bool enabled);

private:
    Q_DISABLE_COPY(QCustom3DLabel)
};

QT_END_NAMESPACE

#endif
