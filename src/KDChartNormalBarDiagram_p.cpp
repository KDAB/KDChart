#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartNormalBarDiagram_p.h"

using namespace KDChart;

NormalBarDiagram::NormalBarDiagram( BarDiagram* d )
    : BarDiagramType( d )
{
}

BarDiagram::BarType NormalBarDiagram::type() const
{
    return BarDiagram::Normal;
}

const QPair<QPointF, QPointF> NormalBarDiagram::calculateDataBoundaries() const
{
    const int rowCount = attributesModel()->rowCount(attributesModelRootIndex());
    const int colCount = attributesModel()->columnCount(attributesModelRootIndex());

    double xMin = 0;
    double xMax = rowCount;
    double yMin = 0, yMax = 0;
    bool bStarting = true;
    for ( int i=0; i<colCount; ++i ) {
        for ( int j=0; j< rowCount; ++j ) {
            const double value = attributesModel()->data( attributesModel()->index( j, i, attributesModelRootIndex() ) ).toDouble();
            // this is always true yMin can be 0 in case all values
            // are the same
            // same for yMax it can be zero if all values are negative
            if( bStarting ){
                yMin = value;
                yMax = value;
                bStarting = false;
            }else{
                yMin = qMin( yMin, value );
                yMax = qMax( yMax, value );
            }
        }
    }

    // special cases
    if (  yMax == yMin ) {
        if ( yMin == 0.0 )
            yMax = 0.1; //we need at list a range
        else
            yMax = 0.0; // they are the same but negative
    }
    QPointF bottomLeft ( QPointF( xMin, yMin ) );
    QPointF topRight ( QPointF( xMax, yMax ) );

    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}

void NormalBarDiagram::paint(  PaintContext* ctx )
{

    const QPair<QPointF,QPointF> boundaries = diagram()->dataBoundaries(); // cached

    const QPointF boundLeft = ctx->coordinatePlane()->translate( boundaries.first ) ;
    const QPointF boundRight = ctx->coordinatePlane()->translate( boundaries.second );

    const int rowCount = attributesModel()->rowCount(attributesModelRootIndex());
    const int colCount = attributesModel()->columnCount(attributesModelRootIndex());

    BarAttributes ba = diagram()->barAttributes( diagram()->model()->index( 0, 0, diagram()->rootIndex() ) );
    double barWidth = 0;
    double maxDepth = 0;
    double width = boundRight.x() - boundLeft.x();
    double groupWidth = width / (rowCount + 2);
    double spaceBetweenBars = 0;
    double spaceBetweenGroups = 0;

    if ( ba.useFixedBarWidth() ) {

        barWidth = ba.fixedBarWidth();
        groupWidth += barWidth;

        // Pending Michel set a min and max value for the groupWidth
        // related to the area.width
        if ( groupWidth < 0 )
            groupWidth = 0;

        if ( groupWidth  * rowCount > ctx->rectangle().width() )
            groupWidth = ctx->rectangle().width() / rowCount;
    }

    // maxLimit: allow the space between bars to be larger until area.width()
    // is covered by the groups.
    double maxLimit = rowCount * (groupWidth + ((colCount-1) * ba.fixedDataValueGap()) );

    //Pending Michel: FixMe
    if ( ba.useFixedDataValueGap() ) {
        if ( ctx->rectangle().width() > maxLimit )
            spaceBetweenBars += ba.fixedDataValueGap();
        else
            spaceBetweenBars = ((ctx->rectangle().width()/rowCount) - groupWidth)/(colCount-1);
    }

    if ( ba.useFixedValueBlockGap() ) {
        spaceBetweenGroups += ba.fixedValueBlockGap();
    }

    calculateValueAndGapWidths( rowCount, colCount,groupWidth,
                                barWidth, spaceBetweenBars, spaceBetweenGroups );

    DataValueTextInfoList list;

    for ( int i=0; i<rowCount; ++i ) {
        double offset = -groupWidth/2 + spaceBetweenGroups/2;

        if ( ba.useFixedDataValueGap() ) {
            if ( spaceBetweenBars > 0 ) {
                if ( ctx->rectangle().width() > maxLimit )
                    offset -= ba.fixedDataValueGap();
                else
                    offset -= ((ctx->rectangle().width()/rowCount) - groupWidth)/(colCount-1);

            } else {
                offset += barWidth/2;
            }
        }

        for ( int j=0; j< colCount; ++j ) {
            // paint one group
            const qreal value = attributesModel()->data( attributesModel()->index( i, j, attributesModelRootIndex() ) ).toDouble();
            QPointF topPoint = ctx->coordinatePlane()->translate( QPointF( i + 0.5, value ) );
            QPointF bottomPoint =  ctx->coordinatePlane()->translate( QPointF( i, 0 ) );
            const double barHeight = bottomPoint.y() - topPoint.y();
            topPoint.setX( topPoint.x() + offset );
            const QModelIndex index = diagram()->model()->index( i, j, diagram()->rootIndex() );
            const QRectF rect( topPoint, QSizeF( barWidth, barHeight ) );
            appendDataValueTextInfoToList( diagram(), list, index, PositionPoints( rect ),
                                           Position::NorthWest, Position::SouthEast,
                                           value );
            paintBars( ctx, index, rect, maxDepth );

            offset += barWidth + spaceBetweenBars;
        }
    }
    paintDataValueTextsAndMarkers(  diagram(),  ctx,  list,  false );
}
