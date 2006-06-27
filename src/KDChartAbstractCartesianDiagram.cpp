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

AbstractCartesianDiagram::AbstractCartesianDiagram ( CartesianCoordinatePlane* plane )
    : AbstractDiagram ( new Private(), plane )
{
}

void AbstractCartesianDiagram::addAxis( CartesianAxis *axis )
{
    d->axesList.append( axis );
    axis->createObserver( this );
}

void AbstractCartesianDiagram::takeAxis( CartesianAxis *axis )
{
    const int idx = d->axesList.indexOf( axis );
    if( idx != -1 )
        d->axesList.takeAt( idx );
    axis->deleteObserver( this );
    axis->setParent( 0 );
}

KDChart::CartesianAxisList AbstractCartesianDiagram::axes( ) const
{
    return d->axesList;
}

void KDChart::AbstractCartesianDiagram::setCoordinatePlane( AbstractCoordinatePlane* plane )
{
    if( coordinatePlane() ) disconnect( coordinatePlane() );
    AbstractDiagram::setCoordinatePlane(plane);
    connect( plane, SIGNAL(rangeChanged()), this, SLOT(slotRangeChanged()) );

    // show the axes, after all have been adjusted
    // (because they might be dependend on each other)
    /*
    if( plane )
        foreach( CartesianAxis* axis, d->axesList )
            axis->show();
    else
        foreach( CartesianAxis* axis, d->axesList )
            axis->hide();
    */
}

void AbstractCartesianDiagram::slotRangeChanged()
{
    foreach( CartesianAxis* a, d->axesList ) {
	a->update();
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
/*
void AbstractCartesianDiagram::drawGrid( PaintContext* context )
{
    if ( !checkInvariants() || !gridAttributes()->isGridVisible() ) return;

    QPair<QPointF, QPointF> boundaries = dataBoundaries();

    const bool hasXCoordinates = datasetDimension() > 1;

    // PENDING(khz) FIXME: make numberOfUnitLinesX work for data with X coordinates too:
    const qreal numberOfUnitLinesX = model()->rowCount(rootIndex()) - 1.0;
    const qreal numberOfUnitLinesY = qAbs( boundaries.second.y() - boundaries.first.y() );

    // do not draw a Zero size grid (and do not divide by Zero)
    if( numberOfUnitLinesX <= 0.0 || numberOfUnitLinesY <= 0.0 ) return;
    if( !isBoundariesValid(boundaries) ) return;

    const QPointF p1 = context->coordinatePlane()->translate( boundaries.first );
    const QPointF p2 = context->coordinatePlane()->translate( boundaries.second );

    const qreal screenRangeX = qAbs ( p1.x() - p2.x() );
    const qreal screenRangeY = qAbs ( p1.y() - p2.y() );

    const qreal MinimumPixelsBetweenLines = 10.0;

    qreal unitFactorX = 1.0;
    qreal unitFactorY = 1.0;
    bool drawUnitLinesX = screenRangeX / numberOfUnitLinesX > MinimumPixelsBetweenLines;
    bool drawUnitLinesY = screenRangeY / numberOfUnitLinesY > MinimumPixelsBetweenLines;
    while ( !drawUnitLinesX ) {
        unitFactorX *= 10.0;
        drawUnitLinesX = screenRangeX / (numberOfUnitLinesX / unitFactorX) > MinimumPixelsBetweenLines;
    }
    while ( !drawUnitLinesY ) {
        unitFactorY *= 10.0;
        drawUnitLinesY = screenRangeY / (numberOfUnitLinesY / unitFactorY) > MinimumPixelsBetweenLines;
    }

    bool drawHalfLinesX = screenRangeX / (numberOfUnitLinesX * 2.0) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawHalfLinesY = screenRangeY / (numberOfUnitLinesY * 2.0) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawFourthLinesX = screenRangeX / (numberOfUnitLinesX * 4.0) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawFourthLinesY = screenRangeY / (numberOfUnitLinesY * 4.0) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();

    const qreal minValueX = boundaries.first.x();
    const qreal maxValueX = boundaries.second.x();
    const qreal minValueY = boundaries.first.y();
    const qreal maxValueY = boundaries.second.y();

    if ( drawFourthLinesX ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( qreal f = minValueX; f <= maxValueX; f += 0.25 ) {
            QPointF topPoint( f, maxValueY );
            QPointF bottomPoint( f, minValueY );
            topPoint = context->coordinatePlane()->translate( topPoint );
            bottomPoint = context->coordinatePlane()->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawFourthLinesY ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( qreal f = minValueY; f <= maxValueY; f += 0.25 ) {
            QPointF leftPoint( minValueX, f );
            QPointF rightPoint( maxValueX, f );
            leftPoint = context->coordinatePlane()->translate( leftPoint );
            rightPoint = context->coordinatePlane()->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    if ( drawHalfLinesX ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( qreal f = minValueX; f <= maxValueX; f += 0.5 ) {
            QPointF topPoint( f, maxValueY );
            QPointF bottomPoint( f, minValueY );
            topPoint = context->coordinatePlane()->translate( topPoint );
            bottomPoint = context->coordinatePlane()->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawHalfLinesY ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( qreal f = minValueY; f <= maxValueY; f += 0.5 ) {
            QPointF leftPoint( minValueX, f );
            QPointF rightPoint( maxValueX, f );
            leftPoint = context->coordinatePlane()->translate( leftPoint );
            rightPoint = context->coordinatePlane()->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    const bool drawXZeroLineX = hasXCoordinates &&
        gridAttributes()->zeroLinePen().style() != Qt::NoPen;
    const bool drawZeroLineY = gridAttributes()->zeroLinePen().style() != Qt::NoPen;

    if ( drawUnitLinesX || drawXZeroLineX ) {
        if ( drawUnitLinesX )
            context->painter()->setPen( gridAttributes()->gridPen() );
        qreal minX = minValueX;
        // PENDING(khz) FIXME: calculate the start value in a *way* more sophisticated way:
        if ( fmod( minX , unitFactorX ) != 0.0 )
            minX = unitFactorX * _trunc( minX / unitFactorX );

        for ( qreal f = minX; f <= maxValueX; f += unitFactorX ) {
            // PENDING(khz) FIXME: make draving/not drawing of Zero line more sophisticated?:
            const bool zeroLineHere = drawXZeroLineX && (f == 0.0);
            if ( drawUnitLinesX || zeroLineHere ){
                QPointF topPoint( f, maxValueY );
                QPointF bottomPoint( f, minValueY );
                topPoint = context->coordinatePlane()->translate( topPoint );
                bottomPoint = context->coordinatePlane()->translate( bottomPoint );
                if ( zeroLineHere )
                    context->painter()->setPen( gridAttributes()->zeroLinePen() );
                context->painter()->drawLine( topPoint, bottomPoint );
                if ( zeroLineHere )
                    context->painter()->setPen( gridAttributes()->gridPen() );
            }
        }
    }
    if ( drawUnitLinesY || drawZeroLineY ) {
        if ( drawUnitLinesY )
            context->painter()->setPen( gridAttributes()->gridPen() );
        qreal minY = minValueY;
        // PENDING(khz) FIXME: calculate the start value in a *way* more sophisticated way:
        if ( fmod( minY , unitFactorY ) != 0.0 )
            minY = unitFactorY * _trunc( minY / unitFactorY );

        for ( qreal f = minY; f <= maxValueY; f += unitFactorY ) {
            // PENDING(khz) FIXME: make draving/not drawing of Zero line more sophisticated?:
//qDebug("f: %f",f);
            const bool zeroLineHere = (f == 0.0);
            if ( drawUnitLinesY || zeroLineHere ){
                QPointF leftPoint( minValueX, f );
                QPointF rightPoint( maxValueX, f );
                leftPoint = context->coordinatePlane()->translate( leftPoint );
                rightPoint = context->coordinatePlane()->translate( rightPoint );
                if ( zeroLineHere )
                    context->painter()->setPen( gridAttributes()->zeroLinePen() );
                context->painter()->drawLine( leftPoint, rightPoint );
                if ( zeroLineHere )
                    context->painter()->setPen( gridAttributes()->gridPen() );
            }
        }
    }
}
*/

