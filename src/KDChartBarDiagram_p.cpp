#include "KDChartBarDiagram.h"
#include "KDChartDataValueAttributes.h"

#include "KDChartBarDiagram_p.h"

using namespace KDChart;

BarDiagram::Private::Private( const Private& rhs )
    : AbstractCartesianDiagram::Private( rhs )
{
}


void BarDiagram::BarDiagramType::paintBars( PaintContext* ctx, const QModelIndex& index, const QRectF& bar, double& maxDepth )
{
    QRectF isoRect;
    QPolygonF topPoints, sidePoints;
    ThreeDBarAttributes threeDAttrs = diagram()->threeDBarAttributes( index );
    double usedDepth;

    //Pending Michel: configure threeDBrush settings - shadowColor etc...
    QBrush indexBrush ( diagram()->brush( index ) );
    QPen indexPen( diagram()->pen( index ) );
    PainterSaver painterSaver( ctx->painter() );
    if ( diagram()->antiAliasing() )
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    ctx->painter()->setBrush( indexBrush );
    ctx->painter()->setPen( indexPen );
    if ( threeDAttrs.isEnabled() ) {
        bool stackedMode = false;
        bool percentMode = false;
        bool paintTop = true;
        if ( maxDepth )
            threeDAttrs.setDepth( -maxDepth );
        QPointF boundRight =  ctx->coordinatePlane()->translate( diagram()->dataBoundaries().second );
        //fixme adjust the painting to reasonable depth value
        switch ( type() )
        {
        case BarDiagram::Normal:
            usedDepth = threeDAttrs.depth()/4;
            stackedMode = false;
            percentMode = false;
            break;
        case BarDiagram::Stacked:
            usedDepth = threeDAttrs.depth();
            stackedMode = true;
            percentMode = false;
            break;
        case BarDiagram::Percent:
            usedDepth = threeDAttrs.depth();
            stackedMode = false;
            percentMode = true;
            break;
        default:
            Q_ASSERT_X ( false, "dataBoundaries()",
                         "Type item does not match a defined bar chart Type." );
        }
        isoRect =  bar.translated( usedDepth, -usedDepth );
        // we need to find out if the height is negative
        // and in this case paint it up and down
        //qDebug() << isoRect.height();
        if (  isoRect.height() < 0 ) {
          topPoints << isoRect.bottomLeft() << isoRect.bottomRight()
                    << bar.bottomRight() << bar.bottomLeft();
          if ( stackedMode ) {
              // fix it when several negative stacked values
              if (  index.column() == 0 ) {
                  paintTop = true;
              }
              else
                  paintTop = false;
          }

        } else {
            ctx->painter()->drawRect( isoRect );
            topPoints << bar.topLeft() << bar.topRight() << isoRect.topRight() << isoRect.topLeft();
        }

        if ( percentMode && isoRect.height() == 0 )
            paintTop = false;

        bool needToSetClippingOffForTop = false;
        if ( paintTop ){
            // Draw the top, if at least one of the top's points is
            // either inside or near at the edge of the coordinate plane:
            bool drawIt = false;
            bool hasPointOutside = false;
            const QRectF r( ctx->rectangle().adjusted(0,-1,1,0) );
            KDAB_FOREACH( QPointF pt, topPoints ) {
                if( r.contains( pt ) )
                    drawIt = true;
                else
                    hasPointOutside = true;
            }
            if( drawIt ){
                needToSetClippingOffForTop = hasPointOutside && ctx->painter()->hasClipping();
                if( needToSetClippingOffForTop )
                    ctx->painter()->setClipping( false );
                ctx->painter()->drawPolygon( topPoints );
                if( needToSetClippingOffForTop )
                    ctx->painter()->setClipping( true );
            }
        }



        sidePoints << bar.topRight() << isoRect.topRight() << isoRect.bottomRight() << bar.bottomRight();
        if (  bar.height() != 0 ){
            if( needToSetClippingOffForTop )
                ctx->painter()->setClipping( false );
            ctx->painter()->drawPolygon( sidePoints );
            if( needToSetClippingOffForTop )
                ctx->painter()->setClipping( true );
        }
    }

    if (  bar.height() != 0 )
        ctx->painter()->drawRect( bar );
    // reset
    //diagram()->maxDepth = threeDAttrs.depth();
}


AttributesModel* BarDiagram::BarDiagramType::attributesModel() const
{
    return m_private->attributesModel;
}

QModelIndex BarDiagram::BarDiagramType::attributesModelRootIndex() const
{
    return m_private->diagram->attributesModelRootIndex();
}


BarDiagram* BarDiagram::BarDiagramType::diagram() const
{
    return m_private->diagram;
}



void BarDiagram::BarDiagramType::appendDataValueTextInfoToList(
            AbstractDiagram * diagram,
            DataValueTextInfoList & list,
            const QModelIndex & index,
            const PositionPoints& points,
            const Position& autoPositionPositive,
            const Position& autoPositionNegative,
            const qreal value )
{
    m_private->appendDataValueTextInfoToList( diagram, list, index, points,
                                              autoPositionPositive, autoPositionPositive, value );
}

void BarDiagram::BarDiagramType::paintDataValueTextsAndMarkers(
    AbstractDiagram* diagram,
    PaintContext* ctx,
    const DataValueTextInfoList & list,
    bool paintMarkers )
{
    m_private->paintDataValueTextsAndMarkers( diagram, ctx, list, paintMarkers );
}


void BarDiagram::BarDiagramType::calculateValueAndGapWidths( int rowCount,int colCount,
                                             double groupWidth,
                                             double& outBarWidth,
                                             double& outSpaceBetweenBars,
                                             double& outSpaceBetweenGroups )
{

    Q_UNUSED( rowCount );

    BarAttributes ba = diagram()->barAttributes( diagram()->model()->index( 0, 0, diagram()->rootIndex() ) );

    // Pending Michel Fixme
    /* We are colCount groups to paint. Each group is centered around the
     * horizontal point position on the grid. The full area covers the
     * values -1 to colCount + 1. A bar has a relative width of one unit,
     * the gaps between bars are 0.5 wide, and the gap between groups is
     * also one unit, by default. */

    const double units = colCount // number of bars in group * 1.0
        + (colCount-1) * ba.barGapFactor() // number of bar gaps
        + 1 * ba.groupGapFactor(); // number of group gaps

    double unitWidth = groupWidth / units;
    outBarWidth = unitWidth;
    outSpaceBetweenBars += unitWidth * ba.barGapFactor();
    // Pending Michel - minLimit: allow space between bars to be reduced until the bars are displayed next to each other.
    // is that what we want?
    if ( outSpaceBetweenBars < 0 )
        outSpaceBetweenBars = 0;
    outSpaceBetweenGroups += unitWidth * ba.groupGapFactor();

}
