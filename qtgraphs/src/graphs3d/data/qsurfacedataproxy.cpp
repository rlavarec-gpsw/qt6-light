// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstract3daxis_p.h"
#include "qsurface3dseries_p.h"
#include "qsurfacedataproxy_p.h"

QT_BEGIN_NAMESPACE

/*!
 * \class QSurfaceDataProxy
 * \inmodule QtGraphs
 * \ingroup graphs_3D
 * \brief The QSurfaceDataProxy class is the data proxy for a 3D surface graph.
 *
 * A surface data proxy handles surface related data in rows. For this it
 * provides two auxiliary type aliases: QtGraphs::QSurfaceDataArray and
 * QtGraphs::QSurfaceDataRow. \c QSurfaceDataArray is a QList that
 * controls the rows. \c QSurfaceDataRow is a QList that contains
 * QSurfaceDataItem objects. For more information about how to feed the data to
 * the proxy, see the sample code in the Q3DSurface documentation.
 *
 * All rows must have the same number of items.
 *
 * QSurfaceDataProxy takes ownership of all \c QSurfaceDataRow objects passed to
 * it, whether directly or in a \c QSurfaceDataArray container.
 * To use surface data row pointers to directly modify data after adding the
 * array to the proxy, the appropriate signal must be emitted to update the
 * graph.
 *
 * To make a sensible surface, the x-value of each successive item in all rows
 * must be either ascending or descending throughout the row. Similarly, the
 * z-value of each successive item in all columns must be either ascending or
 * descending throughout the column.
 *
 * \note Currently only surfaces with straight rows and columns are fully
 * supported. Any row with items that do not have the exact same z-value or any
 * column with items that do not have the exact same x-value may get clipped
 * incorrectly if the whole surface does not completely fit within the visible
 * x-axis or z-axis ranges.
 *
 * \note Surfaces with less than two rows or columns are not considered valid
 * surfaces and will not be rendered.
 *
 * \note On some environments, surfaces with a lot of visible vertices may not
 * render, because they exceed the per-draw vertex count supported by the
 * graphics driver. This is mostly an issue on 32-bit and OpenGL ES2 platforms.
 *
 * \sa {Qt Graphs Data Handling with 3D}
 */

/*!
 * \typealias QSurfaceDataRow
 * \relates QSurfaceDataProxy
 *
 * A list of \l {QSurfaceDataItem} objects.
 */

/*!
 * \typealias QSurfaceDataArray
 * \relates QSurfaceDataProxy
 *
 * A list of pointers to \l {QSurfaceDataRow} objects.
 */

/*!
 * \qmltype SurfaceDataProxy
 * \inqmlmodule QtGraphs
 * \ingroup graphs_qml_3D
 * \instantiates QSurfaceDataProxy
 * \inherits AbstractDataProxy
 * \brief The data proxy for a 3D surface graph.
 *
 * This type handles surface data items. The data is arranged into rows and
 * columns, and all rows must have the same number of columns.
 *
 * This type is uncreatable, but contains properties that are exposed via
 * subtypes.
 *
 * For a more complete description, see QSurfaceDataProxy.
 *
 * \sa ItemModelSurfaceDataProxy, {Qt Graphs Data Handling with 3D}
 */

/*!
 * \qmlproperty int SurfaceDataProxy::rowCount
 * The number of rows in the data array.
 */

/*!
 * \qmlproperty int SurfaceDataProxy::columnCount
 * The number of columns in the data array.
 */

/*!
 * \qmlproperty Surface3DSeries SurfaceDataProxy::series
 *
 * The series this proxy is attached to.
 */

/*!
 * Constructs QSurfaceDataProxy with the given \a parent.
 */
QSurfaceDataProxy::QSurfaceDataProxy(QObject *parent)
    : QAbstractDataProxy(*(new QSurfaceDataProxyPrivate()), parent)
{}

/*!
 * \internal
 */
QSurfaceDataProxy::QSurfaceDataProxy(QSurfaceDataProxyPrivate &d, QObject *parent)
    : QAbstractDataProxy(d, parent)
{}

/*!
 * Deletes the surface data proxy.
 */
QSurfaceDataProxy::~QSurfaceDataProxy() {}

/*!
 * \property QSurfaceDataProxy::series
 *
 *  \brief The series this proxy is attached to.
 */
QSurface3DSeries *QSurfaceDataProxy::series() const
{
    const Q_D(QSurfaceDataProxy);
    return static_cast<QSurface3DSeries *>(d->series());
}

/*!
 * Clears the existing array and triggers the arrayReset() signal.
 */
void QSurfaceDataProxy::resetArray()
{
    Q_D(QSurfaceDataProxy);
    d->resetArray(QSurfaceDataArray());

    emit arrayReset();
    emit rowCountChanged(rowCount());
    emit columnCountChanged(columnCount());
}

/*!
 * Sets the array from \a newArray. If the new array is equal to the
 * existing one, this function simply triggers the arrayReset() signal.
*/
void QSurfaceDataProxy::resetArray(QSurfaceDataArray newArray)
{
    Q_D(QSurfaceDataProxy);
    if (d->m_dataArray.data() != newArray.data()) {
        d->resetArray(std::move(newArray));
    }

    emit arrayReset();
    emit rowCountChanged(rowCount());
    emit columnCountChanged(columnCount());
}

/*!
 * Changes an existing row by replacing the row at the position \a rowIndex
 * with the new row specified by \a row. The new row can be the same as the
 * existing row already stored at the \a rowIndex. The new row must have
 * the same number of columns as the row it is replacing.
 */
void QSurfaceDataProxy::setRow(int rowIndex, QSurfaceDataRow row)
{
    Q_D(QSurfaceDataProxy);
    d->setRow(rowIndex, std::move(row));
    emit rowsChanged(rowIndex, 1);
}

/*!
 * Changes existing rows by replacing the rows starting at the position
 * \a rowIndex with the new rows specifies by \a rows.
 * The rows in the \a rows array can be the same as the existing rows already
 * stored at the \a rowIndex. The new rows must have the same number of columns
 * as the rows they are replacing.
 */
void QSurfaceDataProxy::setRows(int rowIndex, QSurfaceDataArray rows)
{
    Q_D(QSurfaceDataProxy);
    d->setRows(rowIndex, std::move(rows));
    emit rowsChanged(rowIndex, rows.size());
}

/*!
 * Changes a single item at the position specified by \a rowIndex and
 * \a columnIndex to the item \a item.
 */
void QSurfaceDataProxy::setItem(int rowIndex, int columnIndex, QSurfaceDataItem item)
{
    Q_D(QSurfaceDataProxy);
    d->setItem(rowIndex, columnIndex, std::move(item));
    emit itemChanged(rowIndex, columnIndex);
}

/*!
 * Changes a single item at the position \a position to the item \a item.
 * The x-value of \a position indicates the row and the y-value indicates the
 * column.
 */
void QSurfaceDataProxy::setItem(const QPoint &position, QSurfaceDataItem item)
{
    setItem(position.x(), position.y(), item);
}

/*!
 * Adds the new row \a row to the end of an array. The new row must have
 * the same number of columns as the rows in the initial array.
 *
 * Returns the index of the added row.
 */
int QSurfaceDataProxy::addRow(QSurfaceDataRow row)
{
    Q_D(QSurfaceDataProxy);
    int addIndex = d->addRow(std::move(row));
    emit rowsAdded(addIndex, 1);
    emit rowCountChanged(rowCount());
    return addIndex;
}

/*!
 * Adds new \a rows to the end of an array. The new rows must have the same
 * number of columns as the rows in the initial array.
 *
 * Returns the index of the first added row.
 */
int QSurfaceDataProxy::addRows(QSurfaceDataArray rows)
{
    Q_D(QSurfaceDataProxy);
    int addIndex = d->addRows(std::move(rows));
    emit rowsAdded(addIndex, rows.size());
    emit rowCountChanged(rowCount());
    return addIndex;
}

/*!
 * Inserts the new row \a row into \a rowIndex.
 * If \a rowIndex is equal to the array size, the rows are added to the end of
 * the array. The new row must have the same number of columns as the rows in
 * the initial array.
 */
void QSurfaceDataProxy::insertRow(int rowIndex, QSurfaceDataRow row)
{
    Q_D(QSurfaceDataProxy);
    d->insertRow(rowIndex, std::move(row));
    emit rowsInserted(rowIndex, 1);
    emit rowCountChanged(rowCount());
}

/*!
 * Inserts new \a rows into \a rowIndex.
 * If \a rowIndex is equal to the array size, the rows are added to the end of
 * the array. The new \a rows must have the same number of columns as the rows
 * in the initial array.
 */
void QSurfaceDataProxy::insertRows(int rowIndex, QSurfaceDataArray rows)
{
    Q_D(QSurfaceDataProxy);
    d->insertRows(rowIndex, std::move(rows));
    emit rowsInserted(rowIndex, rows.size());
    emit rowCountChanged(rowCount());
}

/*!
 * Removes the number of rows specified by \a removeCount starting at the
 * position \a rowIndex. Attempting to remove rows past the end of the
 * array does nothing.
 */
void QSurfaceDataProxy::removeRows(int rowIndex, int removeCount)
{
    Q_D(QSurfaceDataProxy);
    if (rowIndex < rowCount() && removeCount >= 1) {
        d->removeRows(rowIndex, removeCount);
        emit rowsRemoved(rowIndex, removeCount);
        emit rowCountChanged(rowCount());
    }
}

/*!
 * Returns the pointer to the data array.
 */
const QSurfaceDataArray &QSurfaceDataProxy::array() const
{
    const Q_D(QSurfaceDataProxy);
    return d->m_dataArray;
}

/*!
 * Returns the pointer to the item at the position specified by \a rowIndex and
 * \a columnIndex. It is guaranteed to be valid only
 * until the next call that modifies data.
 */
const QSurfaceDataItem &QSurfaceDataProxy::itemAt(int rowIndex, int columnIndex) const
{
    const Q_D(QSurfaceDataProxy);
    const QSurfaceDataArray &dataArray = d->m_dataArray;
    Q_ASSERT(rowIndex >= 0 && rowIndex < dataArray.size());
    const QSurfaceDataRow &dataRow = dataArray[rowIndex];
    Q_ASSERT(columnIndex >= 0 && columnIndex < dataRow.size());
    return dataRow.at(columnIndex);
}

/*!
 * Returns the pointer to the item at the position \a position. The x-value of
 * \a position indicates the row and the y-value indicates the column. The item
 * is guaranteed to be valid only until the next call that modifies data.
 */
const QSurfaceDataItem &QSurfaceDataProxy::itemAt(const QPoint &position) const
{
    return itemAt(position.x(), position.y());
}

/*!
 * \property QSurfaceDataProxy::rowCount
 *
 * \brief The number of rows in the data array.
 */
int QSurfaceDataProxy::rowCount() const
{
    const Q_D(QSurfaceDataProxy);
    return d->m_dataArray.size();
}

/*!
 * \property QSurfaceDataProxy::columnCount
 *
 * \brief The number of columns in the data array.
 */
int QSurfaceDataProxy::columnCount() const
{
    const Q_D(QSurfaceDataProxy);
    if (d->m_dataArray.size() > 0)
        return d->m_dataArray.at(0).size();
    else
        return 0;
}

/*!
 * \fn void QSurfaceDataProxy::arrayReset()
 *
 * This signal is emitted when the data array is reset.
 * If the contents of the whole array are changed without calling resetArray(),
 * this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QSurfaceDataProxy::rowsAdded(int startIndex, int count)
 *
 * This signal is emitted when the number of rows specified by \a count is
 * added starting at the position \a startIndex.
 * If rows are added to the array without calling addRow() or addRows(),
 * this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QSurfaceDataProxy::rowsChanged(int startIndex, int count)
 *
 * This signal is emitted when the number of rows specified by \a count is
 * changed starting at the position \a startIndex.
 * If rows are changed in the array without calling setRow() or setRows(),
 * this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QSurfaceDataProxy::rowsRemoved(int startIndex, int count)
 *
 * This signal is emitted when the number of rows specified by \a count is
 * removed starting at the position \a startIndex.
 *
 * The index is the current array size if the rows were removed from the end of
 * the array. If rows are removed from the array without calling removeRows(),
 * this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QSurfaceDataProxy::rowsInserted(int startIndex, int count)
 *
 * This signal is emitted when the number of rows specified by \a count is
 * inserted at the position \a startIndex.
 *
 * If rows are inserted into the array without calling insertRow() or
 * insertRows(), this signal needs to be emitted to update the graph.
 */

/*!
 * \fn void QSurfaceDataProxy::itemChanged(int rowIndex, int columnIndex)
 *
 * This signal is emitted when the item at the position specified by \a rowIndex
 * and \a columnIndex changes.
 * If the item is changed in the array without calling setItem(),
 * this signal needs to be emitted to update the graph.
 */

//  QSurfaceDataProxyPrivate

QSurfaceDataProxyPrivate::QSurfaceDataProxyPrivate()
    : QAbstractDataProxyPrivate(QAbstractDataProxy::DataType::Surface)
{}

QSurfaceDataProxyPrivate::~QSurfaceDataProxyPrivate()
{
    clearArray();
}

void QSurfaceDataProxyPrivate::resetArray(QSurfaceDataArray &&newArray)
{
    if (newArray.data() != m_dataArray.data()) {
        clearArray();
        m_dataArray = newArray;
    }
}

void QSurfaceDataProxyPrivate::setRow(int rowIndex, QSurfaceDataRow &&row)
{
    Q_ASSERT(rowIndex >= 0 && rowIndex < m_dataArray.size());
    Q_ASSERT(m_dataArray.at(rowIndex).size() == row.size());

    if (row.data() != m_dataArray.at(rowIndex).data()) {
        clearRow(rowIndex);
        m_dataArray[rowIndex] = row;
    }
}

void QSurfaceDataProxyPrivate::setRows(int rowIndex, QSurfaceDataArray &&rows)
{
    QSurfaceDataArray &dataArray = m_dataArray;
    Q_ASSERT(rowIndex >= 0 && (rowIndex + rows.size()) <= dataArray.size());

    for (int i = 0; i < rows.size(); i++) {
        Q_ASSERT(m_dataArray.at(rowIndex).size() == rows.at(i).size());
        if (rows.at(i).data() != dataArray.at(rowIndex).data()) {
            clearRow(rowIndex);
            dataArray[rowIndex] = rows.at(i);
        }
        rowIndex++;
    }
}

void QSurfaceDataProxyPrivate::setItem(int rowIndex, int columnIndex, QSurfaceDataItem &&item)
{
    Q_ASSERT(rowIndex >= 0 && rowIndex < m_dataArray.size());
    QSurfaceDataRow &row = m_dataArray[rowIndex];
    Q_ASSERT(columnIndex < row.size());
    row[columnIndex] = item;
}

int QSurfaceDataProxyPrivate::addRow(QSurfaceDataRow &&row)
{
    Q_ASSERT(m_dataArray.isEmpty() || m_dataArray.at(0).size() == row.size());
    int currentSize = m_dataArray.size();
    m_dataArray.append(row);
    return currentSize;
}

int QSurfaceDataProxyPrivate::addRows(QSurfaceDataArray &&rows)
{
    int currentSize = m_dataArray.size();
    for (int i = 0; i < rows.size(); i++) {
        Q_ASSERT(m_dataArray.isEmpty() || m_dataArray.at(0).size() == rows.at(i).size());
        m_dataArray.append(rows.at(i));
    }
    return currentSize;
}

void QSurfaceDataProxyPrivate::insertRow(int rowIndex, QSurfaceDataRow &&row)
{
    Q_ASSERT(rowIndex >= 0 && rowIndex <= m_dataArray.size());
    Q_ASSERT(m_dataArray.isEmpty() || m_dataArray.at(0).size() == row.size());
    m_dataArray.insert(rowIndex, row);
}

void QSurfaceDataProxyPrivate::insertRows(int rowIndex, QSurfaceDataArray &&rows)
{
    Q_ASSERT(rowIndex >= 0 && rowIndex <= m_dataArray.size());

    for (int i = 0; i < rows.size(); i++) {
        Q_ASSERT(m_dataArray.isEmpty() || m_dataArray.at(0).size() == rows.at(i).size());
        m_dataArray.insert(rowIndex++, rows.at(i));
    }
}

void QSurfaceDataProxyPrivate::removeRows(int rowIndex, int removeCount)
{
    Q_ASSERT(rowIndex >= 0);
    int maxRemoveCount = m_dataArray.size() - rowIndex;
    removeCount = qMin(removeCount, maxRemoveCount);
    for (int i = 0; i < removeCount; i++) {
        clearRow(rowIndex);
        m_dataArray.removeAt(rowIndex);
    }
}

void QSurfaceDataProxyPrivate::limitValues(QVector3D &minValues,
                                           QVector3D &maxValues,
                                           QAbstract3DAxis *axisX,
                                           QAbstract3DAxis *axisY,
                                           QAbstract3DAxis *axisZ) const
{
    float min = 0.0f;
    float max = 0.0f;

    int rows = m_dataArray.size();
    int columns = 0;
    if (rows)
        columns = m_dataArray.at(0).size();

    if (rows && columns) {
        min = m_dataArray.at(0).at(0).y();
        max = m_dataArray.at(0).at(0).y();
    }

    for (int i = 0; i < rows; i++) {
        const QSurfaceDataRow &row = m_dataArray.at(i);
        if (!row.isEmpty()) {
            for (int j = 0; j < columns; j++) {
                float itemValue = m_dataArray.at(i).at(j).y();
                if (qIsNaN(itemValue) || qIsInf(itemValue))
                    continue;
                if ((min > itemValue || (qIsNaN(min) || qIsInf(min)))
                    && isValidValue(itemValue, axisY)) {
                    min = itemValue;
                }
                if (max < itemValue || (qIsNaN(max) || qIsInf(max)))
                    max = itemValue;
            }
        }
    }

    minValues.setY(min);
    maxValues.setY(max);

    if (columns) {
        // Have some defaults
        float xLow = m_dataArray.at(0).at(0).x();
        float xHigh = m_dataArray.at(0).last().x();
        float zLow = m_dataArray.at(0).at(0).z();
        float zHigh = m_dataArray.last().at(0).z();
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                float zItemValue = m_dataArray.at(i).at(j).z();
                if (qIsNaN(zItemValue) || qIsInf(zItemValue))
                    continue;
                else if (isValidValue(zItemValue, axisZ))
                    zLow = qMin(zLow, zItemValue);
            }
            if (!qIsNaN(zLow) && !qIsInf(zLow))
                break;
        }
        for (int i = rows - 1; i >= 0; i--) {
            for (int j = 0; j < columns; j++) {
                float zItemValue = m_dataArray.at(i).at(j).z();
                if (qIsNaN(zItemValue) || qIsInf(zItemValue))
                    continue;
                else if (isValidValue(zItemValue, axisZ)) {
                    if (!qIsNaN(zHigh) && !qIsInf(zHigh))
                        zHigh = qMax(zHigh, zItemValue);
                    else
                        zHigh = zItemValue;
                }
            }
            if (!qIsNaN(zHigh) && !qIsInf(zHigh))
                break;
        }
        for (int j = 0; j < columns; j++) {
            for (int i = 0; i < rows; i++) {
                float xItemValue = m_dataArray.at(i).at(j).x();
                if (qIsNaN(xItemValue) || qIsInf(xItemValue))
                    continue;
                else if (isValidValue(xItemValue, axisX))
                    xLow = qMin(xLow, xItemValue);
            }
            if (!qIsNaN(xLow) && !qIsInf(xLow))
                break;
        }
        for (int j = columns - 1; j >= 0; j--) {
            for (int i = 0; i < rows; i++) {
                float xItemValue = m_dataArray.at(i).at(j).x();
                if (qIsNaN(xItemValue) || qIsInf(xItemValue)) {
                    continue;
                } else if (isValidValue(xItemValue, axisX)) {
                    if (!qIsNaN(xHigh) && !qIsInf(xHigh))
                        xHigh = qMax(xHigh, xItemValue);
                    else
                        xHigh = xItemValue;
                }
            }
            if (!qIsNaN(xHigh) && !qIsInf(xHigh))
                break;
        }
        minValues.setX(xLow);
        minValues.setZ(zLow);
        maxValues.setX(xHigh);
        maxValues.setZ(zHigh);
    } else {
        minValues.setX(axisX->d_func()->allowZero() ? 0.0f : 1.0f);
        minValues.setZ(axisZ->d_func()->allowZero() ? 0.0f : 1.0f);
        maxValues.setX(axisX->d_func()->allowZero() ? 0.0f : 1.0f);
        maxValues.setZ(axisZ->d_func()->allowZero() ? 0.0f : 1.0f);
    }
}

bool QSurfaceDataProxyPrivate::isValidValue(float value, QAbstract3DAxis *axis) const
{
    return (value > 0.0f || (value == 0.0f && axis->d_func()->allowZero())
            || (value < 0.0f && axis->d_func()->allowNegatives()));
}

void QSurfaceDataProxyPrivate::clearRow(int rowIndex)
{
    m_dataArray[rowIndex].clear();
}

void QSurfaceDataProxyPrivate::clearArray()
{
    m_dataArray.clear();
}

void QSurfaceDataProxyPrivate::setSeries(QAbstract3DSeries *series)
{
    Q_Q(QSurfaceDataProxy);
    QAbstractDataProxyPrivate::setSeries(series);
    QSurface3DSeries *surfaceSeries = static_cast<QSurface3DSeries *>(series);
    emit q->seriesChanged(surfaceSeries);
}

QT_END_NAMESPACE
