#include "PaintingHelpers_p.h"

#include "KDChartGlobal.h"

#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartValueTrackerAttributes.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartPrintingParameters.h"


namespace KDChart {
namespace PaintingHelpers {

void paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt, const QPointF& at )
{
    CartesianCoordinatePlane* plane = qobject_cast<CartesianCoordinatePlane*>( ctx->coordinatePlane() );
    if( !plane )
        return;

    DataDimensionsList gridDimensions = ctx->coordinatePlane()->gridDimensionsList();
    const QPointF bottomLeft( ctx->coordinatePlane()->translate(
                              QPointF( plane->isHorizontalRangeReversed() ?
                                           gridDimensions.at( 0 ).end :
                                           gridDimensions.at( 0 ).start,
                                       plane->isVerticalRangeReversed() ?
                                           gridDimensions.at( 1 ).end :
                                           gridDimensions.at( 1 ).start ) ) );
    const QPointF topRight( ctx->coordinatePlane()->translate(
                            QPointF( plane->isHorizontalRangeReversed() ?
                                         gridDimensions.at( 0 ).start :
                                         gridDimensions.at( 0 ).end,
                                     plane->isVerticalRangeReversed() ?
                                         gridDimensions.at( 1 ).start :
                                         gridDimensions.at( 1 ).end ) ) );
    const QPointF markerPoint = at;

    QPointF startPoint;
    if ( vt.orientations() & Qt::Horizontal ) {
        startPoint = QPointF( bottomLeft.x(), at.y() );
    } else {
        startPoint = QPointF( at.x(), topRight.y() );
    }

    QPointF endPoint;
    if ( vt.orientations() & Qt::Vertical ) {
        endPoint = QPointF( at.x(), bottomLeft.y() );
    } else {
        endPoint = QPointF( topRight.x(), at.y() );
    }

    const QSizeF markerSize = vt.markerSize();
    const QRectF ellipseMarker = QRectF( at.x() - markerSize.width() / 2,
                                         at.y() - markerSize.height() / 2,
                                         markerSize.width(), markerSize.height() );

    QPointF startMarker[3];
    if ( vt.orientations() & Qt::Horizontal ) {
        startMarker[0] = startPoint + QPointF( 0,  markerSize.height() / 2 );
        startMarker[1] = startPoint + QPointF( markerSize.width() / 2, 0 );
        startMarker[2] = startPoint - QPointF( 0, markerSize.height() / 2 );
    } else {
        startMarker[0] = startPoint + QPointF( 0, markerSize.height() / 2 );
        startMarker[1] = startPoint + QPointF( markerSize.width() / 2, 0 );
        startMarker[2] = startPoint - QPointF( markerSize.width() / 2, 0 );
    }

    QPointF endMarker[3];

    if ( vt.orientations() & Qt::Vertical ) {
        endMarker[0] = endPoint + QPointF( markerSize.width() / 2, 0 );
        endMarker[1] = endPoint - QPointF( 0, markerSize.height() / 2 );
        endMarker[2] = endPoint - QPointF( markerSize.width() / 2, 0 );
    } else {
        endMarker[0] = endPoint + QPointF( 0,  markerSize.width() / 2 );
        endMarker[1] = endPoint - QPointF( 0, markerSize.height() / 2 );
        endMarker[2] = endPoint - QPointF( markerSize.width() / 2, 0 );
    }

    QPointF topLeft = startPoint;
    QPointF bottomRightOffset = endPoint - topLeft;
    QSizeF size( bottomRightOffset.x(), bottomRightOffset.y() );
    QRectF area( topLeft, size );

    PainterSaver painterSaver( ctx->painter() );
    ctx->painter()->setPen( PrintingParameters::scalePen( vt.linePen() ) );
    ctx->painter()->setBrush( QBrush() );
    ctx->painter()->drawLine( markerPoint, startPoint );
    ctx->painter()->drawLine( markerPoint, endPoint );

    ctx->painter()->fillRect( area, vt.areaBrush() );

    ctx->painter()->setPen( PrintingParameters::scalePen( vt.markerPen() ) );
    ctx->painter()->setBrush( vt.markerBrush() );
    ctx->painter()->drawEllipse( ellipseMarker );

    ctx->painter()->setPen( PrintingParameters::scalePen( vt.arrowBrush().color() ) );
    ctx->painter()->setBrush( vt.arrowBrush() );
    ctx->painter()->drawPolygon( startMarker, 3 );
    ctx->painter()->drawPolygon( endMarker, 3 );
}

} // namespace PaintingHelpers
} // namespace KDChart