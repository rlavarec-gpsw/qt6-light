// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "q3dscatter.h"
#include "qquickgraphsscatter_p.h"

QT_BEGIN_NAMESPACE

/*!
 * \class Q3DScatter
 * \inmodule QtGraphs
 * \ingroup graphs_3D
 * \brief The Q3DScatter class provides methods for rendering 3D scatter graphs.
 *
 * This class enables developers to render scatter graphs in 3D and to view them
 * by rotating the scene freely. Rotation is done by holding down the right
 * mouse button and moving the mouse. Zooming is done by mouse wheel. Selection,
 * if enabled, is done by left mouse button. The scene can be reset to default
 * camera view by clicking mouse wheel. In touch devices rotation is done by
 * tap-and-move, selection by tap-and-hold and zoom by pinch.
 *
 * If no axes are set explicitly to Q3DScatter, temporary default axes with no
 * labels are created. These default axes can be modified via axis accessors,
 * but as soon any axis is set explicitly for the orientation, the default axis
 * for that orientation is destroyed.
 *
 * Q3DScatter supports more than one series visible at the same time.
 *
 * \section1 How to construct a minimal Q3DScatter graph
 *
 * First, construct Q3DScatter. Since we are running the graph as top level
 * window in this example, we need to clear the \c Qt::FramelessWindowHint flag,
 * which gets set by default:
 *
 * \snippet doc_src_q3dscatter_construction.cpp 0
 *
 * Now Q3DScatter is ready to receive data to be rendered. Add one series of 3
 * QVector3D items:
 *
 * \snippet doc_src_q3dscatter_construction.cpp 1
 *
 * Finally you will need to set it visible:
 *
 * \snippet doc_src_q3dscatter_construction.cpp 2
 *
 * The complete code needed to create and display this graph is:
 *
 * \snippet doc_src_q3dscatter_construction.cpp 3
 *
 * And this is what those few lines of code produce:
 *
 * \image q3dscatter-minimal.png
 *
 * The scene can be rotated, zoomed into, and an item can be selected to view
 * its position, but no other interaction is included in this minimal code
 * example. You can learn more by familiarizing yourself with the examples
 * provided, like the \l{Simple Scatter Graph}.
 *
 * \sa Q3DBars, Q3DSurface, {Qt Graphs C++ Classes for 3D}
 */

/*!
 * Constructs a new 3D scatter graph.
 */
Q3DScatter::Q3DScatter()
    : QAbstract3DGraph(QStringLiteral("Scatter3D"))
{}

/*!
 * Destroys the 3D scatter graph.
 */
Q3DScatter::~Q3DScatter() {}

/*!
 * Adds the \a series to the graph. A graph can contain multiple series, but has
 * only one set of axes. If the newly added series has specified a selected
 * item, it will be highlighted and any existing selection will be cleared. Only
 * one added series can have an active selection.
 *
 * \sa QAbstract3DGraph::hasSeries()
 */
void Q3DScatter::addSeries(QScatter3DSeries *series)
{
    graphScatter()->addSeries(series);
}

/*!
 * Removes the \a series from the graph.
 *
 * \sa QAbstract3DGraph::hasSeries()
 */
void Q3DScatter::removeSeries(QScatter3DSeries *series)
{
    graphScatter()->removeSeries(series);
}

/*!
 * Returns the list of series added to this graph.
 *
 * \sa QAbstract3DGraph::hasSeries()
 */
QList<QScatter3DSeries *> Q3DScatter::seriesList() const
{
    QList<QScatter3DSeries *> scatterSeriesList;
    for (QAbstract3DSeries *abstractSeries : graphScatterc()->m_seriesList) {
        QScatter3DSeries *scatterSeries = qobject_cast<QScatter3DSeries *>(abstractSeries);
        if (scatterSeries)
            scatterSeriesList.append(scatterSeries);
    }

    return scatterSeriesList;
}

/*!
 * \property Q3DScatter::axisX
 *
 * \brief The active x-axis.
 *
 * Sets \a axis as the active x-axis. Implicitly calls addAxis() to transfer the
 * ownership of the axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels and an
 * automatically adjusting range is created. This temporary axis is destroyed if
 * another axis is set explicitly to the same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DScatter::setAxisX(QValue3DAxis *axis)
{
    graphScatter()->setAxisX(axis);
}

QValue3DAxis *Q3DScatter::axisX() const
{
    return static_cast<QValue3DAxis *>(graphScatterc()->axisX());
}

/*!
 * \property Q3DScatter::axisY
 *
 * \brief The active y-axis.
 *
 * Sets \a axis as the active y-axis. Implicitly calls addAxis() to transfer the
 * ownership of the axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels and an
 * automatically adjusting range is created. This temporary axis is destroyed if
 * another axis is set explicitly to the same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DScatter::setAxisY(QValue3DAxis *axis)
{
    graphScatter()->setAxisY(axis);
}

QValue3DAxis *Q3DScatter::axisY() const
{
    return static_cast<QValue3DAxis *>(graphScatterc()->axisY());
}

/*!
 * \property Q3DScatter::axisZ
 *
 * \brief The active z-axis.
 *
 * Sets \a axis as the active z-axis. Implicitly calls addAxis() to transfer the
 * ownership of the axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels and an
 * automatically adjusting range is created. This temporary axis is destroyed if
 * another axis is set explicitly to the same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DScatter::setAxisZ(QValue3DAxis *axis)
{
    graphScatter()->setAxisZ(axis);
}

QValue3DAxis *Q3DScatter::axisZ() const
{
    return static_cast<QValue3DAxis *>(graphScatterc()->axisZ());
}

/*!
 * \property Q3DScatter::selectedSeries
 *
 * \brief The selected series or null.
 */
QScatter3DSeries *Q3DScatter::selectedSeries() const
{
    return graphScatterc()->selectedSeries();
}

/*!
 * Adds \a axis to the graph. The axes added via addAxis are not yet taken to
 * use, addAxis is simply used to give the ownership of the \a axis to the
 * graph. The \a axis must not be null or added to another graph.
 *
 * \sa releaseAxis(), setAxisX(), setAxisY(), setAxisZ()
 */
void Q3DScatter::addAxis(QValue3DAxis *axis)
{
    graphScatter()->addAxis(axis);
}

/*!
 * Releases the ownership of the \a axis back to the caller, if it is added to
 * this graph. If the released \a axis is in use, a new default axis will be
 * created and set active.
 *
 * If the default axis is released and added back later, it behaves as any other
 * axis would.
 *
 * \sa addAxis(), setAxisX(), setAxisY(), setAxisZ()
 */
void Q3DScatter::releaseAxis(QValue3DAxis *axis)
{
    graphScatter()->releaseAxis(axis);
}

/*!
 * Returns the list of all added axes.
 *
 * \sa addAxis()
 */
QList<QValue3DAxis *> Q3DScatter::axes() const
{
    QList<QAbstract3DAxis *> abstractAxes = graphScatterc()->axes();
    QList<QValue3DAxis *> retList;
    for (QAbstract3DAxis *axis : abstractAxes)
        retList.append(static_cast<QValue3DAxis *>(axis));

    return retList;
}

/*!
 * \internal
 */
QQuickGraphsScatter *Q3DScatter::graphScatter()
{
    return static_cast<QQuickGraphsScatter *>(m_graphsItem.data());
}

/*!
 * \internal
 */
const QQuickGraphsScatter *Q3DScatter::graphScatterc() const
{
    return static_cast<const QQuickGraphsScatter *>(m_graphsItem.data());
}

QT_END_NAMESPACE
