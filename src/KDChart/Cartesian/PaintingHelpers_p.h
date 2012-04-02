#ifndef PAINTINGHELPERS_P_H
#define PAINTINGHELPERS_P_H

class QBrush;
class QPen;
class QPointF;
class QPolygonF;

namespace KDChart {

class PaintContext;
class ValueTrackerAttributes;

namespace PaintingHelpers {

void paintPolyline( PaintContext* ctx, const QBrush& brush, const QPen& pen, const QPolygonF& points );

void paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt, const QPointF& at );

}
}

#endif