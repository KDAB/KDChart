/****************************************************************************
 ** Copyright (C) 2006 Klar�vdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <QtXml/QDomDocumentFragment>
#include <QPainter>
#include <QDebug>

#include "BarDiagram.h"
#include "BarDiagram_p.h"
#include "KDChartThreeDBarAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"

// temp:
#include "CoordinatePlane.h"

using namespace KDChart;

/* ModelDataCache provides information about the model that is valid as
   long as the model data or layout does not change. It helps us to not
   calculate values such as upper bounds for columns or row sums
   unnecessarily.

   ModelDataCache is supposed to be deleted and reconstructed when the
   model changes. It cannot be used standalone. It may be possible to
   generalize it, though.
*/
class KDChart::ModelDataCache : public QObject
{
    Q_OBJECT

public:
    explicit ModelDataCache ( Bar2Diagram* parent )
        : QObject ( parent )
        , m_cachedDataBoundariesValid ( false )
    {
        QAbstractItemModel* model = parent->model();
        if ( model )
        {
            connect ( model, SIGNAL( dataChanged ( const QModelIndex&, const QModelIndex&) ),
                      SLOT ( slotDataChanged ( const QModelIndex&, const QModelIndex& ) ) );
            connect ( model, SIGNAL( layoutChanged () ), SLOT ( slotLayoutChanged() ) );
            connect ( model, SIGNAL( modelReset () ), SLOT ( slotModelReset() ) );
        }
    }

#define DEBUG_HIT_COUNT 1

    const QPair<QPointF,  QPointF>& dataBoundaries()
    {   // FIXME implement for other chart types than normal,  this is
        // just a demo :-)
        Q_ASSERT_X ( diagram()->type() == Bar2Diagram::Normal, "ModelDataCache",
                     "sorry, not implemented: other types than Normal" );

#ifdef DEBUG_HIT_COUNT
        static int hitCount;
        static int missCount;
#endif
        if ( m_cachedDataBoundariesValid == false )
        {
            // ----- reset:
            m_cachedDataBoundaries = QPair<QPointF, QPointF> ();
            // ----- calculate model data bounds:
            QModelIndex root = diagram()->rootIndex();
            QAbstractItemModel* model = diagram()->model();
            if ( model )
            {
                AttributesModel* attributesModel = diagram()->attributesModel();
                Q_ASSERT ( attributesModel && "attributes model should always be valid" );

                const int rowCount = model->rowCount( root );
                const int columnCount = model->columnCount ( root );
                double xMin = 0;
                double xMax = 1.0 + rowCount;
                double yMin = 0;
                double yMax = 0;

                for ( int i=0; i<columnCount; ++i ) {
                    for ( int j=0; j< rowCount; ++j ) {
                        QModelIndex index = attributesModel->index( j, i, diagram()->attributesModelRootIndex() );
                        double value = attributesModel->data( index ).toDouble();
                    yMin = qMin( yMin, value );
                    yMax = qMax( yMax, value );
                    }
                }
                m_cachedDataBoundaries = QPair <QPointF, QPointF> (
                    QPointF( xMin, yMin ), QPointF( xMax, yMax ) );
            } // else: keep the empty range

            //       take note that the cache is valid:
            m_cachedDataBoundariesValid = true;
#ifdef DEBUG_HIT_COUNT
            ++missCount;
#endif
        }
#ifdef DEBUG_HIT_COUNT
        else {
            ++hitCount;
        }
#endif

#ifdef DEBUG_HIT_COUNT
        qDebug() << "ModelDataCache::dataBoundaries: returning " << m_cachedDataBoundaries
                 << " (cache hit ratio: " << ( 1.0*hitCount ) / missCount << " hits/miss)";
#endif
        return m_cachedDataBoundaries;
    }

public slots:
    void invalidate()
    {
        m_cachedDataBoundariesValid = false;
    }
    void slotDataChanged ( const QModelIndex&, const QModelIndex& )
    {
        invalidate();
    }

    void slotLayoutChanged()
    {
        invalidate();
    }

    void slotModelReset ()
    {
        invalidate();
    }

private:
    Bar2Diagram* diagram()
    {
        Q_ASSERT ( dynamic_cast<Bar2Diagram*> ( parent() ) );
        return static_cast<Bar2Diagram*> ( parent() );
    }

    bool m_cachedDataBoundariesValid;
    QPair<QPointF, QPointF> m_cachedDataBoundaries;
};


Bar2Diagram::Private::Private()
    : barType( Normal )
    , maxDepth ( 0 )
{
}

Bar2Diagram::Private::~Private() {}

#define d d_func()


Bar2Diagram::Bar2Diagram( CartesianCoordinatePlane* parent ) :
    AbstractCartesianDiagram( new Private(), parent )
{
    init();
}

void Bar2Diagram::init()
{
    d->barType = Bar2Diagram::Normal;
    d->modelDataCache = new ModelDataCache ( this );
}

Bar2Diagram::~Bar2Diagram()
{
}

Bar2Diagram * Bar2Diagram::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: Bar2Diagram * Bar2Diagram::clone() const" );
    return (Bar2Diagram*)0xdeadbeef;
}

void Bar2Diagram::setModel ( QAbstractItemModel* model )
{
    AbstractCartesianDiagram::setModel ( model );
    delete d->modelDataCache;
    d->modelDataCache = new ModelDataCache ( this );
}

void Bar2Diagram::setType( const BarType type )
{
    if ( type != d->barType )
    {
        d->barType = type;
        emit layoutChanged( this );
    }
}

Bar2Diagram::BarType Bar2Diagram::type() const
{
    return d->barType;
}

void Bar2Diagram::setBarAttributes( const BarAttributes & ta )
{
    attributesModel()->setModelData( qVariantFromValue( ta ), BarAttributesRole );
}

void Bar2Diagram::setBarAttributes( int column, const BarAttributes & ta )
{
    attributesModel()->setHeaderData( column, Qt::Vertical, qVariantFromValue( ta ),
                                      BarAttributesRole );
}

void Bar2Diagram::setBarAttributes( const QModelIndex & index, const BarAttributes & ta )
{
    attributesModel()->setData( d->attributesModel->mapFromSource(index), qVariantFromValue( ta ),
                                BarAttributesRole );
}

BarAttributes Bar2Diagram::barAttributes( const QModelIndex & index ) const
{
    return qVariantValue<BarAttributes>( attributesModel()->data( attributesModel()->mapFromSource(index),
                                                                  KDChart::BarAttributesRole ) );
}

void Bar2Diagram::setThreeDBarAttributes( const ThreeDBarAttributes & tda )
{
    attributesModel()->setModelData( qVariantFromValue( tda ), ThreeDBarAttributesRole );
    emit layoutChanged( this );
}

void Bar2Diagram::setThreeDBarAttributes( int column, const ThreeDBarAttributes & tda )
{
    attributesModel()->setHeaderData( column, Qt::Vertical, qVariantFromValue( tda ), ThreeDBarAttributesRole );
    emit layoutChanged( this );
}

void Bar2Diagram::setThreeDBarAttributes( const QModelIndex & index, const ThreeDBarAttributes & tda )
{
    attributesModel()->setData( attributesModel()->mapFromSource(index),
                                qVariantFromValue( tda ), ThreeDBarAttributesRole );
    emit layoutChanged( this );
}

ThreeDBarAttributes Bar2Diagram::threeDBarAttributes( const QModelIndex & index ) const
{
    return qVariantValue<ThreeDBarAttributes>( attributesModel()->data( attributesModel()->mapFromSource(index),
                                                                        KDChart::ThreeDBarAttributesRole ) );
}

const QPair<QPointF, QPointF> Bar2Diagram::dataBoundaries () const
{
    return d->modelDataCache->dataBoundaries();
}

void Bar2Diagram::paintEvent ( QPaintEvent*)
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

void Bar2Diagram::calculateValueAndGapWidths( int rowCount,int colCount,
                                              double groupWidth,
                                              double& outBarWidth,
                                              double& outSpaceBetweenBars,
                                              double& outSpaceBetweenGroups )
{
    Q_UNUSED( rowCount );

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

struct KDChart::BarInfo {
    double clientXCenter;
    double clientYNull; // bottom line y coordinate
    double clientBarWidth;
    double clientBarHeight;
};

void Bar2Diagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants() ) return;

    Q_ASSERT ( type() == Bar2Diagram::Normal ); // ni for other types
    // steps:
    // (1) calculate the bar positions, widths and heights
    // (2) draw the diagram
    // And no, we do not draw the grid. We align to decimal positions, and the
    // grid is drawn by the coordinate plane (we assume).
    // ----- set up the bars:
    const int rowCount = attributesModel()->rowCount(attributesModelRootIndex());
    const int columnCount = attributesModel()->columnCount(attributesModelRootIndex());
    for ( int row = 0; row < rowCount; ++row )
    {
        // we just draw the first column for now
        // for ( int column = 0; column < columnCount; ++column )
        const double value = attributesModel()->data(
            attributesModel()->index( row, 0, attributesModelRootIndex() ) ).toDouble();
        BarInfo bar = {
            1.0 + row, 0.0,
            0.6,
            value
        };
        drawBar2D ( bar, *ctx );
    }

//     // first draw the grid
//     //     drawGrid( ctx );
//     // Calculate width
//     QPointF boundLeft = coordinatePlane()->translate( dataBoundaries().first );
//     QPointF boundRight = coordinatePlane()->translate( dataBoundaries().second );

//     double width = boundRight.x() - boundLeft.x();
//     //calculates and stores the values
//     const int rowCount = d->attributesModel->rowCount(attributesModelRootIndex());
//     const int colCount = d->attributesModel->columnCount(attributesModelRootIndex());
//     DataValueTextInfoList list;
//     BarAttributes ba = barAttributes( model()->index( 0, 0, rootIndex() ) );
//     double maxValue = 0;
//     double sumValues = 0;
//     QVector <double > sumValuesVector;
//     double barWidth = 0;
//     double maxDepth = 0;
//     double spaceBetweenBars = 0;
//     double spaceBetweenGroups = 0;
//     double groupWidth =  /*ctx->rectangle().width() / ( rowCount + 2 )*/ width/ (rowCount + 2);


//     if ( ba.useFixedBarWidth() ) {
//         barWidth = ba.fixedBarWidth();
//         groupWidth += barWidth;

//         // Pending Michel set a min and max value for the groupWidth related to the area.width
//         // FixMe
//         if ( groupWidth < 0 )
//             groupWidth = 0;

//         if ( groupWidth  * rowCount > ctx->rectangle().width() )
//             groupWidth = ctx->rectangle().width() / rowCount;
//     }

//     // maxLimit: allow the space between bars to be larger until area.width() is covered by the groups.
//     double maxLimit = rowCount * (groupWidth + ((colCount-1) * ba.fixedDataValueGap()) );

//     //Pending Michel: FixMe
//     if ( ba.useFixedDataValueGap() ) {
//         if ( ctx->rectangle().width() > maxLimit )
//             spaceBetweenBars += ba.fixedDataValueGap();
//         else
//             spaceBetweenBars = ((ctx->rectangle().width()/rowCount) - groupWidth)/(colCount-1);
//     }

//     //Pending Michel: FixMe
//     if ( ba.useFixedValueBlockGap() )
//         spaceBetweenGroups += ba.fixedValueBlockGap();

//     calculateValueAndGapWidths( rowCount, colCount,groupWidth,
//                                 barWidth, spaceBetweenBars, spaceBetweenGroups );

//     // paint different bar types: Normal - Stacked - Percent
//     switch ( type() )
//     {
//     case Bar2Diagram::Normal:
//         // we paint the bars for all series next to each other, then move to the next value
//         for ( int i=0; i<rowCount; ++i ) {
//             double offset = -groupWidth/2 + spaceBetweenGroups/2;
//             // case fixed data value gap - handles max and min limits as well
//             if ( ba.useFixedDataValueGap() ) {
//                 if ( spaceBetweenBars > 0 ) {
//                     if ( ctx->rectangle().width() > maxLimit )
//                         offset -= ba.fixedDataValueGap();
//                     else
// 			offset -= ((ctx->rectangle().width()/rowCount) - groupWidth)/(colCount-1);

//                 } else {
//                     //allow reducing the gap until the bars are displayed next to each other - null gap
//                     offset += barWidth/2;
//                 }
//             }

//             for ( int j=0; j< colCount; ++j ) {
//                 // paint one group
//                 const double value = d->attributesModel->data( d->attributesModel->index( i, j, attributesModelRootIndex() ) ).toDouble();
//                 QPointF topPoint = coordinatePlane()->translate( QPointF( i + 0.5, value ) );
//                 QPointF bottomPoint = coordinatePlane()->translate( QPointF( i, 0 ) );
//                 const double barHeight = bottomPoint.y() - topPoint.y();
//                 topPoint.setX( topPoint.x() + offset );
//                 //PENDING Michel: FIXME barWidth
//                 QModelIndex index = model()->index( i, j, rootIndex() );
//                 list.append( DataValueTextInfo( index, topPoint, value ) );
//                 paintBars( ctx, index, QRectF( topPoint, QSizeF( barWidth, barHeight ) ), maxDepth );

//                 offset += barWidth + spaceBetweenBars;
//             }
//         }
//         break;
//     case Bar2Diagram::Stacked:
//         for ( int i = 0; i<colCount; ++i ) {
//             double offset = spaceBetweenGroups;
//             for ( int j = 0; j< rowCount; ++j ) {
//                 QModelIndex index = model()->index( j, i, rootIndex() );
//                 ThreeDBarAttributes tda = threeDBarAttributes( index );
//                 double value = 0, stackedValues = 0;
//                 QPointF point, previousPoint;

//                 if ( tda.isEnabled() ) {
//                     if ( barWidth > 0 )
//                         barWidth =  (width - ((offset+(tda.depth()))*rowCount))/ rowCount;
//                     if ( barWidth <= 0 ) {
//                         barWidth = 0;
//                         maxDepth = offset - (width/rowCount);
//                     }
//                 } else
//                     barWidth =  (ctx->rectangle().width() - (offset*rowCount))/ rowCount ;

//                 value = model()->data( index ).toDouble();
//                 for ( int k = i; k >= 0 ; --k )
//                     stackedValues += model()->data( model()->index( j, k, rootIndex() ) ).toDouble();
//                 point = coordinatePlane()->translate( QPointF( j, stackedValues ) );
//                 point.setX( point.x() + offset/2 );
//                 previousPoint = coordinatePlane()->translate( QPointF( j, stackedValues - value ) );
//                 const double barHeight = previousPoint.y() - point.y();
//                 list.append( DataValueTextInfo( index, point, value ) );
//                 paintBars( ctx, index, QRectF( point, QSizeF( barWidth , barHeight ) ), maxDepth );
//             }

//         }
//         break;
//     case Bar2Diagram::Percent:
//         // search for ordinate max value or 100 %
//         for ( int i=0; i<colCount; ++i ) {
//             for ( int j=0; j< rowCount; ++j ) {
//                 double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
//                 maxValue = qMax( maxValue, value );
//             }
//         }
//         //calculate sum of values for each column and store
//         for ( int j=0; j<rowCount; ++j ) {
//             for ( int i=0; i<colCount; ++i ) {
//                 double tmpValue = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
//                 if ( tmpValue > 0 )
//                     sumValues += tmpValue;
//                 if ( i == colCount-1 ) {
//                     sumValuesVector <<  sumValues ;
//                     sumValues = 0;
//                 }
//             }
//         }
//         // calculate stacked percent value
//         for ( int i = 0; i<colCount; ++i ) {
//             double offset = spaceBetweenGroups;
//             for ( int j=0; j<rowCount ; ++j ) {
//                 double value = 0, stackedValues = 0;
//                 QPointF point, previousPoint;
//                 QModelIndex index = model()->index( j, i, rootIndex() );
//                 ThreeDBarAttributes tda = threeDBarAttributes( index );

//                 if ( tda.isEnabled() ) {
//                     if ( barWidth > 0 )
//                         barWidth =  (width - ((offset+(tda.depth()))*rowCount))/ rowCount;
//                     if ( barWidth <= 0 ) {
//                         barWidth = 0;
//                         maxDepth = offset - ( width/rowCount);
//                     }
//                 }
//                 else
//                     barWidth = (ctx->rectangle().width() - (offset*rowCount))/ rowCount;

//                 value = model()->data( index ).toDouble();
//                 //calculate stacked percent value- we only take in account positives values for now.
//                 for ( int k = i; k >= 0 ; --k ) {
//                     double val = model()->data( model()->index( j, k, rootIndex() ) ).toDouble();
//                     if ( val > 0)
//                         stackedValues += val;
//                 }
//                 point = coordinatePlane()->translate( QPointF( j,  stackedValues/sumValuesVector.at(j)* maxValue ) );
//                 point.setX( point.x() + offset/2 );

//                 previousPoint = coordinatePlane()->translate( QPointF( j, (stackedValues - value)/sumValuesVector.at(j)* maxValue ) );
//                 const double barHeight = previousPoint.y() - point.y();

//                 list.append( DataValueTextInfo( index, point, value ) );
//                 paintBars( ctx, index, QRectF( point, QSizeF( barWidth, barHeight ) ), maxDepth );

//             }
//         }
//         break;
//     default:
//         Q_ASSERT_X ( false, "paint()",
//                      "Type item does not match a defined bar chart Type." );
//     }
//     */

//     DataValueTextInfoListIterator it( list );
//     while ( it.hasNext() ) {
//         const DataValueTextInfo& info = it.next();
//         paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
//     }
}

void Bar2Diagram::paintBars( PaintContext* ctx, const QModelIndex& index, const QRectF& bar, double& maxDepth )
{
    QRectF isoRect;
    QPolygonF topPoints, sidePoints;
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
        if ( maxDepth )
            tda.setDepth( -maxDepth );
        QPointF boundRight = coordinatePlane()->translate( dataBoundaries().second );
        //fixme adjust the painting to reasonable depth value
        switch ( type() )
        {
        case Bar2Diagram::Normal:
            usedDepth = tda.depth()/4;
            break;
        case Bar2Diagram::Stacked:
            usedDepth = tda.depth();
            break;
        case Bar2Diagram::Percent:
            usedDepth = tda.depth();
            break;
	default:
            Q_ASSERT_X ( false, "dataBoundaries()",
                         "Type item does not match a defined bar chart Type." );
        }
        isoRect =  bar.translated( usedDepth, -usedDepth );
        ctx->painter()->drawRect( isoRect );
        topPoints << bar.topLeft() << bar.topRight() << isoRect.topRight() << isoRect.topLeft();
        ctx->painter()->drawPolygon( topPoints );
        sidePoints << bar.topRight() << isoRect.topRight() << isoRect.bottomRight() << bar.bottomRight();
        ctx->painter()->drawPolygon( sidePoints );
    }

    ctx->painter()->drawRect( bar );
    // reset
    d->maxDepth = tda.depth();
}

void Bar2Diagram::resize ( const QSizeF& )
{
}

const int Bar2Diagram::numberOfAbscissaSegments () const
{
    return d->attributesModel->rowCount(attributesModelRootIndex());
}

const int Bar2Diagram::numberOfOrdinateSegments () const
{
    return d->attributesModel->columnCount(attributesModelRootIndex());
}

void Bar2Diagram::drawBar2D ( const BarInfo& bar,  PaintContext& ctx )
{
    // we will draw using the new 3D enabled Coordinate Plane:
    CoordinatePlane* plane = dynamic_cast<CoordinatePlane*> ( coordinatePlane() );
    Q_ASSERT ( plane );

    const QPointF bottomLeft ( - bar.clientBarWidth/2.0 + bar.clientXCenter, bar.clientYNull );
    const QPointF bottomRight ( bar.clientBarWidth/2.0 + bar.clientXCenter, bar.clientYNull );
    const QPointF topLeft ( bottomLeft.x(), bar.clientYNull + bar.clientBarHeight );
    const QPointF topRight( bottomRight.x(), bar.clientYNull + bar.clientBarHeight );
    const QSizeF barSize ( plane->translateDistance ( bottomRight, bottomLeft ),
                           plane->translateDistance ( topRight, bottomRight ) );

    QColor barColor ( "black" ); // a decent reminder to put the brush
                                 // attibute in
    barColor.setAlphaF ( 0.75 );

    QRectF barRect ( plane->translate ( topLeft ), barSize );

    ctx.painter()->drawRect ( barRect );
}

#include "BarDiagram.moc"

#undef d
