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

#include <cmath>

#include <QtDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>

#include "KDChartPaintContext.h"
#include "KDChartCartesianAxis.h"
#include "KDChartCartesianAxis_p.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartPainterSaver_p.h"


using namespace KDChart;

#define d (d_func())

CartesianAxis::CartesianAxis ( AbstractCartesianDiagram* parent )
    : AbstractAxis ( new Private(), parent )
{
    init();
    parent->addAxis( this );
}

CartesianAxis::~CartesianAxis ()
{
}

void CartesianAxis::init ()
{
    d->position = Bottom;
}

void CartesianAxis::setPosition ( Position p )
{
    d->position = p;
}

const CartesianAxis::Position CartesianAxis::position() const
{
    return d->position;
}

void CartesianAxis::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );

    PaintContext context;
    QPainter painter( this );
    context.setPainter( &painter );
    AbstractDiagram* diag = d->diagram();
    AbstractCoordinatePlane* plane = diag->coordinatePlane();
    context.setCoordinatePlane( plane );
    QRectF rect = QRectF ( 1, 1, plane->width() - 3, plane->height() - 3 );
    context.setRectangle( rect );
    d->geometry.setSize( size() );
    paint( &context );
}

#define ptr (context->painter())
void CartesianAxis::paint ( PaintContext* context ) const
{

    Q_ASSERT ( d->diagram() );
    const int MinimumPixelsBetweenRulers = 5;
    const QPair<QPointF, QPointF> dataBoundaries = d->diagram()->dataBoundaries();

    // preparations:
    // - calculate the range that will be displayed:
    double range;
    if ( position() == Bottom || position() == Top )
    {
        range = dataBoundaries.second.x() - dataBoundaries.first.x();
    } else {
        range = dataBoundaries.second.y() - dataBoundaries.first.y();
    }

    double absRange = qAbs ( range );
    // - calculate the number of unit, fifth and half measure rulers we will draw:
    /* for line and bar charts the number of rulers should depend of the number of
     * line = rows
     * bar = groups
     */
    /*
    int numberOfUnitRulers = ( int ) ( ( absRange + basicUnit * 1.05 ) / basicUnit );
    int numberOfFourthRulers = numberOfUnitRulers * 5 + 1;
    int numberOfHalfRulers = numberOfUnitRulers * 2 + 1;
    */

    // Fixme Michel: Need to find the type of chart here - Line or Bar
    // if Bars calculate the number of groups

    int numberOfUnitRulers;
    if ( position() == Bottom || position() == Top )
        numberOfUnitRulers = d->diagram()->model()->rowCount() - 1;
    else {
        if ( d->diagram()->percentMode() )
            numberOfUnitRulers = 10;
        else
            numberOfUnitRulers = ( int ) absRange;
    }
    int numberOfFourthRulers = numberOfUnitRulers * 4;
    int numberOfHalfRulers = numberOfUnitRulers * 2;

    // - calculate the absolute range in screen pixels:
    QPointF p1 = context->coordinatePlane()->translate( dataBoundaries.first );
    QPointF p2 = context->coordinatePlane()->translate( dataBoundaries.second );

    double screenRange;
    if ( position() == Bottom || position() == Top )
    {
        screenRange = qAbs ( p1.x() - p2.x() );
    } else {
        screenRange = qAbs ( p1.y() - p2.y() );
    }

    bool drawUnitRulers = screenRange / numberOfUnitRulers > MinimumPixelsBetweenRulers;
    bool drawFourthRulers = screenRange / numberOfFourthRulers > MinimumPixelsBetweenRulers;
    bool drawHalfRulers = screenRange / numberOfHalfRulers > MinimumPixelsBetweenRulers;
    // - find the reference point at which to start drawing and the increment (line distance);
    QPointF rulerRef;
    QRectF geoRect( d->geometry );
    QRectF rulerRect;
    double rulerWidth;
    double rulerHeight;


    // FIXME references are of course different for all locations:
    rulerWidth = geoRect.width();
    rulerHeight =  geoRect.height();
    if ( position() == Top )
    {
        rulerRef.setX( geoRect.topLeft().x() );
        rulerRef.setY( geoRect.topLeft().y() + rulerHeight );
    } else if ( position() == Bottom ) {
        rulerRef.setX( geoRect.bottomLeft().x() );
        rulerRef.setY( geoRect.bottomLeft().y() - rulerHeight );
    } else if ( position() == Right ) {
        rulerRef.setX( geoRect.bottomRight().x() - rulerWidth );
        rulerRef.setY( geoRect.bottomRight().y() );
    } else {
        rulerRef.setX( geoRect.bottomLeft().x() + rulerWidth );
        rulerRef.setY( geoRect.bottomLeft().y() );
    }

    // set up the lines to paint:

    // set up a map of integer positions,

    // - starting with the fourth
    // - the the halfs
    // - then the tens
    // this will override all halfs and fourth that hit a higher-order ruler
    // MAKE SURE TO START AT (0, 0)!

    // set up a reference point,  a step vector and a unit vector for the drawing:

    #ifdef AXES_PAINTING_DEBUG
    qDebug() << "CartesianAxis::paint: reference values:" << endl
             << "-- range: " << range << endl
             << "-- absRange: " << absRange << endl
             << "-- magnitude: " << magnitude << endl
             << "-- basicUnit: " << basicUnit << endl
             << "-- numberOfUnitRulers: " << numberOfUnitRulers << endl
             << "-- numberOfHalfRulers: " << numberOfHalfRulers << endl
             << "-- numberOfFourthRulers: " << numberOfFourthRulers << endl
             << "-- screenRange: " << screenRange << endl
             << "-- drawUnitRulers: " << drawUnitRulers << endl
             << "-- drawHalfRulers: " << drawHalfRulers << endl
             << "-- drawFourthRulers: " << drawFourthRulers << endl
             << "-- ruler reference point:: " << rulerRef << endl;
    #endif

    ptr->setPen ( Qt::black );
    ptr->setBrush ( Qt::red );
    QPointF fourthRulerRef ( rulerRef );
    int minValueY = qRound( dataBoundaries.first.y() );
    int maxValueY = qRound( dataBoundaries.second.y() );
    int minValueX = qRound( dataBoundaries.first.x() );
    int maxValueX = qRound( dataBoundaries.second.x() );


    if ( drawFourthRulers ) {
        if ( position() == Top || position() == Bottom ) {
            int tickLength = position() == Top ? -2 : 1;
            for ( float f = minValueX; f <= maxValueX; f += 0.25 ) {
                QPointF topPoint ( f, 0 );
                QPointF bottomPoint ( f, 0 );
                topPoint = context->coordinatePlane()->translate( topPoint );
                bottomPoint = context->coordinatePlane()->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
            }
        } else {
            int tickLength = position() == Left ? -1 : 1;
            for ( float f = minValueY; f <= maxValueY; f += 0.25 ) {
                QPointF leftPoint ( 0, f );
                QPointF rightPoint ( 0, f );
                leftPoint = context->coordinatePlane()->translate( leftPoint );
                rightPoint = context->coordinatePlane()->translate( rightPoint );
                leftPoint.setX( fourthRulerRef.x() + tickLength );
                rightPoint.setX( fourthRulerRef.x() );
                ptr->drawLine( leftPoint, rightPoint );
           }
       }
    }

    if ( drawHalfRulers ) {
        if ( position() == Top || position() == Bottom ) {
            int tickLength = position() == Top ? -3 : 2;
            for ( float f = minValueX; f <= maxValueX; f += 0.5 ) {
                QPointF topPoint ( f, 0 );
                QPointF bottomPoint ( f, 0 );
                topPoint = context->coordinatePlane()->translate( topPoint );
                bottomPoint = context->coordinatePlane()->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
            }
        } else {
            int tickLength = position() == Left ? -3 : 2;
            for ( float f = minValueY; f <= maxValueY; f += 0.5 ) {
                QPointF leftPoint ( 0, f );
                QPointF rightPoint ( 0, f );
                leftPoint = context->coordinatePlane()->translate( leftPoint );
                rightPoint = context->coordinatePlane()->translate( rightPoint );
                leftPoint.setX( fourthRulerRef.x() + tickLength );
                rightPoint.setX( fourthRulerRef.x() );
                ptr->drawLine( leftPoint, rightPoint );
           }
       }
    }

    if ( drawUnitRulers ) {
        if ( position() == Top || position() == Bottom ) {
            int tickLength = position() == Top ? -4 : 3;
            for ( int i = minValueX; i <= maxValueX; ++i ) {
                QPointF topPoint ( i, 0 );
                QPointF bottomPoint ( i, 0 );
                topPoint = context->coordinatePlane()->translate( topPoint );
                bottomPoint = context->coordinatePlane()->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
                if ( drawHalfRulers ) {
                    QFont textFont( QFont("comic", 5 ) );
                    topPoint.setX( topPoint.x() - textFont.pointSize() );
                    topPoint.setY( position() == Top ? topPoint.y() - textFont.pointSize() : topPoint.y() + (2 * textFont.pointSize()) );
                    PainterSaver painterSaver( ptr );
                    ptr->setPen( Qt::blue );
                    ptr->setFont( textFont );
                    ptr->drawText( topPoint, QString::number( minValueX ) );
                    minValueX += 1;
                }
            }
        } else {
          double maxLimit, steg;
	  bool percent = d->diagram()->percentMode();
	  int tickLength = position() == Left ? -4 : 3;
          if ( percent ) {
	    maxLimit = maxValueY*10;
	    steg = maxValueY;
	  } else {
            maxLimit = maxValueY;
            steg = 1.0;
	  }
	    for ( double f = minValueY; f <= maxLimit; f+= steg ) {
	    QPointF leftPoint ( 0,  percent ? f/numberOfUnitRulers : f );
	    QPointF rightPoint ( 0, percent ? f/numberOfUnitRulers : f );
	    leftPoint = context->coordinatePlane()->translate( leftPoint );
	    rightPoint = context->coordinatePlane()->translate( rightPoint );
	    leftPoint.setX( fourthRulerRef.x() + tickLength );
	    rightPoint.setX( fourthRulerRef.x() );
	    ptr->drawLine( leftPoint, rightPoint );
	    if ( drawHalfRulers ) {
	      QFont textFont( QFont("comic", 5 ) );
	      leftPoint.setX( position() == Left ? leftPoint.x()- (2*textFont.pointSize()) : leftPoint.x()+textFont.pointSize() );
	      leftPoint.setY( leftPoint.y() + textFont.pointSize()/2 );
	      PainterSaver painterSaver( ptr );
	      ptr->setPen( Qt::red );
	      ptr->setFont( textFont );
	      ptr->drawText( leftPoint, QString::number( minValueY ) );
	      d->diagram()->percentMode() ? minValueY += 10 : minValueY += 1;
	    }
	  }
	    //Pending Michel: reset to default - is that really what we want?
	    d->diagram()->setPercentMode( false );
	}
    }
}
#undef ptr

QSize CartesianAxis::sizeHint() const
{   // FIXME return real values:
    QSize result;

    switch ( position() )
    {
    case Bottom:
    case Top:
        result = QSize ( 1, 32 );
        break;
    case Left:
    case Right:
        result = QSize ( 32, 1 );
        break;
    default:
        Q_ASSERT( false ); // all positions need to be handled
        break;
    };

    return result;
}

QSizePolicy CartesianAxis::sizePolicy() const
{
    QSizePolicy result;

    switch ( position() )
    {
    case Bottom:
    case Top:
        result.setVerticalPolicy( QSizePolicy::Fixed );
        break;
    case Left:
    case Right:
        result.setHorizontalPolicy( QSizePolicy::Fixed );
        break;
    default:
        Q_ASSERT( false ); // all positions need to be handeld
	break;
    };

    return result;
}

void CartesianAxis::setGeometry ( const QRectF& rect )
{
    d->geometry = rect;
}

QRectF CartesianAxis::geometry() const
{
    return d->geometry;
}

QDomDocumentFragment CartesianAxis::toXML() const
{
    Q_ASSERT_X ( false, "CartesianAxis::toXML()", "not implemented" );
    return QDomDocumentFragment();
}
