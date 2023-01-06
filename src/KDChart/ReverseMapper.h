/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef REVERSEMAPPER_H
#define REVERSEMAPPER_H

#include <QHash>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QRectF;
class QGraphicsScene;
class QPolygonF;
QT_END_NAMESPACE

namespace KDChart {

class AbstractDiagram;
class ChartGraphicsItem;

/**
 * @brief The ReverseMapper stores information about objects on a chart and their respective model indexes
 * \internal
 */
class ReverseMapper
{
public:
    ReverseMapper();
    explicit ReverseMapper(AbstractDiagram *diagram);

    ~ReverseMapper();

    void setDiagram(AbstractDiagram *diagram);

    void clear();

    QModelIndexList indexesAt(const QPointF &point) const;
    QModelIndexList indexesIn(const QRect &rect) const;

    QPolygonF polygon(int row, int column) const;
    QRectF boundingRect(int row, int column) const;

    void addItem(ChartGraphicsItem *item);

    // convenience methods:
    void addPolygon(int row, int column, const QPolygonF &polygon);
    void addRect(int row, int column, const QRectF &rect);
    void addCircle(int row, int column, const QPointF &location, const QSizeF &diameter);
    void addLine(int row, int column, const QPointF &from, const QPointF &to);

private:
    QGraphicsScene *m_scene = nullptr;
    AbstractDiagram *m_diagram = nullptr;
    QHash<QModelIndex, ChartGraphicsItem *> m_itemMap;
};
}

#endif
