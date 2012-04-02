#ifndef PAINTINGHELPERS_P_H
#define PAINTINGHELPERS_P_H

class QPointF;

namespace KDChart {

class PaintContext;
class ValueTrackerAttributes;

namespace PaintingHelpers {

void paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt, const QPointF& at );

}
}

#endif