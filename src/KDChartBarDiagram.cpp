 #include <QtXml/QDomDocumentFragment>
#include <QPainter>
#include <QDebug>

#include "KDChartBarDiagram.h"
#include "KDChartBarDiagram_p.h"
#include "KDChartThreeDBarAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"

using namespace KDChart;

BarDiagram::Private::Private()
{
}

BarDiagram::Private::~Private() {}

#define d d_func()


BarDiagram::BarDiagram( CartesianCoordinatePlane* parent ) :
    AbstractCartesianDiagram( new Private(), parent )
{
    init();
}

void BarDiagram::init()
{
}

BarDiagram::~BarDiagram()
{
}

BarDiagram * BarDiagram::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: BarDiagram * BarDiagram::clone() const" );
    return (BarDiagram*)0xdeadbeef;
}

void BarDiagram::setBarAttributes( const BarAttributes & ta )
{
    d->attributesModel->setModelData( QVariant::fromValue( ta ), BarAttributesRole );
}

void BarDiagram::setBarAttributes( int column, const BarAttributes & ta )
{
    d->attributesModel->setHeaderData( column, Qt::Vertical, QVariant::fromValue( ta ), BarAttributesRole );
}

void BarDiagram::setBarAttributes( const QModelIndex & index, const BarAttributes & ta )
{
   model()->setData( index, QVariant::fromValue( ta ), BarAttributesRole );
}

BarAttributes BarDiagram::barAttributes( const QModelIndex & index ) const
{
    return model()->data( index, KDChart::BarAttributesRole ).value<BarAttributes>();
}

void BarDiagram::setThreeDBarAttributes( const ThreeDBarAttributes & tda )
{
    d->attributesModel->setModelData( QVariant::fromValue( tda ), ThreeDBarAttributesRole );
}

void BarDiagram::setThreeDBarAttributes( int column, const ThreeDBarAttributes & tda )
{
    d->attributesModel->setHeaderData( column, Qt::Vertical, QVariant::fromValue( tda ), ThreeDBarAttributesRole );
}

void BarDiagram::setThreeDBarAttributes( const QModelIndex & index, const ThreeDBarAttributes & tda )
{
   model()->setData( index, QVariant::fromValue( tda ), ThreeDBarAttributesRole );
}

ThreeDBarAttributes BarDiagram::threeDBarAttributes( const QModelIndex & index ) const
{
    return model()->data( index, KDChart::ThreeDBarAttributesRole ).value<ThreeDBarAttributes>();
}

void BarDiagram::resizeEvent ( QResizeEvent*)
{
}

const QPair<QPointF, QPointF> BarDiagram::dataBoundaries () const
{
    if ( !checkInvariants() ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());

    double xMin = 0;
    double xMax = rowCount;
    double yMin = 0, yMax = 0;
    BarAttributes ba = barAttributes( model()->index( 0, 0, rootIndex() ) );
    BarAttributes::BarType barType = ba.type();
    ThreeDBarAttributes tda;

     // calculate boundaries for  different line types Normal - Stacked - Percent - Default Normal
    switch ( barType )
        {

        case BarAttributes::Normal:
            for ( int i=0; i<colCount; ++i ) {
                for ( int j=0; j< rowCount; ++j ) {
                    double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    yMin = qMin( yMin, value );
                    yMax = qMax( yMax, value );

                    tda = threeDBarAttributes( model()->index( j, i, rootIndex() ) );
                }
            }
            break;
        case BarAttributes::Stacked:
            for ( int j=0; j< rowCount; ++j ) {
                // calculate sum of values per column - Find out stacked Min/Max
                double stackedValues = 0;
                for ( int i=0; i<colCount ; ++i ) {
                    stackedValues +=  model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    yMin = qMin( yMin, stackedValues);
                    yMax = qMax( yMax, stackedValues);
                    tda = threeDBarAttributes( model()->index( j, i, rootIndex() ) );
                }
            }
            break;
        case BarAttributes::Percent:
            for ( int i=0; i<colCount; ++i ) {
                for ( int j=0; j< rowCount; ++j ) {
                    // Ordinate should begin at 0 the max value being the 100% pos
                    double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    yMax = qMax( yMax, value );
                    tda = threeDBarAttributes( model()->index( j, i, rootIndex() ) );
                }
            }
            break;

         default:
             Q_ASSERT_X ( false, "dataBoundaries()",
                         "Type item does not match a defined bar chart Type." );
        }

    QPointF bottomLeft ( QPointF( xMin, yMin ) );
    QPointF topRight ( QPointF( xMax, yMax ) );
       if ( tda.isEnabled() ) {
	 //threeDBoundaries calculate a depth percent value and add it
	 QPointF pTRTranslated = coordinatePlane()->translate( topRight );
         QPointF pBLTranslated = coordinatePlane()->translate( bottomLeft );
	 //reserve some plane space for the top of the threeD bars
         //Pending Michel: why 4?
         double percentx = ((tda.depth()*4)/ pTRTranslated.x());
         double percenty = ((tda.depth()*4)/ pBLTranslated.y());
         topRight.setX( topRight.x() + percentx);
         topRight.setY( topRight.y() + percenty);
       }

    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}

void BarDiagram::paintEvent ( QPaintEvent*)
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

void BarDiagram::calculateValueAndGapWidths( int rowCount,int colCount,
                                             double groupWidth,
                                             double& outBarWidth,
                                             double& outSpaceBetweenBars,
                                             double& outSpaceBetweenGroups )
{
     BarAttributes ba = barAttributes( model()->index( 0, 0, rootIndex() ) );

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

void BarDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants() ) return;

    // first draw the grid
    drawGrid( ctx );
    
    //calculates and stores the values
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());
    DataValueTextInfoList list;
    BarAttributes ba = barAttributes( model()->index( 0, 0, rootIndex() ) );
    BarAttributes::BarType barType = ba.type();
    double maxValue = 0;
    double sumValues = 0;
    QVector <double > sumValuesVector;
    double barWidth = 0;
    double spaceBetweenBars = 0;
    double spaceBetweenGroups = 0;
    double groupWidth =  ctx->rectangle().width() / ( rowCount + 2 );

    if ( ba.useFixedBarWidth() ) {
        barWidth = ba.fixedBarWidth();
        groupWidth += barWidth;

        // Pending Michel set a min and max value for the groupWidth related to the area.width
        // FixMe
        if ( groupWidth < 0 )
            groupWidth = 0;

        if ( groupWidth  * rowCount > ctx->rectangle().width() )
            groupWidth = ctx->rectangle().width() / rowCount;
    }

    // maxLimit: allow the space between bars to be larger until area.width() is covered by the groups.
    double maxLimit = rowCount * (groupWidth + ((colCount-1) * ba.fixedDataValueGap()) );

    //Pending Michel: FixMe
    if ( ba.useFixedDataValueGap() ) {
        if ( ctx->rectangle().width() > maxLimit )
            spaceBetweenBars += ba.fixedDataValueGap();
        else
            spaceBetweenBars = ((ctx->rectangle().width()/rowCount) - groupWidth)/(colCount-1);
   }

    //Pending Michel: FixMe
    if ( ba.useFixedValueBlockGap() )
        spaceBetweenGroups += ba.fixedValueBlockGap();

    calculateValueAndGapWidths( rowCount, colCount,groupWidth,
                                barWidth, spaceBetweenBars, spaceBetweenGroups );

    // paint different bar types: Normal - Stacked - Percent
    switch ( barType )
        {
        case BarAttributes::Normal:
            // we paint the bars for all series next to each other, then move to the next value
            for ( int i=0; i<rowCount; ++i ) {
                double offset = -groupWidth/2 + spaceBetweenGroups/2;                
                // case fixed data value gap - handles max and min limits as well
                if ( ba.useFixedDataValueGap() ) {
                    if ( spaceBetweenBars > 0 ) {
		      if ( ctx->rectangle().width() > maxLimit )
                            offset -= ba.fixedDataValueGap();
		      else 
			offset -= ((ctx->rectangle().width()/rowCount) - groupWidth)/(colCount-1);
		      
                    } else {
                        //allow reducing the gap until the bars are displayed next to each other - null gap
                        offset += barWidth/2;                     
                    }
                }

                for ( int j=0; j< colCount; ++j ) {
                    // paint one group
                    QModelIndex index = model()->index( i, j, rootIndex() );
                    const double value = model()->data( index ).toDouble();
                    QPointF topPoint = coordinatePlane()->translate( QPointF( i + 0.5, value ) );
                    QPointF bottomPoint = coordinatePlane()->translate( QPointF( i, 0 ) );
                    const double barHeight = bottomPoint.y() - topPoint.y();
                    topPoint.setX( topPoint.x() + offset );
                    //PENDING Michel: FIXME barWidth
                    //barWidth =  ctx->rectangle().width() / (rowCount);
                    list.append( DataValueTextInfo( index, topPoint, value ) );
                    paintBars( ctx, index, QRectF( topPoint, QSizeF( barWidth, barHeight ) ) );
                     
                    offset += barWidth + spaceBetweenBars;
                }
            }
            break;
        case BarAttributes::Stacked:
            for ( int i = 0; i<colCount; ++i ) {                
	      double offset =  spaceBetweenBars;
                for ( int j = 0; j< rowCount; ++j ) {
                    QModelIndex index = model()->index( j, i, rootIndex() );
		    ThreeDBarAttributes tda = threeDBarAttributes( model()->index( j, i, rootIndex() ) );
                    double value = 0, stackedValues = 0;
                    QPointF point, previousPoint;
                    if ( tda.isEnabled() )
		      barWidth =  (ctx->rectangle().width() - ((offset+(tda.depth()/2))*rowCount))/ rowCount ;
		    else
		      barWidth =  (ctx->rectangle().width() - (offset*rowCount))/ rowCount ; 
                    value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    for ( int k = i; k >= 0 ; --k )
                        stackedValues += model()->data( model()->index( j, k, rootIndex() ) ).toDouble();
                    point = coordinatePlane()->translate( QPointF( j, stackedValues ) );
                    //adjust the bars position in the view
                    point.setX( point.x() + offset/2 );
                    previousPoint = coordinatePlane()->translate( QPointF( j, stackedValues - value ) );
                    const double barHeight = previousPoint.y() - point.y();
                    list.append( DataValueTextInfo( model()->index( j, i, rootIndex() ), point, value ) );
                    paintBars( ctx, index, QRectF( point, QSizeF( barWidth , barHeight ) ) );
                }
            }
            break;
        case BarAttributes::Percent:
            // search for ordinate max value or 100 %
            for ( int i=0; i<colCount; ++i ) {
                for ( int j=0; j< rowCount; ++j ) {
                    double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    maxValue = qMax( maxValue, value );
                }
            }
            //calculate sum of values for each column and store
            for ( int j=0; j<rowCount; ++j ) {
                for ( int i=0; i<colCount; ++i ) {
                    double tmpValue = model()->data( model()->index ( j, i, rootIndex() ) ).toDouble();
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
                double offset =  spaceBetweenBars;
                for ( int j=0; j<rowCount ; ++j ) {
                    double value = 0, stackedValues = 0;
                    QPointF point, previousPoint;
                    QModelIndex index = model()->index( j, i, rootIndex() );
		    ThreeDBarAttributes tda = threeDBarAttributes( model()->index( j, i, rootIndex() ) );
		    if ( tda.isEnabled() )
		      barWidth =  (ctx->rectangle().width() - ((offset+tda.depth())*rowCount))/ rowCount;
		    else 
		      barWidth = (ctx->rectangle().width() - (offset*rowCount))/ rowCount;

                    value = model()->data(  model()->index( j, i, rootIndex() ) ).toDouble();
                    //calculate stacked percent value- we only take in account positives values for now.
                    for ( int k = i; k >= 0 ; --k ) {
		        double val = model()->data( model()->index( j, k, rootIndex() ) ).toDouble();
                        if ( val > 0)
			    stackedValues += val;
		    }
                    point = coordinatePlane()->translate( QPointF( j,  stackedValues/sumValuesVector.at(j)* maxValue ) );
                    //adjust the bars position in the view
                    point.setX( point.x() + offset/2 );

                    previousPoint = coordinatePlane()->translate( QPointF( j, (stackedValues - value)/sumValuesVector.at(j)* maxValue ) );
                    const double barHeight = previousPoint.y() - point.y();

                    list.append( DataValueTextInfo( model()->index( j, i, rootIndex() ), point, value ) );
                    paintBars( ctx, index, QRectF( point, QSizeF( barWidth, barHeight ) ) );

                }
            }
            break;
        default:
            Q_ASSERT_X ( false, "paint()",
                         "Type item does not match a defined bar chart Type." );
        }

    DataValueTextInfoListIterator it( list );
    while ( it.hasNext() ) {
        const DataValueTextInfo& info = it.next();
        paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
    }
}

void BarDiagram::paintBars( PaintContext* ctx, const QModelIndex& index, const QRectF& bar )
{
    QRectF isoRect;
    QPolygonF topPoints, sidePoints;
    BarAttributes ba = barAttributes( index );
    BarAttributes::BarType barType = ba.type();
    ThreeDBarAttributes tda = threeDBarAttributes( index );
    double usedDepth;
    //Pending Michel: configure threeDBrush settings - shadowColor etc...
    QBrush indexBrush ( brush( index ) );
    QPen indexPen( pen( index ) );
    PainterSaver painterSaver( ctx->painter() );
    ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    ctx->painter()->setBrush( indexBrush );
    ctx->painter()->setPen( indexPen );
    if ( tda.isEnabled() ) {
      //fixme adjust the painting to reasonable depth value
      switch ( barType )
        {
        case BarAttributes::Normal:
	  usedDepth = tda.depth()/4;
	  break;
        case BarAttributes::Stacked:
          usedDepth = tda.depth()/2;
          break;
        case BarAttributes::Percent:
	  usedDepth = tda.depth();
          break;
	default:
	  Q_ASSERT_X ( false, "dataBoundaries()",
		       "Type item does not match a defined bar chart Type." );
        }
      isoRect = bar.translated( usedDepth, -usedDepth );
      ctx->painter()->drawRect( isoRect );
      topPoints << bar.topLeft() << bar.topRight() << isoRect.topRight() << isoRect.topLeft();
      ctx->painter()->drawPolygon( topPoints );
      sidePoints << bar.topRight() << isoRect.topRight() << isoRect.bottomRight() << bar.bottomRight();
      ctx->painter()->drawPolygon( sidePoints );
    }
   
    ctx->painter()->drawRect( bar );
}

void BarDiagram::resize ( const QSizeF& )
{
}

const int BarDiagram::numberOfAbscissaSegments () const
{
    return model()->rowCount(rootIndex());
}

const int BarDiagram::numberOfOrdinateSegments () const
{
    return model()->columnCount(rootIndex());
}

CartesianAxisList BarDiagram::axes () const
{
    return CartesianAxisList();
}

#undef d
