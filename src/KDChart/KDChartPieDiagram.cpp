/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include <QDebug>
#include <QPainter>
#include <QStack>

#include "KDChartPieDiagram.h"
#include "KDChartPieDiagram_p.h"

#include "KDChartAttributesModel.h"
#include "KDChartPaintContext.h"
#include "KDChartPieAttributes.h"
#include "KDChartPolarCoordinatePlane_p.h"
#include "KDChartThreeDPieAttributes.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartDataValueAttributes.h"
#include "KDChartNullPaintDevice.h"

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

/**
  * Creates an exact copy of this diagram.
  */
PieDiagram * PieDiagram::clone() const
{
    return new PieDiagram( new Private( *d ) );
}

const QPair<QPointF, QPointF> PieDiagram::calculateDataBoundaries () const
{
    if ( !checkInvariants( true ) || model()->rowCount() < 1 ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );

    const PieAttributes attrs( pieAttributes() );

    QPointF bottomLeft ( QPointF( 0, 0 ) );
    QPointF topRight;
    // If we explode, we need extra space for the slice that has the largest explosion distance.
    if ( attrs.explode() ) {
        const int colCount = columnCount();
        qreal maxExplode = 0.0;
        for( int j = 0; j < colCount; ++j ){
            const PieAttributes columnAttrs( pieAttributes( model()->index( 0, j, rootIndex() ) ) ); // checked
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

void PieDiagram::paint(PaintContext* ctx)
{
    // Painting is a two stage process
    // In the first stage we figure out how much space is needed
    // for text labels.
    // In the second stage, we make use of that information and
    // perform the actual painting.
    QPainter* actualPainter = ctx->painter();
    QRectF textBoundingRect;

    // Use a null paint device and perform the first painting.
    KDChart::NullPaintDevice nullPd(ctx->rectangle().size().toSize());
    QPainter nullPainter(&nullPd);
    ctx->setPainter(&nullPainter);
    paintInternal(ctx, textBoundingRect);

    // Now perform the real painting
    ctx->setPainter(actualPainter);
    paintInternal(ctx, textBoundingRect);
}

void PieDiagram::paintInternal(PaintContext* ctx, QRectF& textBoundingRect)
{
    // note: Not having any data model assigned is no bug
    //       but we can not draw a diagram then either.
    if ( !checkInvariants(true) || model()->rowCount() < 1 )
        return;

    d->reverseMapper.clear();

    const ThreeDPieAttributes threeDAttrs( threeDPieAttributes() );

    const int colCount = columnCount();

    QRectF contentsRect( boundingRect( polarCoordinatePlane() ) );
    contentsRect = ctx->rectangle();

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
    // TODO this can be cached
    qreal maxExplode = 0.0;
    for( int j = 0; j < colCount; ++j ){
        const PieAttributes columnAttrs( pieAttributes( model()->index( 0, j, rootIndex() ) ) ); // checked
        maxExplode = qMax( maxExplode, columnAttrs.explodeFactor() );
    }
    d->size /= ( 1.0 + 1.0 * maxExplode );

    if(d->size < 0.0)
        d->size = 0;

    if(!textBoundingRect.isEmpty() && d->size > 0.0)
    {
        // Find out the distances from every corner of the rectangle with
        // the center.
        QPointF center = ctx->rectangle().center();
        qreal left = qMax(qreal(0.0), center.x() - textBoundingRect.left());
        qreal right = qMax(qreal(0.0), textBoundingRect.right() - center.x());
        qreal top = qMax(qreal(0.0), center.y() - textBoundingRect.top());
        qreal bottom = qMax(qreal(0.0), textBoundingRect.bottom() - center.y());
        // Compute the minimal and maximal distances for horizontal vs vertical
        // the text has.
        qreal xDistanceMax, xDistanceMin, yDistanceMax, yDistanceMin;
        if ( left > right ) {
            xDistanceMax = left;
            xDistanceMin = right;
        } else {
            xDistanceMax = right;
            xDistanceMin = left;
        }
        if ( top > bottom ) {
            yDistanceMax = top;
            yDistanceMin = bottom;
        } else {
            yDistanceMax = bottom;
            yDistanceMin = top;
        }
        // Above we are dealing with the distance from the center what means
        // below we need to make sure to use the half d->size while working
        // with those values.
        qreal availableDistance = d->size/2.0;
        // Now first check what size (if any) the text needs more in any
        // of the corners then what is available for the pie-chart.
        // The resulting diff value is not any longer only related to the
        // distance (d->size/2 cause of calculation from the center) but
        // is now in relation to the whole d->size.
        qreal diff;
        if ( xDistanceMax + xDistanceMin > yDistanceMax + yDistanceMin ) {
            diff = qMax(qreal(0.0), xDistanceMax - availableDistance) + qMax(0.0, xDistanceMin - availableDistance);
        } else {
            diff = qMax(qreal(0.0), yDistanceMax - availableDistance) + qMax(0.0, yDistanceMin - availableDistance);
        }
        if(diff > 0.0) {
            // If that is the case then we need to shrink the size available for the
            // pie-chart by the additional space needed for the text. Those space
            // removed from the pie-chart will then be used by the text and makes
            // sure that the text fits into the contentsRect and is not drawn outside.
            d->size -= qMin(d->size, diff);
        }
    }

    QRectF slicePosition;

    if ( ! threeDAttrs.isEnabled() ) {
        qreal x = ( contentsRect.width() == d->size ) ? 0.0 : ( ( contentsRect.width() - d->size ) / 2.0 );
        qreal y = ( contentsRect.height() == d->size ) ? 0.0 : ( ( contentsRect.height() - d->size ) / 2.0 );
        slicePosition = QRectF( x, y, d->size, d->size );
        slicePosition.translate( contentsRect.left(), contentsRect.top() );
    } else {
        // threeD: width is the maximum possible width; height is 1/2 of that
        qreal sizeFor3DEffect = 0.0;

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

        slicePosition = QRectF( contentsRect.left() + x, contentsRect.top() + y, d->size, height );
    }

    const PolarCoordinatePlane * plane = polarCoordinatePlane();
    const qreal sectorsPerValue = 360.0 / sum;
    qreal currentValue = plane ? plane->startPosition() : 0.0;

    bool atLeastOneValue = false; // guard against completely empty tables
    QVariant vValY;
    for ( int iColumn = 0; iColumn < colCount; ++iColumn ) {
        // is there anything at all at this column?
        bool bOK;
        const qreal cellValue = qAbs( model()->data( model()->index( 0, iColumn, rootIndex() ) ) // checked
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
    // later on (e.g. in findSliceAt()).
    if( ! atLeastOneValue )
        return;

    // Paint from back to front ("painter's algorithm") - first draw the backmost slice,
    // then the slices on the left and right from back to front, then the frontmost one.

    const int backmostSlice = findSliceAt( 90, colCount );
    const int frontmostSlice = findSliceAt( 270, colCount );

    int currentLeftSlice = backmostSlice;
    int currentRightSlice = backmostSlice;

    d->forgetAlreadyPaintedDataValues();

    drawSlice( ctx->painter(), slicePosition, &list, backmostSlice );

    if ( backmostSlice == frontmostSlice ) {
        const int rightmostSlice = findSliceAt( 0, colCount );
        const int leftmostSlice = findSliceAt( 180, colCount );

        if( backmostSlice == leftmostSlice )
            currentLeftSlice = findLeftSlice( currentLeftSlice, colCount );
        if( backmostSlice == rightmostSlice )
            currentRightSlice = findRightSlice( currentRightSlice, colCount );
    }

    while ( currentLeftSlice != frontmostSlice ) {
        if( currentLeftSlice != backmostSlice )
            drawSlice( ctx->painter(), slicePosition, &list, currentLeftSlice );
        currentLeftSlice = findLeftSlice( currentLeftSlice, colCount );
    }

    while ( currentRightSlice != frontmostSlice ) {
        if( currentRightSlice != backmostSlice )
            drawSlice( ctx->painter(), slicePosition, &list, currentRightSlice );
        currentRightSlice = findRightSlice( currentRightSlice, colCount );
    }

    // if the backmost slice is not the frontmost slice, we draw the frontmost one last
    if ( backmostSlice != frontmostSlice || ! threeDPieAttributes().isEnabled() ) {
        drawSlice( ctx->painter(), slicePosition, &list, frontmostSlice );
    }

    d->paintDataValueTextsAndMarkers(  this,  ctx,  list,  false, false, &textBoundingRect );
}

#if defined ( Q_WS_WIN)
#define trunc(x) ((int)(x))
#endif

/**
  Internal method that draws one of the slices in a pie chart.

  \param painter the QPainter to draw in
  \param dataset the dataset to draw the pie for
  \param slice the slice to draw
  \param threeDPieHeight the height of the three dimensional effect
  */
void PieDiagram::drawSlice( QPainter* painter,
        const QRectF &drawPosition,
        DataValueTextInfoList* list,
        uint slice )
{
    // Is there anything to draw at all?
    const qreal angleLen = d->angleLens[ slice ];
    if ( !angleLen ) {
        return;
    }

    const QModelIndex index( model()->index( 0, slice, rootIndex() ) ); // checked
    const PieAttributes attrs( pieAttributes( index ) );
    const ThreeDPieAttributes threeDAttrs( threeDPieAttributes( index ) );

    QRectF adjustedDrawPosition = drawPosition;
    if ( attrs.explode() ) {
        qreal startAngle = d->startAngles[ slice ];
        qreal explodeAngle = ( DEGTORAD( startAngle + angleLen / 2.0 ) );
        qreal explodeDistance = attrs.explodeFactor() * d->size / 2.0;

        adjustedDrawPosition.translate( explodeDistance * cos( explodeAngle ),
                                        explodeDistance * - sin( explodeAngle ) );
    }

    draw3DEffect( painter, adjustedDrawPosition, slice, threeDAttrs );
    drawSliceSurface( painter, adjustedDrawPosition, list, slice );
}

/**
  Internal method that draws the surface of one of the slices in a pie chart.

  \param painter the QPainter to draw in
  \param dataset the dataset to draw the slice for
  \param slice the slice to draw
  */
void PieDiagram::drawSliceSurface( QPainter* painter,
        const QRectF &drawPosition,
        DataValueTextInfoList* list,
        uint slice )
{
    // Is there anything to draw at all?
    qreal angleLen = d->angleLens[ slice ];
    if ( angleLen ) {
        qreal startAngle = d->startAngles[ slice ];

        QModelIndex index( model()->index( 0, slice, rootIndex() ) ); // checked
        const PieAttributes attrs( pieAttributes( index ) );
        const ThreeDPieAttributes threeDAttrs( threeDPieAttributes( index ) );
        painter->setRenderHint ( QPainter::Antialiasing );

        QBrush br = brush( index );
        if( threeDAttrs.isEnabled() ) {
            br = threeDAttrs.threeDBrush( br, drawPosition );
        }
        painter->setBrush( br );

        QPen pen = this->pen( index );
        if ( threeDAttrs.isEnabled() )
            pen.setColor( QColor( 0, 0, 0 ) );
        painter->setPen( pen );

        qreal favoriteTextAngle = 0.0;

        if ( angleLen == 360 ) {
            // full circle, avoid nasty line in the middle
            painter->drawEllipse( drawPosition );

            //Add polygon to Reverse mapper for showing tool tips.
            QPolygonF poly( drawPosition );
            d->reverseMapper.addPolygon( index.row(), index.column(), poly );
        } else {
            // draw the top of this piece
            // Start with getting the points for the arc.
            const int arcPoints = static_cast<int>(trunc( angleLen / granularity() ));
            QPolygonF poly( arcPoints+2 );
            qreal degree=0.0;
            int iPoint = 0;
            bool perfectMatch = false;

            while ( degree <= angleLen ){
                poly[ iPoint ] = pointOnEllipse( drawPosition, startAngle + degree );
                //qDebug() << degree << angleLen << poly[ iPoint ];
                perfectMatch = (degree == angleLen);
                degree += granularity();
                ++iPoint;
            }
            // if necessary add one more point to fill the last small gap
            if( ! perfectMatch ){
                poly[ iPoint ] = pointOnEllipse( drawPosition, startAngle + angleLen );

                // add the center point of the piece
                poly.append( drawPosition.center() );
            }else{
                poly[ iPoint ] = drawPosition.center();
            }
            //find the value and paint it
            //fix value position
            d->reverseMapper.addPolygon( index.row(), index.column(), poly );

            painter->drawPolygon( poly );

            if ( autoRotateLabels() ) {
                const QLineF line( poly.first(), poly[ poly.count() - 2 ] );
                favoriteTextAngle = line.dx() == 0 ? 0.0 : atan( line.dy() / line.dx() );
                favoriteTextAngle = favoriteTextAngle / 2.0 / 3.141592653589793 * 360.0;
            }
        }

        // Position points are calculated relative to the slice.
        // They are calculated as if the slice was 'standing' on its tip and the rim was up,
        // so North is the middle (also highest part) of the rim and South is the tip of the slice.

        const qreal sum = valueTotals();
        const QPointF south = drawPosition.center();
        const QPointF southEast = south;
        const QPointF southWest = south;
        const QPointF north = pointOnEllipse( drawPosition, startAngle + angleLen/2.0 );

        const QPointF northEast = pointOnEllipse( drawPosition, startAngle );
        const QPointF northWest = pointOnEllipse( drawPosition, startAngle + angleLen );
        QPointF center    = (south + north) / 2.0;
        const QPointF east      = (south + northEast) / 2.0;
        const QPointF west      = (south + northWest) / 2.0;

        if ( !list->isEmpty() ) {
            CartesianDiagramDataCompressor::DataValueAttributesList allAttrs =
                d->aggregatedAttrs( this, index, 0 );
            DataValueAttributes dva = allAttrs.value( index );
            const QFontMetrics* fm = d->cachedFontMetrics( dva.textAttributes()
                .calculatedFont( d->plane, KDChartEnums::MeasureOrientationMinimum ), this );

            QRect textRect = fm->boundingRect( d->formatDataValueText( dva, index, list->last().value ) );
            textRect.translated(center.toPoint());
            QPoint textRectCenter = textRect.center();
            qreal newX = center.x() - textRectCenter.x();
            qreal newY = center.y() - textRectCenter.y();
            center.setX( newX );
            center.setY( newY );
        }

        PositionPoints points( center, northWest, north, northEast, east, southEast, south, southWest, west );
        qreal topAngle = startAngle - 90;
        if( topAngle < 0.0 )
            topAngle += 360;

        points.setDegrees(KDChartEnums::PositionEast,      topAngle);
        points.setDegrees(KDChartEnums::PositionNorthEast, topAngle);
        points.setDegrees(KDChartEnums::PositionWest,      topAngle + angleLen);
        points.setDegrees(KDChartEnums::PositionNorthWest, topAngle + angleLen);
        points.setDegrees(KDChartEnums::PositionCenter,    topAngle + angleLen/2.0);
        points.setDegrees(KDChartEnums::PositionNorth,     topAngle + angleLen/2.0);

        //painter->drawText(points.mPositionCenter,QLatin1String("P"));

        d->appendDataValueTextInfoToList(
                this, *list, index, 0,
                points, Position::Center, Position::Center,
                angleLen*sum / 360, favoriteTextAngle );
    }
}

static bool doSpansOverlap( qreal s1Start, qreal s1End, qreal s2Start, qreal s2End )
{
    if ( s1Start < s2Start ) {
        return s1End >= s2Start;
    } else {
        return s1Start <= s2End;
    }
}

static bool doArcsOverlap( qreal a1Start, qreal a1End, qreal a2Start, qreal a2End )
{
    Q_ASSERT( a1Start >= 0 && a1Start <= 360 && a1End >= 0 && a1End <= 360 &&
              a2Start >= 0 && a2Start <= 360 && a2End >= 0 && a2End <= 360 );
    // all of this could probably be done better...
    if ( a1End < a1Start ) {
        a1End += 360;
    }
    if ( a2End < a2Start ) {
        a2End += 360;
    }

    if ( doSpansOverlap( a1Start, a1End, a2Start, a2End ) ) {
        return true;
    }
    if ( a1Start > a2Start ) {
        return doSpansOverlap( a1Start - 360.0, a1End - 360.0, a2Start, a2End );
    } else {
        return doSpansOverlap( a1Start + 360.0, a1End + 360.0, a2Start, a2End );
    }
}

/**
  Internal method that draws the shadow creating the 3D effect of a pie

  \param painter the QPainter to draw in
  \param drawPosition the position to draw at
  \param slice the slice to draw the shadow for
  */
void PieDiagram::draw3DEffect( QPainter* painter,
        const QRectF& drawPosition,
        uint slice,
        const ThreeDPieAttributes& threeDAttrs )
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
    const QBrush brush = this->brush( model()->index( 0, slice, rootIndex() ) ); // checked
    if( threeDAttrs.useShadowColors() ){
        painter->setBrush( QBrush( brush.color().darker() ) );
    }
    else{
        painter->setBrush( brush );
    }

    qreal startAngle = d->startAngles[ slice ];
    qreal endAngle = startAngle + d->angleLens[ slice ];
    // Normalize angles
    while ( startAngle >= 360 )
        startAngle -= 360;
    while ( endAngle >= 360 )
        endAngle -= 360;
    Q_ASSERT( startAngle >= 0 && startAngle <= 360 );
    Q_ASSERT( endAngle >= 0 && endAngle <= 360 );

    // positive pie height: absolute value
    // negative pie height: relative value
    const int depth = threeDAttrs.depth() >= 0.0 ? threeDAttrs.depth() : -threeDAttrs.depth() / 100.0 * drawPosition.height();

    if ( startAngle == endAngle || startAngle == endAngle - 360 ) { // full circle
        draw3dOuterRim( painter, drawPosition, depth, 180, 360 );
    } else {
        if ( doArcsOverlap( startAngle, endAngle, 180, 360 ) ) {
            draw3dOuterRim( painter, drawPosition, depth, startAngle, endAngle );
        }

        if ( startAngle >= 270 || startAngle <= 90 ) {
            draw3dCutSurface( painter, drawPosition, depth, startAngle );
        }
        if ( endAngle >= 90 && endAngle <= 270 ) {
            draw3dCutSurface( painter, drawPosition, depth, endAngle );
        }
    }
}


/**
  Internal method that draws the cut surface of a slice (think of a real pie cut into slices)
  in 3D mode, for surfaces that are facing the observer.

  \param painter the QPainter to draw in
  \param rect the position to draw at
  \param threeDHeight the height of the shadow
  \param angle the angle of the segment
  */
void PieDiagram::draw3dCutSurface( QPainter* painter,
        const QRectF& rect,
        qreal threeDHeight,
        qreal angle )
{
    QPolygonF poly( 4 );
    const QPointF center = rect.center();
    const QPointF circlePoint = pointOnEllipse( rect, angle );
    poly[0] = center;
    poly[1] = circlePoint;
    poly[2] = QPointF( circlePoint.x(), circlePoint.y() + threeDHeight );
    poly[3] = QPointF( center.x(), center.y() + threeDHeight );
    // TODO: add polygon to ReverseMapper
    painter->drawPolygon( poly );
}

/**
  Internal method that draws the outer rim of a slice when the rim is facing the observer.

  \param painter the QPainter to draw in
  \param rect the position to draw at
  \param threeDHeight the height of the shadow
  \param startAngle the starting angle of the segment
  \param endAngle the ending angle of the segment
  */
void PieDiagram::draw3dOuterRim( QPainter* painter,
        const QRectF& rect,
        qreal threeDHeight,
        qreal startAngle,
        qreal endAngle )
{
    // Start with getting the points for the inner arc.
    if ( endAngle < startAngle ) {
        endAngle += 360;
    }
    startAngle = qMax( startAngle, qreal( 180.0 ) );
    endAngle = qMin( endAngle, qreal( 360.0 ) );

    int numHalfPoints = trunc( ( endAngle - startAngle ) / granularity() ) + 1;
    if ( numHalfPoints < 2 ) {
        return;
    }

    QPolygonF poly( numHalfPoints );

    qreal degree = endAngle;
    int iPoint = 0;
    bool perfectMatch = false;
    while ( degree >= startAngle  ){
        poly[ numHalfPoints - iPoint - 1 ] = pointOnEllipse( rect, degree );

        perfectMatch = (degree == startAngle);
        degree -= granularity();
        ++iPoint;
    }
    // if necessary add one more point to fill the last small gap
    if( ! perfectMatch ){
        poly.prepend( pointOnEllipse( rect, startAngle ) );
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

    // TODO: Add polygon to ReverseMapper
    painter->drawPolygon( poly );
}

/**
  Internal method that finds the slice that is located at the position specified by \c angle.

  \param angle the angle at which to search for a slice
  \return the number of the slice found
  */
uint PieDiagram::findSliceAt( qreal angle, int colCount )
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
        return findSliceAt( angle + 360, colCount );
    // otherwise - what ever went wrong - we return 0
    return 0;
}


/**
  Internal method that finds the slice that is located to the left of \c slice.

  \param slice the slice to start the search from
  \return the number of the pie to the left of \c pie
  */
uint PieDiagram::findLeftSlice( uint slice, int colCount )
{
    if ( slice == 0 ) {
        if ( colCount > 1 ) {
            return colCount - 1;
        } else {
            return 0;
        }
    } else {
        return slice - 1;
    }
}


/**
  Internal method that finds the slice that is located to the right of \c slice.

  \param slice the slice to start the search from
  \return the number of the slice to the right of \c slice
  */
uint PieDiagram::findRightSlice( uint slice, int colCount  )
{
    int rightSlice = slice + 1;
    if ( rightSlice == colCount ) {
        rightSlice = 0;
    }
    return rightSlice;
}


/**
  * Auxiliary method returning a point to a given boundary
  * rectangle of the enclosed ellipse and an angle.
  */
QPointF PieDiagram::pointOnEllipse( const QRectF& boundingBox, qreal angle )
{
    qreal angleRad = DEGTORAD( angle );
    qreal cosAngle = cos( angleRad );
    qreal sinAngle = -sin( angleRad );
    qreal posX = cosAngle * boundingBox.width() / 2.0;
    qreal posY = sinAngle * boundingBox.height() / 2.0;
    return QPointF( posX + boundingBox.center().x(),
                    posY + boundingBox.center().y() );

}

/*virtual*/
qreal PieDiagram::valueTotals() const
{
    const int colCount = columnCount();
    qreal total = 0.0;
    Q_ASSERT( model()->rowCount() >= 1 );
    for ( int j = 0; j < colCount; ++j ) {
      total += qAbs(model()->data( model()->index( 0, j, rootIndex() ) ).toDouble()); // checked
    }
    return total;
}

/*virtual*/
qreal PieDiagram::numberOfValuesPerDataset() const
{
    return model() ? model()->columnCount( rootIndex() ) : 0.0;
}

/*virtual*/
qreal PieDiagram::numberOfGridRings() const
{
    return 1;
}
