#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartStackedBarDiagram_p.h"

using namespace KDChart;

StackedBarDiagram::StackedBarDiagram( BarDiagram* d )
    : BarDiagramType( d )
{
}

BarDiagram::BarType StackedBarDiagram::type() const
{
    return BarDiagram::Stacked;
}

const QPair<QPointF, QPointF> StackedBarDiagram::calculateDataBoundaries() const
{
    const int rowCount = attributesModel()->rowCount(attributesModelRootIndex());
    const int colCount = attributesModel()->columnCount(attributesModelRootIndex());

    double xMin = 0;
    double xMax = rowCount;
    double yMin = 0, yMax = 0;
    bool bStarting = true;
    for ( int j=0; j< rowCount; ++j ) {
        // calculate sum of values per column - Find out stacked Min/Max
        double stackedValues = 0;
        for ( int i=0; i<colCount ; ++i ) {
            QModelIndex idx = diagram()->model()->index( j, i, diagram()->rootIndex() );
            stackedValues +=  diagram()->model()->data( idx ).toDouble();
            // this is always true yMin can be 0 in case all values
            // are the same
            // same for yMax it can be zero if all values are negative
            if( bStarting ){
                yMin = stackedValues;
                yMax = stackedValues;
                bStarting = false;
            }else{
                yMin = qMin( yMin, stackedValues );
                yMax = qMax( yMax, stackedValues );
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

void StackedBarDiagram::paint(  PaintContext* ctx )
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
    double groupWidth = width/ (rowCount + 2);
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

    if ( ba.useFixedValueBlockGap() )
        spaceBetweenGroups += ba.fixedValueBlockGap();

    calculateValueAndGapWidths( rowCount, colCount,groupWidth,
                                barWidth, spaceBetweenBars, spaceBetweenGroups );

    DataValueTextInfoList list;
    for ( int i = 0; i<colCount; ++i ) {
        double offset = spaceBetweenGroups;
        for ( int j = 0; j< rowCount; ++j ) {
            QModelIndex index = diagram()->model()->index( j, i, diagram()->rootIndex() );
            ThreeDBarAttributes threeDAttrs = diagram()->threeDBarAttributes( index );
            double value = 0, stackedValues = 0;
            QPointF point, previousPoint;

            if ( threeDAttrs.isEnabled() ) {
                if ( barWidth > 0 )
                    barWidth =  (width - ((offset+(threeDAttrs.depth()))*rowCount))/ rowCount;
                if ( barWidth <= 0 ) {
                    barWidth = 0;
                    maxDepth = offset - (width/rowCount);
                }
            } else
                barWidth =  (ctx->rectangle().width() - (offset*rowCount))/ rowCount ;

            value = diagram()->model()->data( index ).toDouble();
            for ( int k = i; k >= 0 ; --k )
                stackedValues += diagram()->model()->data( diagram()->model()->index( j, k, diagram()->rootIndex() ) ).toDouble();
            point = ctx->coordinatePlane()->translate( QPointF( j, stackedValues ) );
            point.setX( point.x() + offset/2 );
            previousPoint = ctx->coordinatePlane()->translate( QPointF( j, stackedValues - value ) );
            const double barHeight = previousPoint.y() - point.y();

            const QRectF rect( point, QSizeF( barWidth , barHeight ) );
            appendDataValueTextInfoToList( diagram(), list, index, PositionPoints( rect ),
                                              Position::NorthWest, Position::SouthEast,
                                              value );
            paintBars( ctx, index, rect, maxDepth );
        }

    }
    paintDataValueTextsAndMarkers(  diagram(),  ctx,  list,  false );
}


