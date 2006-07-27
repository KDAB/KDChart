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

#include <QDebug>
#include <QPainter>
#include <QStack>

#include <QtXml/QDomDocumentFragment>

#include "KDChartAttributesModel.h"
#include "KDChartPaintContext.h"
#include "KDChartPieDiagram.h"
#include "KDChartPieDiagram_p.h"
#include "KDChartPieAttributes.h"
#include "KDChartThreeDPieAttributes.h"
#include "KDChartPainterSaver_p.h"

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
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: PieDiagram * PieDiagram::clone() const" );
    return (PieDiagram*)0xdeadbeef;
}

QDomDocumentFragment PieDiagram::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment PieDiagram::toXML() const" );
    return QDomDocumentFragment();
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
    if ( !checkInvariants() ) return;
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

        ctx->painter()->drawPie( pieRect, ( int ) ((-startAngle + 90 ) * 16), ( int ) (-spanAngle * 16) );
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


/*
  Paints the actual data area.

  \param painter the QPainter onto which the chart should be painted
  \param data the data that will be displayed as a chart
  \param paint2nd specifies whether the main chart or the additional chart is to be drawn now
  \param regions a pointer to a list of regions that will be filled
  with regions representing the data segments, if not null
  */
//void PieDiagram::paintData( QPainter* painter,
//        KDChartTableDataBase* data,
//        bool paint2nd,
//        KDChartDataRegionList* regions )

void PieDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants() ) return;

    const PieAttributes attrs( pieAttributes( model()->index( 0, 0, rootIndex() ) ) );
    const ThreeDPieAttributes threeDAttrs( threeDPieAttributes( model()->index( 0, 0, rootIndex() ) ) );

    const int colCount = columnCount();

    QRectF contentsRect( buildReferenceRect( polarCoordinatePlane() ) );
//    contentsRect = geometry();
//qDebug() << contentsRect;
    if( contentsRect.isEmpty() )
        return;

    DataValueTextInfoList list;
    const qreal startAngle = attrs.startPosition();
    const qreal sum = valueTotals();

    if( sum == 0.0 ) //nothing to draw
        return;

    d->startAngles.resize( colCount );
    d->angleLens.resize( colCount );

    // compute position
    d->size = qMin( contentsRect.width(), contentsRect.height() ); // initial size

    // if the pies explode, we need to give them additional space =>
    // make the basic size smaller
    if ( attrs.explode() ) {
        const int colCount = columnCount();
        qreal maxExplode = 0.0;
        for( int j = 0; j < colCount; ++j ){
            const PieAttributes columnAttrs( pieAttributes( model()->index( 0, j, rootIndex() ) ) );
            maxExplode = qMax( maxExplode, columnAttrs.explodeFactor() );
        }
        d->size /= ( 1.0 + 2.0 * maxExplode );
    }



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
        if ( threeDAttrs.height() >= 0.0 ) {
            // positive pie height: absolute value
            sizeFor3DEffect = threeDAttrs.height();
            height = d->size - sizeFor3DEffect;
        } else {
            // negative pie height: relative value
            sizeFor3DEffect = - threeDAttrs.height() / 100.0 * height;
            height = d->size - sizeFor3DEffect;
        }
        qreal y = ( contentsRect.height() == height ) ? 0.0 : ( ( contentsRect.height() - height - sizeFor3DEffect ) / 2.0 );

        d->position = QRectF( contentsRect.left() + x, contentsRect.top() + y,
                d->size, height );
        //  d->position.moveBy( contentsRect.left(), contentsRect.top() );
    }

    const qreal sectorsPerValue = 5760.0 / sum; // 5760 == 16*360, number of sections in Qt circle
    qreal currentValue = startAngle * 16.0;

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
        //qDebug() << d->startAngles[ iColumn ] << d->angleLens[ iColumn ];

        currentValue = d->startAngles[ iColumn ] + d->angleLens[ iColumn ];
    }

    // If there was no value at all, bail out, to avoid endless loops
    // later on (e.g. in findPieAt()).
    if( ! atLeastOneValue )
        return;


    // Find the backmost pie which is at +90° and needs to be drawn
    // first
    int backmostpie = findPieAt( 90 * 16, colCount );
    // Find the frontmost pie (at -90°/+270°) that should be drawn last
    int frontmostpie = findPieAt( 270 * 16, colCount );
    // and put the backmost pie on the TODO stack to initialize it,
    // but only if it is not the frontmostpie
    QStack < int > todostack;
    if ( backmostpie != frontmostpie )
        todostack.push( backmostpie );
    else {
        // Otherwise, try to find something else
        int leftOfCurrent = findLeftPie( backmostpie, colCount );
        if ( leftOfCurrent != frontmostpie ) {
            todostack.push( leftOfCurrent );
        } else {
            int rightOfCurrent = findRightPie( backmostpie, colCount );
            if ( rightOfCurrent != frontmostpie ) {
                todostack.push( rightOfCurrent );
            }
        }
        // If we get here, there was nothing else, and we will bail
        // out of the while loop below.
    }

    // The list with pies that have already been drawn

    QList < int > donelist;

    // Draw pies until the todostack is empty or only the frontmost
    // pie is there
    while ( !todostack.isEmpty() &&
            !( ( todostack.count() == 1 ) &&
                ( ( todostack.top() == frontmostpie ) ) ) ) {
        // The while loop cannot be cancelled if frontmostpie is on
        // top of the stack, but this is also backmostpie (can happen
        // when one of the pies covers more than 1/2 of the circle. In
        // this case, we need to find something else to put on the
        // stack to get things going.

        // take one pie from the stack
        int currentpie = todostack.pop();
        // if this pie was already drawn, ignore it
        if ( donelist.contains( currentpie ) )
            continue;

        // If this pie is the frontmost pie, put it back, but at the
        // second position (otherwise, there would be an endless
        // loop). If this pie is the frontmost pie, there must be at
        // least one other pie, otherwise the loop would already have
        // been terminated by the loop condition.
        if ( currentpie == frontmostpie ) {
            Q_ASSERT( !todostack.isEmpty() );
            // QValueStack::exchange() would be nice here...
            int secondpie = todostack.pop();
            if ( currentpie == secondpie )
                // no need to have the second pie twice on the stack,
                // forget about one instance and take the third
                // instead
                if ( todostack.isEmpty() )
                    break; // done anyway
                else
                    secondpie = todostack.pop();
            todostack.push( currentpie );
            todostack.push( secondpie );
            continue;
        }
        // When we get here, we can just draw the pie and proceed.
        drawOnePie( ctx->painter(), 0, currentpie, sizeFor3DEffect );

        // Mark the pie just drawn as done.
        donelist.append( currentpie );

        // Now take the pie to the left and to the right, check
        // whether these have not been painted already, and put them
        // on the stack.
        int leftOfCurrent = findLeftPie( currentpie, colCount );
        if ( ! donelist.contains( leftOfCurrent ) )
            todostack.push( leftOfCurrent );
        int rightOfCurrent = findRightPie( currentpie, colCount );
        if ( ! donelist.contains( rightOfCurrent ) )
            todostack.push( rightOfCurrent );
    }

    // now only the frontmost pie is left to draw
    drawOnePie( ctx->painter(), 0, frontmostpie, sizeFor3DEffect );
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
        qreal threeDPieHeight )
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
        const PieAttributes attrs( pieAttributes( model()->index( 0, pie, rootIndex() ) ) );
        const ThreeDPieAttributes threeDAttrs( threeDPieAttributes( model()->index( 0, pie, rootIndex() ) ) );

        QRectF drawPosition( d->position );

        if ( attrs.explode() ) {
            qreal explodeAngle = ( startAngle + angleLen / 2.0 ) / 16.0;
            qreal explodeAngleRad = DEGTORAD( explodeAngle );
            qreal cosAngle = cos( explodeAngleRad );
            qreal sinAngle = -sin( explodeAngleRad );
            qreal explodeX = attrs.explodeFactor() * d->size * cosAngle;
            qreal explodeY = attrs.explodeFactor() * d->size * sinAngle;
            drawPosition.translate( explodeX, explodeY );
        }else{
            drawPosition = d->position;
        }
        // The 3D effect needs to be drawn first because it could
        // otherwise partly hide the pie itself.
        draw3DEffect( painter,
            drawPosition, dataset, pie,
            threeDAttrs,
            attrs.explode() );

        const QBrush brush = qVariantValue<QBrush>( attributesModel()->headerData( pie, Qt::Vertical, KDChart::DatasetBrushRole ) );
        const QPen pen = qVariantValue<QPen>( attributesModel()->headerData( pie, Qt::Vertical, KDChart::DatasetPenRole ) );
        PainterSaver painterSaver( painter );
        painter->setRenderHint ( QPainter::Antialiasing );
        painter->setBrush( brush );
        painter->setPen( pen );

        if ( angleLen == 5760 ) {
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
                    = pointOnCircle( drawPosition,  720 );
                datReg->points[ KDChartEnums::PosTopCenter ]
                    = pointOnCircle( drawPosition, 1440 );
                datReg->points[ KDChartEnums::PosTopLeft ]
                    = pointOnCircle( drawPosition, 2160 );
                datReg->points[ KDChartEnums::PosCenterLeft ]
                    = pointOnCircle( drawPosition, 2880 );
                datReg->points[ KDChartEnums::PosBottomLeft ]
                    = pointOnCircle( drawPosition, 3600 );
                datReg->points[ KDChartEnums::PosBottomCenter ]
                    = pointOnCircle( drawPosition, 4320 );
                datReg->points[ KDChartEnums::PosBottomRight ]
                    = pointOnCircle( drawPosition, 5040 );
                datReg->startAngle = 2880;
                datReg->angleLen   = 5760;
                regions->append( datReg );
            }
*/
        } else {
            // draw the top of this piece
            // Start with getting the points for the arc.
            const qreal stepWidth = 1.0;

            const int arcPoints = static_cast<int>(trunc( angleLen / stepWidth ));
            QPolygonF poly( arcPoints+2 );
            qreal degree=0.0;
            int iPoint = 0;
            bool perfectMatch = false;
            while ( degree <= angleLen ){
                poly[ iPoint ] = pointOnCircle( drawPosition, startAngle + degree );
//qDebug() << degree << angleLen << poly[ iPoint ];
                perfectMatch = (degree == angleLen);
                degree += stepWidth;
                ++iPoint;
            }
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
            painter->drawPolygon( poly );


//if( bHelp ){
//              painter->drawPolyline( collect );
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
        //const QBrush brush = qVariantValue<QBrush>( attributesModel()->headerData( pie, Qt::Vertical, KDChart::DatasetBrushRole ) );
        const QPen pen = qVariantValue<QPen>( attributesModel()->headerData( pie, Qt::Vertical, KDChart::DatasetPenRole ) );
        painter->setBrush( QBrush( pen.color() ) );
    }
    //painter->setBrush( QBrush( threeDAttrs.dataShadow1Color( pie ),
    //            params()->shadowPattern() ) );

    qreal startAngle = d->startAngles[ pie ];
    qreal endAngle = startAngle + d->angleLens[ pie ];
    // Normalize angles
    while ( startAngle >= 5760 )
        startAngle -= 5760;
    while ( endAngle >= 5760 )
        endAngle -= 5760;
    Q_ASSERT( startAngle >= 0 && startAngle <= 360 * 16 );
    Q_ASSERT( endAngle >= 0 && endAngle <= 360 * 16 );

    //int centerY = drawPosition.center().y();

    if ( startAngle == endAngle ||
            startAngle == endAngle - 5760 ) { // full circle
        drawArcEffectSegment( painter, drawPosition, 
                threeDAttrs.height(), 2880, 5760 );
    } else if ( startAngle <= 90 * 16 ) {
        if ( endAngle <= 90 * 16 ) {
            if ( startAngle <= endAngle ) {
                /// starts and ends in first quadrant, less than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                                           threeDAttrs.height(), startAngle );
            } else {
                /// starts and ends in first quadrant, more than 3/4
                drawStraightEffectSegment( painter, drawPosition,
                                           threeDAttrs.height(), startAngle );
                drawArcEffectSegment( painter, drawPosition,  
                                      threeDAttrs.height(), 2880, 5760 );
            }
        } else if ( endAngle <= 180 * 16 ) {
            /// starts in first quadrant, ends in second quadrant,
            /// less than 1/2
            drawStraightEffectSegment( painter, drawPosition, 
                                       threeDAttrs.height(), startAngle );
            drawStraightEffectSegment( painter, drawPosition, 
                                       threeDAttrs.height(), endAngle );
        } else if ( endAngle <= 270 * 16 ) {
            /// starts in first quadrant, ends in third quadrant
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle );
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), endAngle );
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), 2880, endAngle );
        } else { // 270*16 < endAngle < 360*16
            /// starts in first quadrant, ends in fourth quadrant,
            /// more than 3/4
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle );
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), 2880, endAngle );
        }
    } else if ( startAngle <= 180 * 16 ) {
        if ( endAngle <= 90 * 16 ) {
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), 2880, 5760 );
        } else if ( endAngle <= 180 * 16 ) {
            if ( startAngle <= endAngle ) {
                /// starts in second quadrant, ends in second
                /// quadrant, less than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                        threeDAttrs.height(), endAngle );
            } else {
                /// starts in second quadrant, ends in second
                /// quadrant, more than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                        threeDAttrs.height(), endAngle );
                drawArcEffectSegment( painter, drawPosition,  
                        threeDAttrs.height(), 2880, 5760 );
            }
        } else if ( endAngle <= 270 * 16 ) {
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), endAngle );
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), 2880, endAngle );
        } else { // 270*16 < endAngle < 360*16
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), 2880, endAngle );
        }
    } else if ( startAngle <= 270 * 16 ) {
        if ( endAngle <= 90 * 16 ) {
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle, 5760 );
        } else if ( endAngle <= 180 * 16 ) {
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), endAngle );
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle, 5760 );
        } else if ( endAngle <= 270 * 16 ) {
            if ( startAngle <= endAngle ) {
                /// starts in third quadrant, ends in third quadrant,
                /// less than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                        threeDAttrs.height(), endAngle );
                drawArcEffectSegment( painter, drawPosition,  
                        threeDAttrs.height(), startAngle, endAngle );
            } else {
                /// starts in third quadrant, ends in third quadrant,
                /// more than 3/4
                drawStraightEffectSegment( painter, drawPosition,
                        threeDAttrs.height(), endAngle );
                drawArcEffectSegment( painter, drawPosition,  
                        threeDAttrs.height(), 2880, endAngle );
                drawArcEffectSegment( painter, drawPosition,  
                        threeDAttrs.height(), startAngle, 5760 );
            }
        } else { // 270*16 < endAngle < 360*16
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle, endAngle );
        }
    } else { // 270*16 < startAngle < 360*16
        if ( endAngle <= 90 * 16 ) {
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle );
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle, 5760 );
        } else if ( endAngle <= 180 * 16 ) {
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle );
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), endAngle );
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle, 5760 );
        } else if ( endAngle <= 270 * 16 ) {
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle );
            drawStraightEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), endAngle );
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), 2880, endAngle );
            drawArcEffectSegment( painter, drawPosition,  
                    threeDAttrs.height(), startAngle, 5760 );
        } else { // 270*16 < endAngle < 360*16
            if ( startAngle <= endAngle ) {
                /// starts in fourth quadrant, ends in fourth
                /// quadrant, less than 1/4
                drawStraightEffectSegment( painter, drawPosition,
                        threeDAttrs.height(), startAngle );
                drawArcEffectSegment( painter, drawPosition,  
                        threeDAttrs.height(), startAngle, endAngle );
            } else {
                /// starts in fourth quadrant, ends in fourth
                /// quadrant, more than 3/4
                drawStraightEffectSegment( painter, drawPosition, 
                        threeDAttrs.height(), startAngle );
                drawArcEffectSegment( painter, drawPosition,  
                        threeDAttrs.height(), startAngle, 5760 );
                drawArcEffectSegment( painter, drawPosition,  
                        threeDAttrs.height(), 2880, endAngle );
            }
        }
    }
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
        qreal endAngle )
{
    // Start with getting the points for the inner arc.
    const qreal startA = qMin(startAngle, endAngle);
    const qreal endA   = qMax(startAngle, endAngle);

    const qreal stepWidth = 1.0;

    int numHalfPoints = static_cast<int>(trunc( (endA-startA) / stepWidth )) + 1;

    QPolygonF poly( numHalfPoints );

    qreal degree=endA;
    int iPoint = 0;
    bool perfectMatch = false;
    while ( degree >= startA ){
        poly[ numHalfPoints - iPoint - 1 ] = pointOnCircle( rect, degree );

        perfectMatch = (degree == startA);
        degree -= stepWidth;
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
    //return findPieAt( angle + 5760, colCount );
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
    return model()->columnCount( rootIndex() );
}

/*virtual*/
double PieDiagram::numberOfGridRings() const
{
    return 1;
}
