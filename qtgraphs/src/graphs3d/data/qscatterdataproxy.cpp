// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstract3daxis_p.h"
#include "qscatter3dseries_p.h"
#include "qscatterdataproxy_p.h"

QT_BEGIN_NAMESPACE

/*!
 * \class QScatterDataProxy
 * \inmodule QtGraphs
 * \ingroup graphs_3D
 * \brief The QScatterDataProxy class is the data proxy for 3D scatter graphs.
 *
 * A scatter data proxy handles adding, inserting, changing, and removing data
 * items.
 *
 * QScatterDataProxy takes ownership of all
 * QtGraphs::QScatterDataArray and QScatterDataItem objects passed to
 * it.
 *
 * \sa {Qt Graphs Data Handling with 3D}
 */

/*!
 * \typealias QScatterDataArray
 * \relates QScatterDataProxy
 *
 * A list of \l {QScatterDataItem} objects.
 */

/*!
 * \qmltype ScatterDataProxy
 * \inqmlmodule QtGraphs
 * \ingroup graphs_qml_3D
 * \instantiates QScatterDataProxy
 * \inherits AbstractDataProxy
 * \brief The data proxy for 3D scatter graphs.
 *
 * This type handles adding, inserting, changing, and removing data items.
 *
 * This type is uncreatable, but contains properties that are exposed via
 * subtypes.
 *
 * \sa ItemModelScatterDataProxy, {Qt Graphs Data Handling with 3D}
 */

/*!
 * \qmlproperty int ScatterDataProxy::itemCount
 * The number of items in the array.
 */

/*!
 * \qmlproperty Scatter3DSeries ScatterDataProxy::series
 *
 * The series this proxy is attached to.
 */

/*!
 * Constructs QScatterDataProxy with the given \a parent.
 */
QScatterDataProxy::QScatterDataProxy(QObject *parent)
    : QAbstractDataProxy(*(new QScatterDataProxyPrivate()), parent)
{}

/*!
 * \internal
 */
QScatterDataProxy::QScatterDataProxy(QScatterDataProxyPrivate &d, QObject *parent)
    : QAbstractDataProxy(d, parent)
{}

/*!
 * Deletes the scatter data proxy.
 */
QScatterDataProxy::~QScatterDataProxy() {}

/*!
 * \property QScatterDataProxy::series
 *
 * \brief The series this proxy is attached to.
 */
QScatter3DSeries *QScatterDataProxy::series() const
{
    const Q_D(QScatterDataProxy);
    return static_cast<QScatter3DSeries *>(d->series());
}

/*!
 * Clears the existing array and triggers the arrayReset() signal.
 */
void QScatterDataProxy::resetArray()
{
    Q_D(QScatterDataProxy);
    d->m_dataArray.clear();

    emit arrayReset();
    emit itemCountChanged(itemCount());
}

/*!
 * Sets the array from \a newArray. If the new array is equal to the
 * existing one, this function simply triggers the arrayReset() signal.
 */
void QScatterDataProxy::resetArray(QScatterDataArray newArray)
{
    Q_D(QScatterDataProxy);
    if (d->m_dataArray.data() != newArray.data())
        d->resetArray(std::move(newArray));

    emit arrayReset();
    emit itemCountChanged(itemCount());
}

/*!
 * Replaces the item at the position \a index with the item \a item.
 */
void QScatterDataProxy::setItem(int index, QScatterDataItem item)
{
    Q_D(QScatterDataProxy);
    d->setItem(index, std::move(item));
    emit itemsChanged(index, 1);
}

/*!
 * Replaces the items starting from the position \a index with the items
 * specified by \a items.
 */
void QScatterDataProxy::setItems(int index, QScatterDataArray items)
{
    Q_D(QScatterDataProxy);
    d->setItems(index, std::move(items));
    emit itemsChanged(index, items.size());
}

/*!
 * Adds the item \a item to the end of the array.
 *
 * Returns the index of the added item.
 */
int QScatterDataProxy::addItem(QScatterDataItem item)
{
    Q_D(QScatterDataProxy);
    int addIndex = d->addItem(std::move(item));
    emit itemsAdded(addIndex, 1);
    emit itemCountChanged(itemCount());
    return addIndex;
}

/*!
 * Adds the items specified by \a items to the end of the array.
 *
 * Returns the index of the first added item.
 */
int QScatterDataProxy::addItems(QScatterDataArray items)
{
    Q_D(QScatterDataProxy);
    int addIndex = d->addItems(std::move(items));
    emit itemsAdded(addIndex, items.size());
    emit itemCountChanged(itemCount());
    return addIndex;
}

/*!
 * Inserts the item \a item to the position \a index. If the index is equal to
 * the data array size, the item is added to the array.
 */
void QScatterDataProxy::insertItem(int index, QScatterDataItem item)
{
    Q_D(QScatterDataProxy);
    d->insertItem(index, std::move(item));
    emit itemsInserted(index, 1);
    emit itemCountChanged(itemCount());
}

/*!
 * Inserts the items specified by \a items to the position \a index. If the
 * index is equal to data array size, the items are added to the array.
 */
void QScatterDataProxy::insertItems(int index, QScatterDataArray items)
{
    Q_D(QScatterDataProxy);
    d->insertItems(index, std::move(items));
    emit itemsInserted(index, items.size());
    emit itemCountChanged(itemCount());
}

/*!
 * Removes the number of items specified by \a removeCount starting at the
 * position \a index. Attempting to remove items past the end of
 * the array does nothing.
 */
void QScatterDataProxy::removeItems(int index, int removeCount)
{
    Q_D(QScatterDataProxy);
    if (index >= d->m_dataArray.size())
        return;

    d->removeItems(index, removeCount);
    emit itemsRemoved(index, removeCount);
    emit itemCountChanged(itemCount());
}

/*!
 * \property QScatterDataProxy::itemCount
 *
 * \brief The number of items in the array.
 */
int QScatterDataProxy::itemCount() const
{
    const Q_D(QScatterDataProxy);
    return d->m_dataArray.size();
}

/*!
 * Returns the pointer to the data array.
 */
const QScatterDataArray &QScatterDataProxy::array() const
{
    const Q_D(QScatterDataProxy);
    return d->m_dataArray;
}

/*!
 * Returns the pointer to the item at the index \a index. It is guaranteed to be
 * valid only until the next call that modifies data.
 */
const QScatterDataItem &QScatterDataProxy::itemAt(int index) const
{
    const Q_D(QScatterDataProxy);
    return d->m_dataArray.at(index);
}

/*!
 * \fn void QScatterDataProxy::arrayReset()
 *
 * This signal is emitted when the data array is reset.
 * If the contents of the whole array are changed without calling resetArray(),
 * this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QScatterDataProxy::itemsAdded(int startIndex, int count)
 *
 * This signal is emitted when the number of items specified by \a count is
 * added starting at the position \a startIndex.
 * If items are added to the array without calling addItem() or addItems(),
 * this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QScatterDataProxy::itemsChanged(int startIndex, int count)
 *
 * This signal is emitted when the number of items specified by \a count is
 * changed starting at the position \a startIndex.
 * If items are changed in the array without calling setItem() or setItems(),
 * this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QScatterDataProxy::itemsRemoved(int startIndex, int count)
 *
 * This signal is emitted when the number of rows specified by \a count is
 * removed starting at the position \a startIndex.
 * The index may be larger than the current array size if items are removed from
 * the end. If items are removed from the array without calling removeItems(),
 * this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QScatterDataProxy::itemsInserted(int startIndex, int count)
 *
 * This signal is emitted when the number of items specified by \a count is
 * inserted starting at the position \a startIndex.
 * If items are inserted into the array without calling insertItem() or
 * insertItems(), this signal needs to be emitted to update the graph.
 */

// QScatterDataProxyPrivate

QScatterDataProxyPrivate::QScatterDataProxyPrivate()
    : QAbstractDataProxyPrivate(QAbstractDataProxy::DataType::Scatter)
{}

QScatterDataProxyPrivate::~QScatterDataProxyPrivate()
{
    m_dataArray.clear();
}

void QScatterDataProxyPrivate::resetArray(QScatterDataArray &&newArray)
{
    if (newArray.data() != m_dataArray.data()) {
        m_dataArray = newArray;
    }
}

void QScatterDataProxyPrivate::setItem(int index, QScatterDataItem &&item)
{
    Q_ASSERT(index >= 0 && index < m_dataArray.size());
    m_dataArray[index] = item;
}

void QScatterDataProxyPrivate::setItems(int index, QScatterDataArray &&items)
{
    Q_ASSERT(index >= 0 && (index + items.size()) <= m_dataArray.size());
    for (int i = 0; i < items.size(); i++)
        m_dataArray[index++] = items[i];
}

int QScatterDataProxyPrivate::addItem(QScatterDataItem &&item)
{
    int currentSize = m_dataArray.size();
    m_dataArray.append(item);
    return currentSize;
}

int QScatterDataProxyPrivate::addItems(QScatterDataArray &&items)
{
    int currentSize = m_dataArray.size();
    m_dataArray += items;
    return currentSize;
}

void QScatterDataProxyPrivate::insertItem(int index, QScatterDataItem &&item)
{
    Q_ASSERT(index >= 0 && index <= m_dataArray.size());
    m_dataArray.insert(index, item);
}

void QScatterDataProxyPrivate::insertItems(int index, QScatterDataArray &&items)
{
    Q_ASSERT(index >= 0 && index <= m_dataArray.size());
    for (int i = 0; i < items.size(); i++)
        m_dataArray.insert(index++, items.at(i));
}

void QScatterDataProxyPrivate::removeItems(int index, int removeCount)
{
    Q_ASSERT(index >= 0);
    int maxRemoveCount = m_dataArray.size() - index;
    removeCount = qMin(removeCount, maxRemoveCount);
    m_dataArray.remove(index, removeCount);
}

void QScatterDataProxyPrivate::limitValues(QVector3D &minValues,
                                           QVector3D &maxValues,
                                           QAbstract3DAxis *axisX,
                                           QAbstract3DAxis *axisY,
                                           QAbstract3DAxis *axisZ) const
{
    if (m_dataArray.isEmpty())
        return;

    const QVector3D &firstPos = m_dataArray.at(0).position();

    float minX = firstPos.x();
    float maxX = minX;
    float minY = firstPos.y();
    float maxY = minY;
    float minZ = firstPos.z();
    float maxZ = minZ;

    if (m_dataArray.size() > 1) {
        for (int i = 1; i < m_dataArray.size(); i++) {
            const QVector3D &pos = m_dataArray.at(i).position();

            float value = pos.x();
            if (qIsNaN(value) || qIsInf(value))
                continue;
            if (isValidValue(minX, value, axisX))
                minX = value;
            if (maxX < value)
                maxX = value;

            value = pos.y();
            if (qIsNaN(value) || qIsInf(value))
                continue;
            if (isValidValue(minY, value, axisY))
                minY = value;
            if (maxY < value)
                maxY = value;

            value = pos.z();
            if (qIsNaN(value) || qIsInf(value))
                continue;
            if (isValidValue(minZ, value, axisZ))
                minZ = value;
            if (maxZ < value)
                maxZ = value;
        }
    }

    minValues.setX(minX);
    minValues.setY(minY);
    minValues.setZ(minZ);

    maxValues.setX(maxX);
    maxValues.setY(maxY);
    maxValues.setZ(maxZ);
}

bool QScatterDataProxyPrivate::isValidValue(float axisValue,
                                            float value,
                                            QAbstract3DAxis *axis) const
{
    return (axisValue > value
            && (value > 0.0f || (value == 0.0f && axis->d_func()->allowZero())
                || (value < 0.0f && axis->d_func()->allowNegatives())));
}

void QScatterDataProxyPrivate::setSeries(QAbstract3DSeries *series)
{
    Q_Q(QScatterDataProxy);
    QAbstractDataProxyPrivate::setSeries(series);
    QScatter3DSeries *scatterSeries = static_cast<QScatter3DSeries *>(series);
    emit q->seriesChanged(scatterSeries);
}

QT_END_NAMESPACE
