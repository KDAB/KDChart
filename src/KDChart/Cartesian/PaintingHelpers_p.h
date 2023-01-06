/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef PAINTINGHELPERS_P_H
#define PAINTINGHELPERS_P_H

#include "KDChartAbstractDiagram_p.h"
#include <KDABLibFakes>

#include <QPointF>
#include <QVector>

QT_BEGIN_NAMESPACE
class QBrush;
class QModelIndex;
class QPen;
class QPolygonF;
QT_END_NAMESPACE

namespace KDChart {

class LineAttributesInfo;
typedef QVector<LineAttributesInfo> LineAttributesInfoList;
class ThreeDLineAttributes;
class ValueTrackerAttributes;

namespace PaintingHelpers {

inline bool isFinite(const QPointF &point)
{
    return !ISINF(point.x()) && !ISNAN(point.x()) && !ISINF(point.y()) && !ISNAN(point.y());
}

const QPointF project(const QPointF &point, const ThreeDLineAttributes &tdAttributes);
void paintPolyline(PaintContext *ctx, const QBrush &brush, const QPen &pen, const QPolygonF &points);
void paintThreeDLines(PaintContext *ctx, AbstractDiagram *diagram, const QModelIndex &index,
                      const QPointF &from, const QPointF &to, const ThreeDLineAttributes &tdAttributes,
                      ReverseMapper *reverseMapper);
void paintValueTracker(PaintContext *ctx, const ValueTrackerAttributes &vt, const QPointF &at);
void paintElements(AbstractDiagram::Private *diagramPrivate, PaintContext *ctx,
                   const LabelPaintCache &lpc, const LineAttributesInfoList &lineList);
void paintAreas(AbstractDiagram::Private *diagramPrivate, PaintContext *ctx, const QModelIndex &index,
                const QList<QPolygonF> &areas, uint opacity);

void paintSpline(PaintContext *ctx, const QBrush &brush, const QPen &pen, const QPolygonF &points);
void paintAreas(AbstractDiagram::Private *diagramPrivate, PaintContext *ctx, const QModelIndex &index,
                const QList<QPainterPath> &areas, uint opacity);
}

inline qreal euclideanLength(const QPointF &p)
{
    return sqrt(p.x() * p.x() + p.y() * p.y());
}

enum SplineNodePosition
{
    LeftSplineNodePosition,
    RightSplineNodePosition
};

inline QPointF splineNode(qreal tension, QPointF before, QPointF current, QPointF after, SplineNodePosition position)
{
    if (ISNAN(before.y()) || ISNAN(after.y()))
        return current;

    const QPointF diff = after - before;
    const qreal scale = 0.25 * tension * diff.x() / euclideanLength(diff);
    return current + diff * scale * (position == LeftSplineNodePosition ? 1 : -1);
}

enum SplineDirection
{
    NormalSplineDirection,
    ReverseSplineDirection
};

inline QPair<QPointF, QPointF> splineChunk(qreal tension, QPointF before, QPointF a, QPointF b, QPointF after,
                                           SplineDirection splineDirection)
{
    QPointF nodeLeft = a;
    QPointF nodeRight = b;

    if (!ISNAN(before.y())) {
        nodeLeft = splineNode(tension, before, a, b,
                              splineDirection == NormalSplineDirection ? LeftSplineNodePosition : RightSplineNodePosition);
    }

    if (!ISNAN(after.y())) {
        nodeRight = splineNode(tension, a, b, after,
                               splineDirection != NormalSplineDirection ? LeftSplineNodePosition : RightSplineNodePosition);
    }

    return qMakePair(nodeLeft, nodeRight);
}

inline void addSplineChunkTo(QPainterPath &path, qreal tension, QPointF before, QPointF a, QPointF b, QPointF after,
                             SplineDirection splineDirection)
{
    const QPair<QPointF, QPointF> chunk = splineChunk(tension, before, a, b, after, splineDirection);
    path.cubicTo(chunk.first, chunk.second, b);
}
}

#endif
