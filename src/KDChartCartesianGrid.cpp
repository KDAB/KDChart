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

qDebug("minY: %f   maxValueY: %f   dimY.stepWidth: %f",minY,maxValueY,dimY.stepWidth);
        for ( qreal f = minY; f <= maxValueY; f += qAbs( dimY.stepWidth ) ) {
            // PENDING(khz) FIXME: make draving/not drawing of Zero line more sophisticated?:
qDebug("f: %f",f);
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


DataDimensionsList CartesianGrid::calculateGrid(
    const DataDimensionsList& rawDataDimensions ) const
{
    Q_ASSERT_X ( rawDataDimensions.count() == 2, "CartesianGrid::calculateGrid",
                 "Error: calculateGrid() expects a list with exactly two entries." );
    DataDimensionsList l;
    // rule:  Returned list is either empty, or it is providing two
    //        valid dimensions, complete with two non-Zero step widths.
    if( isBoundariesValid( rawDataDimensions ) ) {
        const DataDimension dimX
            = calculateGridXY( rawDataDimensions.first(), Qt::Vertical );
        if( dimX.stepWidth ){
            const DataDimension dimY
                = calculateGridXY( rawDataDimensions.last(), Qt::Horizontal );
            if( dimY.stepWidth ){
                l.append( dimX );
                l.append( dimY );
            }
        }
    }
    return l;
}


void adjustUpperLowerRange( qreal& start, qreal& end, qreal stepWidth )
{
    if ( fmod( start, stepWidth ) != 0.0 )
        start = stepWidth * _trunc( start / stepWidth );
    if ( fmod( end, stepWidth ) != 0.0 )
        end = stepWidth * (_trunc( end / stepWidth ) + 1.0);
}
double fastPow10( int x )
{
    double res = 1.0;
    if( 0 <= x ){
        for( int i = 1; i <= x; ++i )
            res *= 10.0;
    }else{
        for( int i = -1; i >= x; --i )
            res /= 10.0;
    }
    return res;
}


DataDimension CartesianGrid::calculateGridXY(
    const DataDimension& rawDataDimension,
    Qt::Orientation orientation ) const
{
    DataDimension dim( rawDataDimension );
    if( dim.isCalculated ){
        if( dim.stepWidth == 0.0 ){
            QList<qreal> granularities;
            switch( dim.sequence ){
                case KDChartEnums::GranularitySequence_10_20:
                    granularities << 1.0 << 2.0;
                    break;
                case KDChartEnums::GranularitySequence_10_50:
                    granularities << 1.0 << 5.0;
                    break;
                case KDChartEnums::GranularitySequence_25_50:
                    granularities << 2.5 << 5.0;
                    break;
                case KDChartEnums::GranularitySequenceIrregular:
                    granularities << 1.0 << 2.0 << 2.5 << 5.0;
                    break;
                default:
                    break;
            }
//qDebug("dim.start: %f   dim.end: %f", dim.start, dim.end);
            dim.stepWidth
                = calculateStepWidth(
                    dim.start, dim.end,
                    granularities,
                    orientation );
        }
        // if needed, adjust start/end to match the step width:
        adjustUpperLowerRange( dim.start, dim.end, dim.stepWidth );
    }else{
        dim.stepWidth = 1.0;
    }
    return dim;
}


void calculateSteps(
    qreal start_, qreal end_, const QList<qreal>& list, qreal minSteps,
    int power,
    qreal& steps, qreal& stepWidth )
{
    steps = 0.0;
    for( int i=0; i<list.count(); ++i){
        const qreal testStepWidth = list.at( i ) * fastPow10( power );
        qreal start = qMin( start_, end_ );
        qreal end   = qMax( start_, end_ );
//qDebug("start: %f   end: %f", start, end);
        adjustUpperLowerRange( start, end, testStepWidth );
//qDebug("start: %f   end: %f\n---------------------------------", start, end);

        const qreal testSteps = end/testStepWidth - start/testStepWidth;

        if( (steps == 0.0) || (minSteps <= testSteps && testSteps < steps) ){
            steps = testSteps;
            stepWidth = testStepWidth;
        }
    }
}


qreal CartesianGrid::calculateStepWidth(
    qreal start_, qreal end_,
    const QList<qreal>& granularities,
    Qt::Orientation orientation ) const
{
    Q_ASSERT_X ( granularities.count(), "CartesianGrid::calculateStepWidth",
                "Error: The list of GranularitySequence values is empty." );
    QList<qreal> list( granularities );
    qSort( list );
    const qreal start = qMin( start_, end_);
    const qreal end   = qMax( start_, end_);

    // For now, we set the minimal number of steps to two,
    // we later could (should?) make thjis configurable by the user.
    const int minSteps = 2;

    qreal steps,       steps0,       steps1;
    qreal stepWidth,   stepWidth0,   stepWidth1;
    // find out in which direction we will find a matching step width:
    calculateSteps( start, end, list, minSteps, 0, steps0, stepWidth0 );
    calculateSteps( start, end, list, minSteps, 1, steps1, stepWidth1 );
    if( steps0 >= minSteps && steps1 <= minSteps ){
        // That was quick success: we found it already!
        if( steps1 == minSteps ){
            steps     = steps1;
            stepWidth = stepWidth1;
        }else{
            steps     = steps0;
            stepWidth = stepWidth0;
        }
    }else if( steps0 >= minSteps && steps1 >= minSteps ){
        // both values are at least minSteps --> increase the power-of-ten
        int power = 1;
        do{
            steps     = steps1;
            stepWidth = stepWidth1;
            ++power;
            calculateSteps( start, end, list, minSteps, power, steps1, stepWidth1 );
        }while( steps1 >= minSteps );
    }else{
        // both values are at smaller thas minSteps --> decrease the power-of-ten
        int power = 0;
        do{
            steps     = steps1;
            stepWidth = stepWidth1;
            --power;
            calculateSteps( start, end, list, minSteps, power, steps1, stepWidth1 );
        }while( steps1 < minSteps );
    }
qDebug("stepWidth: %f   steps: %f", stepWidth, steps);
    return stepWidth;
}
