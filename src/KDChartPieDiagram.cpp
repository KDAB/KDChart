/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#include <QDebug>
#include <QPainter>
#include <QStack>

#include "KDChartAttributesModel.h"
#include "KDChartPaintContext.h"
#include "KDChartPieDiagram.h"
#include "KDChartPieDiagram_p.h"
#include "KDChartPieAttributes.h"
#include "KDChartThreeDPieAttributes.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartDataValueAttributes.h"

#include <KDABLibFakes>


using namespace KDChart;

PieDiagram::Private::Private()
{
}

PieDiagram::Private::~Private() {}

#define d d_func()

PieDiagram::PieDiagram( QWidget* parent, PolarCoordinatePlane* plane ) :
    AbstractPieDiagram( new Private(), parent, plane )
{
    init();
}

PieDiagram::~PieDiagram()
{
}

void PieDiagram::init()
{
}

PieDiagram * PieDiagram::clone() const
{
    return new PieDiagram( new Private( *d ) );
}

const QPair<QPointF, QPointF> PieDiagram::calculateDataBoundaries () const
{
    if ( !checkInvariants( true ) ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );

    const PieAttributes attrs( pieAttributes( model()->index( 0, 0, rootIndex() ) ) );

    QPointF bottomLeft ( QPointF( 0, 0 ) );
    QPointF topRight;
    // If we explode, we need extra space for the pie slice that has
    // the largest explosion distance.
    if ( attrs.explode() ) {
        const int colCount = columnCount();
        qreal maxExplode = 0.0;
        for( int j = 0; j < colCount; ++j ){
            const PieAttributes columnAttrs( pieAttributes( model()->index( 0, j, rootIndex() ) ) );
            maxExplode = qMax( maxExplode, columnAttrs.explodeFactor() );
        }
        topRight = QPointF( 1.0+maxExplode, 1.0+maxExplode );
    }else{
        topRight = QPointF( 1.0, 1.0 );
    }
    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}


void PieDiagram::paintEvent( QPaintEvent* )
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

void PieDiagram::resizeEvent ( QResizeEvent*)
{
}

void PieDiagram::resize ( const QSizeF& )
{
}

static QRectF buildReferenceRect( const PolarCoordinatePlane* plane )
{
    QRectF contentsRect;
//qDebug() << "..........................................";
    QPointF referencePointAtTop = plane->translate( QPointF( 1, 0 ) );
    QPointF temp = plane->translate( QPointF( 0, 0 ) ) - referencePointAtTop;
    const double offset = temp.y();
    referencePointAtTop.setX( referencePointAtTop.x() - offset );
    contentsRect.setTopLeft( referencePointAtTop );
    contentsRect.setBottomRight( referencePointAtTop + QPointF( 2*offset, 2*offset) );
//qDebug() << contentsRect;
    return contentsRect;
}
/*
void PieDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants(true) ) return;
    const int colCount = model()->columnCount(rootIndex());
    QRectF contentsRect = buildReferenceRect( polarCoordinatePlane() );
    DataValueTextInfoList list;
    double startAngle = startPosition();
    double startAngleValueSpace = valueTotals() / 360 * startAngle;
    for ( int j=0; j<colCount; ++j ) {
        const double nextValue = qAbs( model()->data( model()->index( 0, j,rootIndex() ) ).toDouble() );
        double spanAngle = polarCoordinatePlane()->translatePolar( QPointF( nextValue, 1 ) ).x();
        if ( spanAngle == 0 ) continue;
        QBrush brush = qVariantValue<QBrush>( attributesModel()->headerData( j, Qt::Vertical, KDChart::DatasetBrushRole ) );
        QPen pen = qVariantValue<QPen>( attributesModel()->headerData( j, Qt::Vertical, KDChart::DatasetPenRole ) );
        PainterSaver painterSaver( ctx->painter() );
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( brush );
        ctx->painter()->setPen( pen );

        // Explosion support
        QRectF pieRect = contentsRect;
        if( explode() ) {
            QPointF oldCenter = contentsRect.center();
            QPointF newCenter = polarCoordinatePlane()->translate( QPointF( explodeFactor( j ),
                                                                            startAngleValueSpace + nextValue/2.0 ) );
            QPointF difference = newCenter - oldCenter;
            pieRect.translate( difference );
        }

        ctx->painter()->drawPie( pieRect, ( int ) ((-startAngle + 90 )), ( int ) (-spanAngle) );
        startAngle += spanAngle;
        startAngleValueSpace += nextValue;
    }
    DataValueTextInfoListIterator it( list );
    while ( it.hasNext() ) {
        const DataValueTextInfo& info = it.next();
        paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
    }
}
*/


void PieDiagram::paint( PaintContext* ctx )
{
    // note: Not having any data model assigned is no bug
    //       but we can not draw a diagram then either.
    if ( !checkInvariants(true) )
        return;

    const PieAttributes attrs( pieAttributes() );
    const ThreeDPieAttributes threeDAttrs( threeDPieAttributes( model()->index( 0, 0, rootIndex() ) ) );

    const int colCount = columnCount();

    QRectF contentsRect( buildReferenceRect( polarCoordinatePlane() ) );
    contentsRect = ctx->rectangle();
//    contentsRect = geometry();
//qDebug() << contentsRect;
    if( contentsRect.isEmpty() )
        return;

    DataValueTextInfoList list;
    const qreal sum = valueTotals();

    if( sum == 0.0 ) //nothing to draw
        return;

    d->startAngles.resize( colCount );
    d->angleLens.resize( colCount );

    // compute position
    d->size = qMin( contentsRect.width(), contentsRect.height() ); // initial size

    // if the pies explode, we need to give them additional space =>
    // make the basic size smaller
    qreal maxExplode = 0.0;
    for( int j = 0; j < colCount; ++j ){
        const PieAttributes columnAttrs( pieAttributes( model()->index( 0, j, rootIndex() ) ) );
        maxExplode = qMax( maxExplode, columnAttrs.explodeFactor() );
    }
    d->size /= ( 1.0 + 2.0 * maxExplode );


    qreal sizeFor3DEffect = 0.0;
    if ( ! threeDAttrs.isEnabled() ) {

        qreal x = ( contentsRect.width() == d->size ) ? 0.0 : ( ( contentsRect.width() - d->size ) / 2.0 );
        qreal y = ( contentsRect.height() == d->size ) ? 0.0 : ( ( contentsRect.height() - d->size ) / 2.0 );
        d->position = QRectF( x, y, d->size, d->size );
        d->position.translate( contentsRect.left(), contentsRect.top() );
    } else {
        // threeD: width is the maximum possible width; height is 1/2 of that
        qreal x = ( contentsRect.width() == d->size ) ? 0.0 : ( ( contentsRect.width() - d->size ) / 2.0 );
        qreal height = d->size;
        // make sure that the height plus the threeDheight is not more than the
        // available size
        if ( threeDAttrs.depth() >= 0.0 ) {
            // positive pie height: absolute value
            sizeFor3DEffect = threeDAttrs.depth();
            height = d->size - sizeFor3DEffect;
        } else {
            // negative pie height: relative value
            sizeFor3DEffect = - threeDAttrs.depth() / 100.0 * height;
            height = d->size - sizeFor3DEffect;
        }
        qreal y = ( contentsRect.height() == height ) ? 0.0 : ( ( contentsRect.height() - height - sizeFor3DEffect ) / 2.0 );

        d->position = QRectF( contentsRect.left() + x, contentsRect.top() + y,
                d->size, height );
        //  d->position.moveBy( contentsRect.left(), contentsRect.top() );
    }

    const PolarCoordinatePlane * plane = polarCoordinatePlane();
    const qreal sectorsPerValue = 360.0 / sum;
    qreal currentValue = plane ? plane->startPosition() : 0.0;

    bool atLeastOneValue = false; // guard against completely empty tables
    QVariant vValY;
    for ( int iColumn = 0; iColumn < colCount; ++iColumn ) {
        // is there anything at all at this column?
        bool bOK;
        const double cellValue = qAbs( model()->data( model()->index( 0, iColumn, rootIndex() ) )
            .toDouble( &bOK ) );

        if( bOK ){
            d->startAngles[ iColumn ] = currentValue;
            d->angleLens[ iColumn ] = cellValue * sectorsPerValue;
            atLeastOneValue = true;
        } else { // mark as non-existent
            d->angleLens[ iColumn ] = 0.0;
            if ( iColumn > 0.0 )
                d->startAngles[ iColumn ] = d->startAngles[ iColumn - 1 ];
            else
                d->startAngles[ iColumn ] = currentValue;
        }
        //qDebug() << "d->startAngles["<<iColumn<<"] == " << d->startAngles[ iColumn ]
        //         << " +  d->angleLens["<<iColumn<<"]" << d->angleLens[ iColumn ]
        //         << " = " << d->startAngles[ iColumn ]+d->angleLens[ iColumn ];

        currentValue = d->startAngles[ iColumn ] + d->angleLens[ iColumn ];
    }

    // If there was no value at all, bail out, to avoid endless loops
    // later on (e.g. in findPieAt()).
    if( ! atLeastOneValue )
        return;


    // Find the backmost pie which is at +90° and needs to be drawn
    // first
    int backmostpie = findPieAt( 90, colCount );
    // Find the frontmost pie (at -90°/+270°) that should be drawn last
    int frontmostpie = findPieAt( 270, colCount );
    // the right- and the leftmost (only needed in some special cases...)
    int rightmostpie = findPieAt( 0, colCount );
    int leftmostpie = findPieAt( 180, colCount );


    int currentLeftPie = backmostpie;
    int currentRightPie = backmostpie;

    drawOnePie( ctx->painter(), 0, backmostpie, granularity(), sizeFor3DEffect );

    if( backmostpie == frontmostpie )
    {
        if( backmostpie == leftmostpie )
            currentLeftPie = findLeftPie( currentLeftPie, colCount );
        if( backmostpie == rightmostpie )
            currentRightPie = findRightPie( currentRightPie, colCount );
    }
    while( currentLeftPie != frontmostpie )
    {
        if( currentLeftPie != backmostpie )
            drawOnePie( ctx->painter(), 0, currentLeftPie, granularity(), sizeFor3DEffect );
        currentLeftPie = findLeftPie( currentLeftPie, colCount );
    }
    while( currentRightPie != frontmostpie )
    {
        if( currentRightPie != backmostpie )
            drawOnePie( ctx->painter(), 0, currentRightPie, granularity(), sizeFor3DEffect );
        currentRightPie = findRightPie( currentRightPie, colCount );
    }

    // if the backmost pie is not the frontmost pie, we draw the frontmost at last
    if( backmostpie != frontmostpie || ! threeDPieAttributes().isEnabled() )
    {
        drawOnePie( ctx->painter(), 0, frontmostpie, granularity(), sizeFor3DEffect );
    // else, this gets a bit mor complicated...
    } else if( threeDPieAttributes().isEnabled() ) {
        drawPieSurface( ctx->painter(), 0, frontmostpie, granularity() );
        const QModelIndex index = model()->index( 0, frontmostpie, rootIndex() );
        QPen pen = this->pen( index );
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( brush( index ) );
        if ( threeDAttrs.isEnabled() )
            pen.setColor( QColor( 0, 0, 0 ) );
        ctx->painter()->setPen( pen );

        qreal startAngle = d->startAngles[ frontmostpie ];
        if( startAngle > 360 )
            startAngle -= 360;

        qreal endAngle = startAngle + d->angleLens[ frontmostpie ];
        startAngle = qMax( startAngle, 180.0 );

        drawArcEffectSegment( ctx->painter(), piePosition( 0, frontmostpie),
                sizeFor3DEffect, startAngle, endAngle, granularity() );
    }
}

#if defined ( Q_WS_WIN)
#define trunc(x) ((int)(x))
#endif

QRectF PieDiagram::piePosition( uint dataset, uint pie ) const
{
    qreal angleLen = d->angleLens[ pie ];
    qreal startAngle = d->startAngles[ pie ];
    QModelIndex index( model()->index( 0, pie, rootIndex() ) );
    const PieAttributes attrs( pieAttributes( index ) );
    const ThreeDPieAttributes threeDAttrs( threeDPieAttributes( index ) );

    QRectF drawPosition( d->position );

    if ( attrs.explode() ) {
        qreal explodeAngle = ( startAngle + angleLen / 2.0 );
        qreal explodeAngleRad = DEGTORAD( explodeAngle );
        qreal cosAngle = cos( explodeAngleRad );
        qreal sinAngle = -sin( explodeAngleRad );
        qreal explodeX = attrs.explodeFactor() * d->size * cosAngle;
        qreal explodeY = attrs.explodeFactor() * d->size * sinAngle;
        drawPosition.translate( explodeX, explodeY );
    }else{
        drawPosition = d->position;
    }
    return drawPosition;
 }

/**
  Internal method that draws one of the pies in a pie chart.

  \param painter the QPainter to draw in
  \param dataset the dataset to draw the pie for
  \param pie the pie to draw
  \param threeDPieHeight the height of the three dimnensional effect
  */
void PieDiagram::drawOnePie( QPainter* painter,
        uint dataset, uint pie,
        qreal granularity,
        qreal threeDPieHeight )
{
    // Is there anything to draw at all?
    qreal angleLen = d->angleLens[ pie ];
    if ( angleLen ) {
  //      qreal startAngle = d->startAngles[ pie ];
/*
        KDChartDataRegion* datReg = 0;
        QRegion* region = 0;
        bool mustDeleteRegion = false;
        if ( regions ){
            region = new QRegion();
            mustDeleteRegion = true;
        }
*/
        QModelIndex index( model()->index( 0, pie, rootIndex() ) );
        const PieAttributes attrs( pieAttributes( index ) );
        const ThreeDPieAttributes threeDAttrs( threeDPieAttributes( index ) );

        QRectF drawPosition = piePosition( dataset, pie );

        draw3DEffect( painter,
            drawPosition, dataset, pie,
            granularity,
            threeDAttrs,
            attrs.explode() );

        drawPieSurface( painter, dataset, pie, granularity );
    }
}

/**
  Internal method that draws the surface of one of the pies in a pie chart.

  \param painter the QPainter to draw in
  \param dataset the dataset to draw the pie for
  \param pie the pie to draw
  */
void PieDiagram::drawPieSurface( QPainter* painter,
        uint dataset, uint pie,
        qreal granularity )
{
    // Is there anything to draw at all?
    qreal angleLen = d->angleLens[ pie ];
    if ( angleLen ) {
        qreal startAngle = d->startAngles[ pie ];
/*
        KDChartDataRegion* datReg = 0;
        QRegion* region = 0;
        bool mustDeleteRegion = false;
        if ( regions ){
            region = new QRegion();
            mustDeleteRegion = true;
        }
*/
        QModelIndex index( model()->index( 0, pie, rootIndex() ) );
        const PieAttributes attrs( pieAttributes( index ) );
        const ThreeDPieAttributes threeDAttrs( threeDPieAttributes( index ) );

        QRectF drawPosition = piePosition( dataset, pie );
//        QRectF drawPosition( d->position );

/*        if ( attrs.explode() ) {
            qreal explodeAngle = ( startAngle + angleLen / 2.0 );
            qreal explodeAngleRad = DEGTORAD( explodeAngle );
            qreal cosAngle = cos( explodeAngleRad );
            qreal sinAngle = -sin( explodeAngleRad );
            qreal explodeX = attrs.explodeFactor() * d->size * cosAngle;
            qreal explodeY = attrs.explodeFactor() * d->size * sinAngle;
            drawPosition.translate( explodeX, explodeY );
        }else{
            drawPosition = d->position;
        }*/

        QPen pen = this->pen( index );
        PainterSaver painterSaver( painter );
        painter->setRenderHint ( QPainter::Antialiasing );
        painter->setBrush( brush( index ) );
        if ( threeDAttrs.isEnabled() )
            pen.setColor( QColor( 0, 0, 0 ) );
        painter->setPen( pen );

        if ( angleLen == 360 ) {
            // full circle, avoid nasty line in the middle
            painter->drawEllipse( drawPosition );
/*
            if ( regions ) {
                QPointArray hitregion;
                hitregion.makeEllipse( drawPosition.x(), drawPosition.y(),
                                       drawPosition.width(),
                                       drawPosition.height() );
                datReg = new KDChartDataRegion( region->unite( QRegion( hitregion ) ),
                                                dataset,
                                                pie,
                                                chart );
                datReg->points[ KDChartEnums::PosCenter ]
                    = drawPosition.center();
                datReg->points[ KDChartEnums::PosCenterRight ]
                    = pointOnCircle( drawPosition,    0 );
                datReg->points[ KDChartEnums::PosTopRight ]
                    = pointOnCircle( drawPosition,  45 );
                datReg->points[ KDChartEnums::PosTopCenter ]
                    = pointOnCircle( drawPosition, 90 );
                datReg->points[ KDChartEnums::PosTopLeft ]
                    = pointOnCircle( drawPosition, 135 );
                datReg->points[ KDChartEnums::PosCenterLeft ]
                    = pointOnCircle( drawPosition, 180 );
                datReg->points[ KDChartEnums::PosBottomLeft ]
                    = pointOnCircle( drawPosition, 225 );
                datReg->points[ KDChartEnums::PosBottomCenter ]
                    = pointOnCircle( drawPosition, 270 );
                datReg->points[ KDChartEnums::PosBottomRight ]
                    = pointOnCircle( drawPosition, 315 );
                datReg->startAngle = 180;
                datReg->angleLen   = 360;
                regions->append( datReg );
            }
*/
        } else {
            // draw the top of this piece
            // Start with getting the points for the arc.
            const int arcPoints = static_cast<int>(trunc( angleLen / granularity ));
            QPolygonF poly( arcPoints+2 );
            qreal degree=0.0;
            int iPoint = 0;
            bool perfectMatch = false;

            while ( degree <= angleLen ){
                poly[ iPoint ] = pointOnCircle( drawPosition, startAngle + degree );
                //qDebug() << degree << angleLen << poly[ iPoint ];
                perfectMatch = (degree == angleLen);
                degree += granularity;
                ++iPoint;
            }
            int last = poly.size();
            // if necessary add one more point to fill the last small gap
            if( ! perfectMatch ){
                poly[ iPoint ] = pointOnCircle( drawPosition, startAngle + angleLen );

//qDebug() << "adding" << poly[ iPoint ];
                // add the center point of the piece
                poly.append( drawPosition.center() );
//qDebug() << "center:" << poly[ ++iPoint ];
            }else{
                poly[ iPoint ] = drawPosition.center();
//qDebug() << "center:" << poly[ iPoint ];
            }
//qDebug() << "a";
            //find the value and paint it
            //fix value position
            const qreal sum = valueTotals();
            painter->drawPolygon( poly );

            QLineF centerLine(  drawPosition.center(),
                            QPointF( (poly[ last - 2].x() + poly.first().x())/2,
                                     ( poly.first().y() + poly[last-2].y() )/2 ) );
            QPointF valuePos( ( centerLine.x1() + centerLine.x2() )/2,
                                  ( centerLine.y1() + centerLine.y2() )/2 ) ;

            paintDataValueText( painter, index, valuePos, angleLen*sum / 360  );

//if( bHelp ){
                            //painter->drawPolyline( collect );
//bHelp=false;
//}


/*
            if ( regions ) {
                QPointArray hitregion;
                hitregion.makeArc( drawPosition.x(), drawPosition.y(),
                        drawPosition.width(),
                        drawPosition.height(),
                        ( int ) startAngle, ( int ) angleLen );
                hitregion.resize( hitregion.size() + 1 );
                hitregion.setPoint( hitregion.size() - 1,
                        drawPosition.center() );
                datReg = new KDChartDataRegion( region->unite( QRegion( hitregion ) ),
                                                dataset,
                                                pie,
                                                chart );

                datReg->points[ KDChartEnums::PosTopLeft ]
                    = pointOnCircle( drawPosition, startAngle + angleLen );
                datReg->points[ KDChartEnums::PosTopCenter ]
                    = pointOnCircle( drawPosition, startAngle + angleLen / 2 );
                datReg->points[ KDChartEnums::PosTopRight ]
                    = pointOnCircle( drawPosition, startAngle );

                datReg->points[   KDChartEnums::PosBottomLeft   ] = drawPosition.center();
                datReg->points[   KDChartEnums::PosBottomCenter ]
                    = datReg->points[ KDChartEnums::PosBottomLeft   ];
                datReg->points[   KDChartEnums::PosBottomRight  ]
                    = datReg->points[ KDChartEnums::PosBottomLeft   ];

                datReg->points[ KDChartEnums::PosCenterLeft ]
                    = QPoint( (   datReg->points[ KDChartEnums::PosTopLeft      ].x()
                                + datReg->points[ KDChartEnums::PosBottomLeft   ].x() ) / 2,
                            (   datReg->points[ KDChartEnums::PosTopLeft      ].y()
                                + datReg->points[ KDChartEnums::PosBottomLeft   ].y() ) / 2 );
                datReg->points[ KDChartEnums::PosCenter ]
                    = QPoint( (   datReg->points[ KDChartEnums::PosTopCenter    ].x()
                                + datReg->points[ KDChartEnums::PosBottomCenter ].x() ) / 2,
                            (   datReg->points[ KDChartEnums::PosTopCenter    ].y()
                                + datReg->points[ KDChartEnums::PosBottomCenter ].y() ) / 2 );
                datReg->points[ KDChartEnums::PosCenterRight ]
                    = QPoint( (   datReg->points[ KDChartEnums::PosTopRight     ].x()
                                + datReg->points[ KDChartEnums::PosBottomRight  ].x() ) / 2,
                            (   datReg->points[ KDChartEnums::PosTopRight     ].y()
                                + datReg->points[ KDChartEnums::PosBottomRight  ].y() ) / 2 );

                datReg->startAngle = startAngle;
                datReg->angleLen   = angleLen;
                regions->append( datReg );
            }
*/
        }
//        if( mustDeleteRegion )
//            delete region;
    }
}


/**
  Internal method that draws the shadow creating the 3D effect of a pie

  \param painter the QPainter to draw in
  \param drawPosition the position to draw at
  \param dataset the dataset to draw the pie for
  \param pie the pie to draw the shadow for
  \param threeDHeight the height of the shadow
  */
void PieDiagram::draw3DEffect( QPainter* painter,
        const QRectF& drawPosition,
        uint dataset, uint pie,
        qreal granularity,
        const ThreeDPieAttributes& threeDAttrs,
        bool /*explode*/ )
{
    if( ! threeDAttrs.isEnabled() )
        return;

    // NOTE: We cannot optimize away drawing some of the effects (even
    // when not exploding), because some of the pies might be left out
    // in future versions which would make some of the normally hidden
    // pies visible. Complex hidden-line algorithms would be much more
    // expensive than just drawing for nothing.

    // No need to save the brush, will be changed on return from this
    // method anyway.
    if( threeDAttrs.useShadowColors() ){
        const QPen pen = this->pen( model()->index( 0, pie, rootIndex() ) );
        painter->setBrush( QBrush( pen.color() ) );
    }
    //painter->setBrush( QBrush( threeDAttrs.dataShadow1Color( pie ),
    //            params()->shadowPattern() ) );

    qreal startAngle = d->startAngles[ pie ];
    qreal endAngle = startAngle + d->angleLens[ pie ];
    // Normalize angles
    while ( startAngle >= 360 )
        startAngle -= 360;
    while ( endAngle >= 360 )
        endAngle -= 360;
    Q_ASSERT( startAngle >= 0 && startAngle <= 360 );
    Q_ASSERT( endAngle >= 0 && endAngle <= 360 );

    //int centerY = drawPosition.center().y();

    if ( startAngle == endAngle ||
            startAngle == endAngle - 360 ) { // full circle
        drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                180, 360, granularity );
    } else if ( startAngle <= 90 ) {
        if ( endAngle <= 90 ) {
            if ( startAngle <= endAngle ) {
                /// starts and ends in first quadrant, less than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(), startAngle );
                drawUpperBrinkEffect( painter, drawPosition, endAngle );
            } else {
                /// starts and ends in first quadrant, more than 3/4
                drawStraightEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(), startAngle );
                drawUpperBrinkEffect( painter, drawPosition, endAngle );
                drawArcEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(),
                    180, 360, granularity );
            }
        } else if ( endAngle <= 180 ) {
            /// starts in first quadrant, ends in second quadrant,
            /// less than 1/2
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), startAngle );
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), endAngle );
        } else if ( endAngle <= 270 ) {
            /// starts in first quadrant, ends in third quadrant
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), startAngle );
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), endAngle );
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                180, endAngle, granularity );
        } else { // 270*16 < endAngle < 360*16
            /// starts in first quadrant, ends in fourth quadrant,
            /// more than 3/4
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), startAngle );
            drawUpperBrinkEffect( painter, drawPosition, endAngle );
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                180, endAngle, granularity );
        }
    } else if ( startAngle <= 180 ) {
        if ( endAngle <= 90 ) {
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                180, 360, granularity );
            drawUpperBrinkEffect( painter, drawPosition, startAngle );
            drawUpperBrinkEffect( painter, drawPosition, endAngle );
        } else if ( endAngle <= 180 ) {
            if ( startAngle <= endAngle ) {
                /// starts in second quadrant, ends in second
                /// quadrant, less than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(), endAngle );
                drawUpperBrinkEffect( painter, drawPosition, startAngle );
            } else {
                /// starts in second quadrant, ends in second
                /// quadrant, more than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(), endAngle );
                drawUpperBrinkEffect( painter, drawPosition, startAngle );
                drawArcEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(),
                    180, 360, granularity );
            }
        } else if ( endAngle <= 270 ) {
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), endAngle );
            drawUpperBrinkEffect( painter, drawPosition, startAngle );
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                180, endAngle, granularity );
        } else { // 270*16 < endAngle < 360*16
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                180, endAngle, granularity );
            drawUpperBrinkEffect( painter, drawPosition, startAngle );
            drawUpperBrinkEffect( painter, drawPosition, endAngle );
        }
    } else if ( startAngle <= 270 ) {
        if ( endAngle <= 90 ) {
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                startAngle, 360, granularity );
            drawUpperBrinkEffect( painter, drawPosition, startAngle );
            drawUpperBrinkEffect( painter, drawPosition, endAngle );
        } else if ( endAngle <= 180 ) {
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), endAngle );
            drawUpperBrinkEffect( painter, drawPosition, startAngle );
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                startAngle, 360, granularity );
        } else if ( endAngle <= 270 ) {
            if ( startAngle <= endAngle ) {
                /// starts in third quadrant, ends in third quadrant,
                /// less than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(), endAngle );
                drawUpperBrinkEffect( painter, drawPosition, startAngle );
                drawArcEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(),
                    startAngle, endAngle, granularity );
            } else {
                /// starts in third quadrant, ends in third quadrant,
                /// more than 3/4
                drawStraightEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(), endAngle );
                drawUpperBrinkEffect( painter, drawPosition, startAngle );
                drawArcEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(),
                    180, endAngle, granularity );
                drawArcEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(),
                    startAngle, 360, granularity );
            }
        } else { // 270*16 < endAngle < 360*16
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                startAngle, endAngle, granularity );
            drawUpperBrinkEffect( painter, drawPosition, startAngle );
            drawUpperBrinkEffect( painter, drawPosition, endAngle );
        }
    } else { // 270*16 < startAngle < 360*16
        if ( endAngle <= 90 ) {
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), startAngle );
            drawUpperBrinkEffect( painter, drawPosition, endAngle );
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                startAngle, 360, granularity );
        } else if ( endAngle <= 180 ) {
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), startAngle );
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), endAngle );
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                startAngle, 360, granularity );
        } else if ( endAngle <= 270 ) {
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), startAngle );
            drawStraightEffectSegment( painter, drawPosition,
                threeDAttrs.depth(), endAngle );
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                180, endAngle, granularity );
            drawArcEffectSegment( painter, drawPosition,
                threeDAttrs.depth(),
                startAngle, 360, granularity );
        } else { // 270*16 < endAngle < 360*16
            if ( startAngle <= endAngle ) {
                /// starts in fourth quadrant, ends in fourth
                /// quadrant, less than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(), startAngle );
                drawUpperBrinkEffect( painter, drawPosition, endAngle );
                drawArcEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(),
                    startAngle, endAngle, granularity );
            } else {
                /// starts in fourth quadrant, ends in fourth
                /// quadrant, more than 3/4
                drawStraightEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(), startAngle );
                drawUpperBrinkEffect( painter, drawPosition, endAngle );
                drawArcEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(),
                    startAngle, 360, granularity );
                drawArcEffectSegment( painter, drawPosition,
                    threeDAttrs.depth(),
                    180, endAngle, granularity );
            }
        }
    }
    drawArcUpperBrinkEffectSegment( painter, drawPosition, startAngle, endAngle, granularity );
}


/**
  Internal method that draws a segment with a straight 3D effect

  \param painter the QPainter to draw in
  \param rect the position to draw at
  \param threeDHeight the height of the shadow
  \param angle the angle of the segment
  */
void PieDiagram::drawStraightEffectSegment( QPainter* painter,
        const QRectF& rect,
        qreal threeDHeight,
        qreal angle )
{
    QPolygonF poly( 4 );
    const QPointF center = rect.center();
    const QPointF circlePoint = pointOnCircle( rect, angle );
    poly[0] = center;
    poly[1] = circlePoint;
    poly[2] = QPointF( circlePoint.x(), circlePoint.y() + threeDHeight );
    poly[3] = QPointF( center.x(), center.y() + threeDHeight );
    painter->drawPolygon( poly );
//    if ( region )
//        *region += QRegion( points );
}

/**
  Internal method that draws the upper brink of a 3D pie piece

  \param painter the QPainter to draw in
  \param rect the position to draw at
  \param angle the angle of the segment
  */
void PieDiagram::drawUpperBrinkEffect( QPainter* painter,
        const QRectF& rect,
        qreal angle )
{
    const QPointF center = rect.center();
    const QPointF circlePoint = pointOnCircle( rect, angle );
    painter->drawLine( center, circlePoint );
}

/**
  Internal method that draws a segment with an arc 3D effect

  \param painter the QPainter to draw in
  \param rect the position to draw at
  \param threeDHeight the height of the shadow
  \param startAngle the starting angle of the segment
  \param endAngle the ending angle of the segment
  */
void PieDiagram::drawArcEffectSegment( QPainter* painter,
        const QRectF& rect,
        qreal threeDHeight,
        qreal startAngle,
        qreal endAngle,
        qreal granularity )
{
    // Start with getting the points for the inner arc.
    qreal startA = qMin( startAngle, endAngle );
    qreal endA   = qMax( startAngle, endAngle );

    // sometimes we have to draw two segments, which are on different sides of the pie
    if( endA > 540 )
        drawArcEffectSegment( painter, rect, threeDHeight, 180, endA - 360, granularity );
    if( endA > 360 )
        endA = qMin( endA, 360.0 );

    int numHalfPoints = static_cast<int>( trunc( ( endA - startA ) / granularity ) ) + 1;

    QPolygonF poly( numHalfPoints );

    qreal degree = endA;
    int iPoint = 0;
    bool perfectMatch = false;
    while ( degree >= startA ){
        poly[ numHalfPoints - iPoint - 1 ] = pointOnCircle( rect, degree );

        perfectMatch = (degree == startA);
        degree -= granularity;
        ++iPoint;
    }
    // if necessary add one more point to fill the last small gap
    if( ! perfectMatch ){
        poly.prepend( pointOnCircle( rect, startA ) );
        ++numHalfPoints;
    }

    poly.resize( numHalfPoints * 2 );

    // Now copy these arcs again into the final array, but in the
    // opposite direction and moved down by the 3D height.
    for ( int i = numHalfPoints - 1; i >= 0; --i ) {
        QPointF pointOnFirstArc( poly[ i ] );
        pointOnFirstArc.setY( pointOnFirstArc.y() + threeDHeight );
        poly[ numHalfPoints * 2 - i - 1 ] = pointOnFirstArc;
    }

    painter->drawPolygon( poly );
//    if ( region )
//        *region += QRegion( collect );
}

/**
  Internal method that draws the upper brink of a 3D pie segment

  \param painter the QPainter to draw in
  \param rect the position to draw at
  \param startAngle the starting angle of the segment
  \param endAngle the ending angle of the segment
  */
void PieDiagram::drawArcUpperBrinkEffectSegment( QPainter* painter,
        const QRectF& rect,
        qreal startAngle,
        qreal endAngle,
        qreal granularity )
{
    if ( endAngle < startAngle )
        endAngle += 360;
    // Start with getting the poits for the inner arc.
    const qreal startA = qMin( startAngle, endAngle );
    const qreal endA   = qMax( startAngle, endAngle );

    int numHalfPoints = static_cast<int>( trunc( ( endA - startA ) / granularity ) ) + 1;

    QPolygonF poly( numHalfPoints );

    qreal degree = endA;
    int iPoint = 0;
    bool perfectMatch = false;
    while ( degree >= startA ){
        poly[ numHalfPoints - iPoint - 1 ] = pointOnCircle( rect, degree );

        perfectMatch = (degree == startA);
        degree -= granularity;
        ++iPoint;
    }
    // if necessary add one more point to fill the last small gap
    if( ! perfectMatch ){
        poly.prepend( pointOnCircle( rect, startA ) );
        ++numHalfPoints;
    }

    painter->drawPolyline( poly );
//    if ( region )
//        *region += QRegion( collect );
}

/**
  Internal method that finds the pie that is located at the position
  specified by \c angle.

  \param angle the angle at which to search for a pie
  \return the number of the pie found
  */
uint PieDiagram::findPieAt( qreal angle, int colCount )
{
    for ( int i = 0; i < colCount; ++i ) {
        qreal endseg = d->startAngles[ i ] + d->angleLens[ i ];
        if ( ( d->startAngles[ i ] <= angle ) &&
                ( endseg >= angle ) )
            // found!
            return i;
    }

    // If we have not found it, try wrap around
    // but only if the current searched angle is < 360 degree
    if ( angle < 360 )
        return findPieAt( angle + 360, colCount );
    // otherwise - what ever went wrong - we return 0
    return 0;
}


/**
  Internal method that finds the pie that is located to the left of
  the pie specified by \c pie.

  \param pie the pie to start the search from
  \return the number of the pie to the left of \c pie
  */
uint PieDiagram::findLeftPie( uint pie, int colCount )
{
    if ( pie == 0 )
        if ( colCount > 1 )
            return colCount - 1;
        else
            return 0;
    else {
        return pie - 1;
    }
}


/**
  Internal method that finds the pie that is located to the right of
  the pie specified by \c pie.

  \param pie the pie to start the search from
  \return the number of the pie to the right of \c pie
  */
uint PieDiagram::findRightPie( uint pie, int colCount  )
{
    int rightpie = pie + 1;
    if ( rightpie == colCount )
        rightpie = 0;
    return rightpie;
}

/*
/ **
  This method is a specialization that returns a fallback legend text
  appropriate for pies that do not have more than one dataset

  This method is only used when automatic legends are used, because
  manual and first-column legends do not need fallback texts.

  \param uint dataset the dataset number for which to generate a
  fallback text
  \return the fallback text to use for describing the specified
  dataset in the legend
  * /
QString PieDiagram::fallbackLegendText( uint dataset ) const
{
    return QObject::tr( "Item " ) + QString::number( dataset + 1 );
}


/ **
  This methods returns the number of elements to be shown in the
  legend in case fallback texts are used.

  This method is only used when automatic legends are used, because
  manual and first-column legends do not need fallback texts.

  \return the number of fallback texts to use
  * /
uint PieDiagram::numLegendFallbackTexts( KDChartTableDataBase* data ) const
{
    return data->usedCols();
}
*/

/**
  * Auxiliary method returning a point to a given boundary
  * rectangle of the enclosed ellipse and an angle.
  */
QPointF PieDiagram::pointOnCircle( const QRectF& rect, qreal angle )
{
    qreal angleRad = DEGTORAD( angle );
    qreal cosAngle = cos( angleRad );
    qreal sinAngle = -sin( angleRad );
    qreal posX = cosAngle * rect.width() / 2.0;
    qreal posY = sinAngle * rect.height() / 2.0;
    return QPointF( posX + rect.center().x(),
                    posY + rect.center().y() );

}

/*virtual*/
double PieDiagram::valueTotals() const
{
    const int colCount = columnCount();
    double total = 0.0;
    for ( int j = 0; j < colCount; ++j ) {
      total += qAbs(model()->data( model()->index( 0, j, rootIndex() ) ).toDouble());
      //qDebug() << model()->data( model()->index( 0, j, rootIndex() ) ).toDouble();
    }
    return total;
}

/*virtual*/
double PieDiagram::numberOfValuesPerDataset() const
{
    return model() ? model()->columnCount( rootIndex() ) : 0.0;
}

/*virtual*/
double PieDiagram::numberOfGridRings() const
{
    return 1;
}
