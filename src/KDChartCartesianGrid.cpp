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

#include "KDChartCartesianGrid.h"
#include "KDChartPaintContext.h"

#include <cmath>

#include <QPainter>


static qreal _trunc( qreal v )
{
    return (( v > 0.0 ) ? floor( v ) : ceil(  v ));
}


using namespace KDChart;


DataDimension CartesianGrid::calculateGridXY( const DataDimension& rawDataDimension, bool isX )
{
    DataDimension dim( rawDataDimension );
    if( dim.isCalculated ){
        // PENDING(khz) FIXME: calculate the step width in a *real* way:
        dim.stepWidth = 1.0;

        // if needed, adjust start/end to match the step width:
        if ( fmod( dim.start, dim.stepWidth ) != 0.0 )
            dim.start = dim.stepWidth * _trunc( dim.start / dim.stepWidth );
        if ( fmod( dim.end, dim.stepWidth ) != 0.0 )
            dim.end = dim.stepWidth * (_trunc( dim.end / dim.stepWidth ) + 1.0);
    }else{
        dim.stepWidth = 1.0;
    }
    return dim;
}


DataDimensionsList CartesianGrid::calculateGrid( const DataDimensionsList& rawDataDimensions )
{
    Q_ASSERT_X ( rawDataDimensions.count() == 2, "CartesianGrid::calculateGrid",
                 "Error: calculateGrid() expects a list with exactly two entries." );
    DataDimensionsList l;
    if( isBoundariesValid( rawDataDimensions ) ) {
        const DataDimension dimX = calculateGridXY( rawDataDimensions.first(), true );
        if( dimX.stepWidth ){
            const DataDimension dimY = calculateGridXY( rawDataDimensions.last(), false );
            if( dimY.stepWidth ){
                l.append( dimX );
                l.append( dimY );
            }
        }
    }
    // rule:  Returned list is either empty, or it is providing two
    //        valid dimensions, complete with two non-Zero step widths.
    return l;
}


void CartesianGrid::drawGrid( PaintContext* context )
{
    CartesianCoordinatePlane* plane = dynamic_cast<CartesianCoordinatePlane*>(context->coordinatePlane());
    Q_ASSERT_X ( plane, "CartesianGrid::drawGrid",
                 "Bad function call: PaintContext::coodinatePlane() NOT a cartesian plane." );

    if ( !gridAttributes.isGridVisible() ) return;

    // important: Need to update the calculated mData,
    //            before we may use it!
    updateData( plane );

    // test for programming errors: critical
    Q_ASSERT_X ( mData.count() == 2, "CartesianGrid::drawGrid",
                 "Error: updateData did not return exactly two dimensions." );
    const DataDimension& dimX = mData.first();
    const DataDimension& dimY = mData.last();
    Q_ASSERT_X ( dimX.stepWidth, "CartesianGrid::drawGrid",
                 "Error: updateData returned a Zero step width for the X grid." );
    Q_ASSERT_X ( dimY.stepWidth, "CartesianGrid::drawGrid",
                 "Error: updateData returned a Zero step width for the Y grid." );

    // test for invalid boundaries: non-critical
    if( !isBoundariesValid( mData ) ) return;

    const qreal numberOfUnitLinesX = qAbs( dimX.distance() / dimX.stepWidth );
    const qreal numberOfUnitLinesY = qAbs( dimY.distance() / dimY.stepWidth );
//  qDebug("numberOfUnitLinesX: %f    numberOfUnitLinesY: %f",numberOfUnitLinesX,numberOfUnitLinesY);

    // do not draw a Zero size grid (and do not divide by Zero)
    if( numberOfUnitLinesX <= 0.0 || numberOfUnitLinesY <= 0.0 ) return;

    const QPointF p1 = plane->translate( QPointF(dimX.start, dimY.start) );
    const QPointF p2 = plane->translate( QPointF(dimX.end, dimX.end) );

    const qreal screenRangeX = qAbs ( p1.x() - p2.x() );
    const qreal screenRangeY = qAbs ( p1.y() - p2.y() );

    const qreal MinimumPixelsBetweenLines = 10.0;

//    qreal unitFactorX = 1.0;
//    qreal unitFactorY = 1.0;
    bool drawUnitLinesX = (screenRangeX / numberOfUnitLinesX > MinimumPixelsBetweenLines);
    bool drawUnitLinesY = (screenRangeY / numberOfUnitLinesY > MinimumPixelsBetweenLines);
/*
    while ( !drawUnitLinesX ) {
        unitFactorX *= 10.0;
        drawUnitLinesX = screenRangeX / (numberOfUnitLinesX / unitFactorX) > MinimumPixelsBetweenLines;
    }
    while ( !drawUnitLinesY ) {
        unitFactorY *= 10.0;
        drawUnitLinesY = screenRangeY / (numberOfUnitLinesY / unitFactorY) > MinimumPixelsBetweenLines;
    }
*/
    bool drawHalfLinesX = screenRangeX / (numberOfUnitLinesX * 2.0) > MinimumPixelsBetweenLines && gridAttributes.isSubGridVisible();
    bool drawHalfLinesY = screenRangeY / (numberOfUnitLinesY * 2.0) > MinimumPixelsBetweenLines && gridAttributes.isSubGridVisible();
    bool drawFourthLinesX = screenRangeX / (numberOfUnitLinesX * 4.0) > MinimumPixelsBetweenLines && gridAttributes.isSubGridVisible();
    bool drawFourthLinesY = screenRangeY / (numberOfUnitLinesY * 4.0) > MinimumPixelsBetweenLines && gridAttributes.isSubGridVisible();

    const qreal minValueX = qMin( dimX.start, dimX.end );
    const qreal maxValueX = qMax( dimX.start, dimX.end );
    const qreal minValueY = qMin( dimY.start, dimY.end );
    const qreal maxValueY = qMax( dimY.start, dimY.end );

    if ( drawFourthLinesX ) {
        context->painter()->setPen( gridAttributes.subGridPen() );
        for ( qreal f = minValueX; f <= maxValueX; f += 0.25 ) {
            QPointF topPoint( f, maxValueY );
            QPointF bottomPoint( f, minValueY );
            topPoint = plane->translate( topPoint );
            bottomPoint = plane->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawFourthLinesY ) {
        context->painter()->setPen( gridAttributes.subGridPen() );
        for ( qreal f = minValueY; f <= maxValueY; f += 0.25 ) {
            QPointF leftPoint( minValueX, f );
            QPointF rightPoint( maxValueX, f );
            leftPoint = plane->translate( leftPoint );
            rightPoint = plane->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    if ( drawHalfLinesX ) {
        context->painter()->setPen( gridAttributes.subGridPen() );
        for ( qreal f = minValueX; f <= maxValueX; f += 0.5 ) {
            QPointF topPoint( f, maxValueY );
            QPointF bottomPoint( f, minValueY );
            topPoint = plane->translate( topPoint );
            bottomPoint = plane->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawHalfLinesY ) {
        context->painter()->setPen( gridAttributes.subGridPen() );
        for ( qreal f = minValueY; f <= maxValueY; f += 0.5 ) {
            QPointF leftPoint( minValueX, f );
            QPointF rightPoint( maxValueX, f );
            leftPoint = plane->translate( leftPoint );
            rightPoint = plane->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    const bool drawXZeroLineX
        = dimX.isCalculated &&
        gridAttributes.zeroLinePen().style() != Qt::NoPen;

    const bool drawZeroLineY
        = gridAttributes.zeroLinePen().style() != Qt::NoPen;

    if ( drawUnitLinesX || drawXZeroLineX ) {
        if ( drawUnitLinesX )
            context->painter()->setPen( gridAttributes.gridPen() );
        const qreal minX = dimX.start;

        for ( qreal f = minX; f <= maxValueX; f += qAbs( dimX.stepWidth ) ) {
            // PENDING(khz) FIXME: make draving/not drawing of Zero line more sophisticated?:
            const bool zeroLineHere = drawXZeroLineX && (f == 0.0);
            if ( drawUnitLinesX || zeroLineHere ){
                QPointF topPoint( f, maxValueY );
                QPointF bottomPoint( f, minValueY );
                topPoint = plane->translate( topPoint );
                bottomPoint = plane->translate( bottomPoint );
                if ( zeroLineHere )
                    context->painter()->setPen( gridAttributes.zeroLinePen() );
                context->painter()->drawLine( topPoint, bottomPoint );
                if ( zeroLineHere )
                    context->painter()->setPen( gridAttributes.gridPen() );
            }
        }
    }
    if ( drawUnitLinesY || drawZeroLineY ) {
        if ( drawUnitLinesY )
            context->painter()->setPen( gridAttributes.gridPen() );
        const qreal minY = dimY.start;

//qDebug("minY: %f   maxValueY: %f   unitFactorY: %f",minY,maxValueY,unitFactorY);
        for ( qreal f = minY; f <= maxValueY; f += qAbs( dimY.stepWidth ) ) {
            // PENDING(khz) FIXME: make draving/not drawing of Zero line more sophisticated?:
//qDebug("f: %f",f);
            const bool zeroLineHere = (f == 0.0);
            if ( drawUnitLinesY || zeroLineHere ){
                QPointF leftPoint( minValueX, f );
                QPointF rightPoint( maxValueX, f );
                leftPoint = plane->translate( leftPoint );
                rightPoint = plane->translate( rightPoint );
                if ( zeroLineHere )
                    context->painter()->setPen( gridAttributes.zeroLinePen() );
                context->painter()->drawLine( leftPoint, rightPoint );
                if ( zeroLineHere )
                    context->painter()->setPen( gridAttributes.gridPen() );
            }
        }
    }
}




/*
void CartesianGrid::paintGrid( PaintContext* ctx )
{
    // FIXME accumulate over all diagrams
    const GridAttributes a = gridAttributes();
    AbstractCartesianDiagram* dgr = dynamic_cast<AbstractCartesianDiagram*> (plane->diagrams().first() );
    Q_ASSERT ( dgr ); // only cartesian diagrams are allowed here

    const int numberOfAbscissaSegments = dgr->numberOfAbscissaSegments();
    const int numberOfOrdinateSegments = dgr->numberOfOrdinateSegments();
    QRectF diagramRect = d->coordinateTransformation.diagramRect;
    bool hasVisibleAbscissa = ( d->coordinateTransformation.unitVectorX
                                * ( diagramRect.left() - diagramRect.right() ) ) <= 0;
    bool hasVisibleOrdinate = ( d->coordinateTransformation.unitVectorY
                                * ( diagramRect.top() - diagramRect.bottom () ) ) <= 0;


    // the axes and rulers to draw, this can be cached:
    QVector<QLineF> axes;
    QVector<QLineF> rulers;

    // draw the abscissa and ordinate, if they are in the visible range:
    if ( hasVisibleAbscissa )
    {
        QLineF l2r ( translate ( QPointF ( diagramRect.left(), 0 ) ),
                     translate ( QPointF ( diagramRect.right(), 0 ) ) );
        axes.append ( l2r );
    }

    if ( hasVisibleOrdinate )
    {
        QLineF t2b ( translate ( QPointF ( 0, diagramRect.top() ) ),
                     translate ( QPointF ( 0, diagramRect.bottom() ) ) );
        axes.append ( t2b );
    }

    double step = diagramRect.width() / ( numberOfAbscissaSegments - 1 );
    double x = diagramRect.left();
    for ( int i = 0; i < numberOfAbscissaSegments; ++i )
    {
        QLineF l ( translate ( QPointF ( x, diagramRect.top() ) ),
                   translate ( QPointF ( x,  diagramRect.bottom() ) ) );
        rulers.append (l );
        x += step;
    }

    step = diagramRect.height() / ( numberOfOrdinateSegments - 1 );
    double y = diagramRect.top();
    for ( int i = 0; i < numberOfOrdinateSegments; ++i )
    {
        QLineF l ( translate ( QPointF ( diagramRect.left(), y ) ),
                   translate ( QPointF ( diagramRect.right(), y ) ) );
        rulers.append (l );
        y += step;
    }

    if ( a.isSubGridVisible() ) {
        ctx->painter()->setPen ( a.subGridPen() );
        ctx->painter()->drawLines ( rulers );
    }
    if ( a.isGridVisible() ) {
        ctx->painter()->setPen ( a.gridPen() );
        ctx->painter()->drawLines ( axes );
    }
}
*/

