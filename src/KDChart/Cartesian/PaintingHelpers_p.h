/****************************************************************************
** Copyright (C) 2001-2020 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

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

const QPointF project( const QPointF& point, const ThreeDLineAttributes& tdAttributes );
void paintPolyline( PaintContext* ctx, const QBrush& brush, const QPen& pen, const QPolygonF& points );
void paintThreeDLines( PaintContext* ctx, AbstractDiagram *diagram, const QModelIndex& index,
                       const QPointF& from, const QPointF& to, const ThreeDLineAttributes& tdAttributes,
                       ReverseMapper* reverseMapper );
void paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt, const QPointF& at );
void paintElements( AbstractDiagram::Private *diagramPrivate, PaintContext* ctx,
                    const LabelPaintCache& lpc, const LineAttributesInfoList& lineList );
void paintAreas( AbstractDiagram::Private* diagramPrivate, PaintContext* ctx, const QModelIndex& index,
                 const QList< QPolygonF >& areas, uint opacity );

void paintSpline( PaintContext* ctx, const QBrush& brush, const QPen& pen, const QPolygonF& points );
void paintAreas( AbstractDiagram::Private* diagramPrivate, PaintContext* ctx, const QModelIndex& index,
                 const QList< QPainterPath >& areas, uint opacity );

}

inline qreal euclideanLength( const QPointF &p )
{
    return sqrt( p.x() * p.x() + p.y() * p.y() );
}

enum SplineNodePosition { LeftSplineNodePosition, RightSplineNodePosition };

inline QPointF splineNode( qreal tension, QPointF before, QPointF current, QPointF after, SplineNodePosition position )
{
    if ( ISNAN( before.y() ) || ISNAN ( after.y() ) )
        return current;

    const QPointF diff = after - before;
    const qreal scale = 0.25 * tension * diff.x() / euclideanLength( diff );
    return current + diff * scale * ( position == LeftSplineNodePosition ? 1 : -1 );
}

enum SplineDirection { NormalSplineDirection, ReverseSplineDirection };

inline QPair<QPointF, QPointF> splineChunk( qreal tension, QPointF before, QPointF a, QPointF b, QPointF after,
                                            SplineDirection direction = NormalSplineDirection )
{
    QPointF nodeLeft = a;
    QPointF nodeRight = b;

    if ( !ISNAN( before.y() ) ) {
        nodeLeft = splineNode( tension, before, a, b,
                direction == NormalSplineDirection ? LeftSplineNodePosition : RightSplineNodePosition );
    }

    if ( !ISNAN( after.y() ) ) {
        nodeRight = splineNode( tension, a, b, after,
                direction != NormalSplineDirection ? LeftSplineNodePosition : RightSplineNodePosition );
    }

    return qMakePair( nodeLeft, nodeRight );
}

inline void addSplineChunkTo( QPainterPath& path, qreal tension, QPointF before, QPointF a, QPointF b, QPointF after,
                              SplineDirection direction = NormalSplineDirection )
{
    const QPair<QPointF, QPointF> chunk = splineChunk( tension, before, a, b, after, direction );
    path.cubicTo( chunk.first, chunk.second, b );
}

}

#endif
