#ifndef PAINTINGHELPERS_P_H
#define PAINTINGHELPERS_P_H

#include "KDChartAbstractDiagram_p.h"

#include <QVector>

class QBrush;
class QModelIndex;
class QPen;
class QPointF;
class QPolygonF;

namespace KDChart {

class LineAttributesInfo;
typedef QVector<LineAttributesInfo> LineAttributesInfoList;
class ThreeDLineAttributes;
class ValueTrackerAttributes;

namespace PaintingHelpers {

const QPointF project( const QPointF& point, const ThreeDLineAttributes& tdAttributes );
void paintPolyline( PaintContext* ctx, const QBrush& brush, const QPen& pen, const QPolygonF& points );
void paintThreeDLines( PaintContext* ctx, AbstractDiagram *diagram, const QModelIndex& index,
                       const QPointF& from, const QPointF& to, const ThreeDLineAttributes& tdAttributes,
                       ReverseMapper* reverseMapper );
void paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt, const QPointF& at );
void paintElements( AbstractDiagram::Private *diagramPrivate, PaintContext* ctx,
                    const LabelPaintCache& lpc, const LineAttributesInfoList& lineList );

}
}

#endif