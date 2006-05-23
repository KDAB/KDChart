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

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartPaintContext.h"
#include <QDebug>
#include <QPainter>

using namespace KDChart;
AbstractCartesianDiagram::Private::Private()
    : referenceDiagram( 0 )
{
}

AbstractCartesianDiagram::Private::~Private()
{

}

void AbstractCartesianDiagram::init()
{
}

#define d d_func()

AbstractCartesianDiagram::AbstractCartesianDiagram ( CartesianCoordinatePlane* parent )
    : AbstractDiagram ( new Private(), parent )
{
}

void AbstractCartesianDiagram::addAxis( CartesianAxis *axis )
{
    d->axesList.append( axis );
    // Hack alert. Internally we pretend that the axes are owned by the plane.
    axis->setParent( coordinatePlane() );
}

KDChart::CartesianAxisList AbstractCartesianDiagram::axes( ) const
{
    return d->axesList;
}

void KDChart::AbstractCartesianDiagram::setCoordinatePlane( AbstractCoordinatePlane* plane )
{
  AbstractDiagram::setCoordinatePlane(plane);
  // Hack alert. Internally we pretend that the axes are owned by the plane.  
  foreach( CartesianAxis* axis, d->axesList ) {
    axis->setParent(plane);
    axis->show();
  }
}

void AbstractCartesianDiagram::setReferenceDiagram( AbstractCartesianDiagram* diagram, const QPointF& offset )
{
    d->referenceDiagram = diagram;
    d->referenceDiagramOffset = offset;
}

AbstractCartesianDiagram* AbstractCartesianDiagram::referenceDiagram() const
{
    return d->referenceDiagram;
}

QPointF AbstractCartesianDiagram::referenceDiagramOffset() const
{
    return d->referenceDiagramOffset;
}

GridAttributes* AbstractCartesianDiagram::gridAttributes()
{
    return &(d->gridAttributes);
}

const GridAttributes* AbstractCartesianDiagram::gridAttributes() const
{
    return &(d->gridAttributes);
}

void AbstractCartesianDiagram::drawGrid( PaintContext* context )
{
    if ( !gridAttributes()->isGridVisible() )
        return;
    QPair<QPointF, QPointF> boundaries = dataBoundaries();
    const int MinimumPixelsBetweenLines = 10;

    QPointF p1 = context->coordinatePlane()->translate( boundaries.first );
    QPointF p2 = context->coordinatePlane()->translate( boundaries.second );

    double screenRangeX = qAbs ( p1.x() - p2.x() );
    double screenRangeY = qAbs ( p1.y() - p2.y() );

    int numberOfUnitLinesX = model()->rowCount(rootIndex()) - 1;
    int numberOfUnitLinesY = (int) (boundaries.second.y() - boundaries.first.y());

    /* Dunno if this is correct, but we get a SIGFPE later
       if we dont bail out */
    if( numberOfUnitLinesX <= 0 ) return;
    if( numberOfUnitLinesY <= 0 ) return;

    bool drawUnitLinesX = screenRangeX / numberOfUnitLinesX > MinimumPixelsBetweenLines;
    bool drawUnitLinesY = screenRangeY / numberOfUnitLinesY > MinimumPixelsBetweenLines;
    int unitFactorX = 1;
    int unitFactorY = 1;

    while ( !drawUnitLinesX ) {
        unitFactorX *= 10;
        drawUnitLinesX = screenRangeX / (numberOfUnitLinesX / unitFactorX) > MinimumPixelsBetweenLines;
    }
    while ( !drawUnitLinesY ) {
        unitFactorY *= 10;
        drawUnitLinesY = screenRangeY / (numberOfUnitLinesY / unitFactorY) > MinimumPixelsBetweenLines;
    }

    bool drawHalfLinesX = screenRangeX / (numberOfUnitLinesX * 2) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawHalfLinesY = screenRangeY / (numberOfUnitLinesY * 2) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawFourthLinesX = screenRangeX / (numberOfUnitLinesX * 4) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawFourthLinesY = screenRangeY / (numberOfUnitLinesY * 4) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();

    float minValueX = boundaries.first.x();
    float maxValueX = boundaries.second.x();
    float minValueY = boundaries.first.y();
    float maxValueY = boundaries.second.y();

    if ( drawFourthLinesX ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( float f = minValueX; f <= maxValueX; f += 0.25 ) {
            QPointF topPoint( f, maxValueY );
	    QPointF bottomPoint( f, minValueY );
            topPoint = context->coordinatePlane()->translate( topPoint );
            bottomPoint = context->coordinatePlane()->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawFourthLinesY ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( float f = minValueY; f <= maxValueY; f += 0.25 ) {
            QPointF leftPoint( minValueX, f );
	    QPointF rightPoint( maxValueX, f );
            leftPoint = context->coordinatePlane()->translate( leftPoint );
            rightPoint = context->coordinatePlane()->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    if ( drawHalfLinesX ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( float f = minValueX; f <= maxValueX; f += 0.5 ) {
            QPointF topPoint( f, maxValueY );
	    QPointF bottomPoint( f, minValueY );
            topPoint = context->coordinatePlane()->translate( topPoint );
            bottomPoint = context->coordinatePlane()->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawHalfLinesY ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( float f = minValueY; f <= maxValueY; f += 0.5 ) {
            QPointF leftPoint( minValueX, f );
	    QPointF rightPoint( maxValueX, f );
            leftPoint = context->coordinatePlane()->translate( leftPoint );
            rightPoint = context->coordinatePlane()->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    if ( drawUnitLinesX ) {
        context->painter()->setPen( gridAttributes()->gridPen() );
        float minX = minValueX;
        if ( qRound( minX ) % unitFactorX != 0 )
            minX = minX + unitFactorX * (minX > 0 ? 1 : -1) - qRound( minX ) % unitFactorX;
        for ( float f = minX; f <= maxValueX; f += unitFactorX ) {
            QPointF topPoint( f, maxValueY );
	    QPointF bottomPoint( f, minValueY );
            topPoint = context->coordinatePlane()->translate( topPoint );
            bottomPoint = context->coordinatePlane()->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawUnitLinesY ) {
        context->painter()->setPen( gridAttributes()->gridPen() );
        float minY = minValueY;
        if ( qRound( minY ) % unitFactorY != 0 )
            minY = minY + unitFactorY * (minY > 0 ? 1 : -1) - qRound( minY ) % unitFactorY;
        for ( float f = minY; f <= maxValueY; f += unitFactorY ) {
            QPointF leftPoint( minValueX, f );
	    QPointF rightPoint( maxValueX, f );
            leftPoint = context->coordinatePlane()->translate( leftPoint );
            rightPoint = context->coordinatePlane()->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    context->painter()->setPen( gridAttributes()->zeroLinePen() );
    QPointF topPoint( 0, maxValueY );
    QPointF bottomPoint( 0, minValueY );
    QPointF leftPoint( minValueX, 0 );
    QPointF rightPoint( maxValueX, 0 );
    topPoint = context->coordinatePlane()->translate( topPoint );
    bottomPoint = context->coordinatePlane()->translate( bottomPoint );
    leftPoint = context->coordinatePlane()->translate( leftPoint );
    rightPoint = context->coordinatePlane()->translate( rightPoint );
    context->painter()->drawLine( topPoint, bottomPoint );
    context->painter()->drawLine( leftPoint, rightPoint );
}
