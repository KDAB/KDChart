#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartPercentBarDiagram_p.h"

using namespace KDChart;

PercentBarDiagram::PercentBarDiagram( BarDiagram* d )
    : BarDiagramType( d )
{
}

BarDiagram::BarType PercentBarDiagram::type() const
{
    return BarDiagram::Percent;
}

const QPair<QPointF, QPointF> PercentBarDiagram::calculateDataBoundaries() const
{
    const int rowCount = attributesModel()->rowCount(attributesModelRootIndex());
    const int colCount = attributesModel()->columnCount(attributesModelRootIndex());

    double xMin = 0;
    double xMax = rowCount;
    double yMin = 0, yMax = 0;
    for ( int i=0; i<colCount; ++i ) {
        for ( int j=0; j< rowCount; ++j ) {
            // Ordinate should begin at 0 the max value being the 100% pos
            QModelIndex idx = diagram()->model()->index( j, i, diagram()->rootIndex() );
            // only positive values are handled
            double value = diagram()->model()->data( idx ).toDouble();
            if ( value > 0 )
                yMax = qMax( yMax, value );
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

    //qDebug() << "BarDiagram::calculateDataBoundaries () returns ( " << bottomLeft << topRight <<")";
    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}

void PercentBarDiagram::paint(  PaintContext* ctx )
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
    double maxValue = 100; // always 100 %
    double sumValues = 0;
    QVector <double > sumValuesVector;

    //calculate sum of values for each column and store
    for ( int j=0; j<rowCount; ++j ) {
        for ( int i=0; i<colCount; ++i ) {
            double tmpValue = diagram()->model()->data( diagram()->model()->index( j, i, diagram()->rootIndex() ) ).toDouble();
            if ( tmpValue > 0 )
                sumValues += tmpValue;
            if ( i == colCount-1 ) {
                sumValuesVector <<  sumValues ;
                sumValues = 0;
            }
        }
    }

    // calculate stacked percent value
    for ( int i = 0; i<colCount; ++i ) {
        double offset = spaceBetweenGroups;
        for ( int j=0; j<rowCount ; ++j ) {
            double value = 0, stackedValues = 0;
            QPointF point, previousPoint;
            QModelIndex index = diagram()->model()->index( j, i, diagram()->rootIndex() );
            ThreeDBarAttributes threeDAttrs = diagram()->threeDBarAttributes( index );

            if ( threeDAttrs.isEnabled() ){
                if ( barWidth > 0 )
                    barWidth =  (width - ((offset+(threeDAttrs.depth()))*rowCount))/ rowCount;
                if ( barWidth <= 0 ) {
                    barWidth = 0;
                    maxDepth = offset - ( width/rowCount);
                }
            }else{
                barWidth = (ctx->rectangle().width() - (offset*rowCount))/ rowCount;
            }

            value = diagram()->model()->data( index ).toDouble();

            // calculate stacked percent value
            // we only take in account positives values for now.
            for ( int k = i; k >= 0 ; --k ) {
                double val = diagram()->model()->data( diagram()->model()->index( j, k, diagram()->rootIndex() ) ).toDouble();
                if ( val > 0)
                    stackedValues += val;
            }

            if (  sumValuesVector.at( j ) != 0 && value > 0 ) {
                point = ctx->coordinatePlane()->translate( QPointF( j,  stackedValues/sumValuesVector.at(j)* maxValue ) );

                point.setX( point.x() + offset/2 );

                previousPoint = ctx->coordinatePlane()->translate( QPointF( j, (stackedValues - value)/sumValuesVector.at(j)* maxValue ) );
            }
            const double barHeight = previousPoint.y() - point.y();

            const QRectF rect( point, QSizeF( barWidth, barHeight ) );
            appendDataValueTextInfoToList( diagram(), list, index, PositionPoints( rect ),
                                              Position::NorthWest, Position::SouthEast,
                                              value );
            paintBars( ctx, index, rect, maxDepth );

        }
    }
    paintDataValueTextsAndMarkers(  diagram(),  ctx,  list,  false );
}


