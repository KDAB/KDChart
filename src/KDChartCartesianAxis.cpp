/****************************************************************************
** Copyright (C) 2006 Klarävdalens Datakonsult AB.  All rights reserved.
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
#include <QApplication>

#include "KDChartPaintContext.h"
#include "KDChartCartesianAxis.h"
#include "KDChartCartesianAxis_p.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartLayoutItems.h"


using namespace KDChart;

#define d (d_func())

CartesianAxis::CartesianAxis ( AbstractCartesianDiagram* diagram )
    : AbstractAxis ( new Private( diagram ), diagram )
{
    init();
    if( diagram )
        diagram->addAxis( this );
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

    if( ! d->diagram || ! d->diagram->coordinatePlane() ) return;

    PaintContext context;
    QPainter painter( this );
    context.setPainter( &painter );
    AbstractCoordinatePlane* plane = d->diagram->coordinatePlane();
    context.setCoordinatePlane( plane );
    QRectF rect = QRectF ( 1, 1, plane->width() - 3, plane->height() - 3 );
    context.setRectangle( rect );
    d->geometry.setSize( size() );
    paint( &context );
}


bool CartesianAxis::isAbscissa() const
{
    return position() == Bottom || position() == Top;
}

bool CartesianAxis::isOrdinate() const
{
    return position() == Left || position() == Right;
}

#define ptr (context->painter())
void CartesianAxis::paint ( PaintContext* context ) const
{
    Q_ASSERT_X ( d->diagram, "CartesianAxis::paint",
                 "Function call not allowed: The axis is not assigned to any diagram." );

    CartesianCoordinatePlane* plane = dynamic_cast<CartesianCoordinatePlane*>(context->coordinatePlane());
    Q_ASSERT_X ( plane, "CartesianAxis::paint",
                 "Bad function call: PaintContext::coodinatePlane() NOT a cartesian plane." );

    const int MinimumPixelsBetweenRulers = 5;
    const DataDimensionsList dimensions( plane->gridDimensionsList() );
    // test for programming errors: critical
    Q_ASSERT_X ( dimensions.count() == 2, "CartesianAxis::paint",
                 "Error: plane->getDataDimensionsList() did not return exactly two dimensions." );
    const DataDimension& dimX( dimensions.first() );
    const DataDimension& dimY( dimensions.last() );

    const bool isLeft   = position() == Left;
    const bool isRight  = position() == Right;
    const bool isTop    = position() == Top;
    const bool isBottom = position() == Bottom;

    // preparations:
    // - calculate the range that will be displayed:
    const qreal absRange = qAbs ( isAbscissa() ? dimX.distance() : dimY.distance() );

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

    qreal numberOfUnitRulers;
    if ( isAbscissa() )
        numberOfUnitRulers = d->diagram->model()->rowCount() - 1.0;
    else {
        if ( d->diagram->percentMode() )
            numberOfUnitRulers = 10.0;
        else{
            numberOfUnitRulers = absRange / qAbs( dimY.stepWidth ) + 1.0;
            qDebug() << "absRange" << absRange << "dimY.stepWidth:" << dimY.stepWidth << "numberOfUnitRulers:" << numberOfUnitRulers;
        }
    }
/*
    int numberOfFourthRulers = numberOfUnitRulers * 4;
    int numberOfHalfRulers = numberOfUnitRulers * 2;
*/
    // - calculate the absolute range in screen pixels:
    const QPointF p1 = plane->translate( QPointF(dimX.start, dimY.start) );
    const QPointF p2 = plane->translate( QPointF(dimX.end, dimX.end) );

    double screenRange;
    if ( isAbscissa() )
    {
        screenRange = qAbs ( p1.x() - p2.x() );
    } else {
        screenRange = qAbs ( p1.y() - p2.y() );
    }

    const bool useItemCountLabels = isAbscissa() && d->diagram->datasetDimension() == 1;

    bool drawUnitRulers = screenRange / numberOfUnitRulers > MinimumPixelsBetweenRulers;

/*
    // for the next two lines, please note:
    // There are no such things as "half of Item 1" or "0.75 times Thursday"
    bool drawFourthRulers = screenRange / numberOfFourthRulers > MinimumPixelsBetweenRulers && ! useItemCountLabels;
    bool drawHalfRulers = screenRange / numberOfHalfRulers > MinimumPixelsBetweenRulers && ! useItemCountLabels;
*/
    const TextAttributes ta = textAttributes();
    const bool drawLabels = ta.isVisible();/* && (screenRange / numberOfHalfRulers > MinimumPixelsBetweenRulers)*/;

    // - find the reference point at which to start drawing and the increment (line distance);
    QPointF rulerRef;
    QRectF geoRect( d->geometry );
    QRectF rulerRect;
    double rulerWidth;
    double rulerHeight;


    // FIXME references are of course different for all locations:
    rulerWidth = geoRect.width();
    rulerHeight =  geoRect.height();
    if ( isTop )
    {
        rulerRef.setX( geoRect.topLeft().x() );
        rulerRef.setY( geoRect.topLeft().y() + rulerHeight );
    } else if ( isBottom ) {
        rulerRef.setX( geoRect.bottomLeft().x() );
        rulerRef.setY( geoRect.bottomLeft().y() - rulerHeight );
    } else if ( isRight ) {
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
            << "-- range: " << dimension.distance() << endl
            << "-- absRange: " << absRange << endl
            << "-- magnitude: " << magnitude << endl
            << "-- basicUnit: " << basicUnit << endl
            << "-- numberOfUnitRulers: " << numberOfUnitRulers << endl
//            << "-- numberOfHalfRulers: " << numberOfHalfRulers << endl
//            << "-- numberOfFourthRulers: " << numberOfFourthRulers << endl
            << "-- screenRange: " << screenRange << endl
            << "-- drawUnitRulers: " << drawUnitRulers << endl
//            << "-- drawHalfRulers: " << drawHalfRulers << endl
            << "-- drawLabels: " << drawLabels << endl
//            << "-- drawFourthRulers: " << drawFourthRulers << endl
            << "-- ruler reference point:: " << rulerRef << endl;
    #endif

    ptr->setPen ( Qt::black );
    ptr->setBrush ( Qt::red );
    QPointF fourthRulerRef ( rulerRef );
    qreal minValueY = dimY.start;
    qreal maxValueY = dimY.end;
    qreal minValueX = dimX.start;
    qreal maxValueX = dimX.end;

/*
    if ( drawFourthRulers ) {
        if ( isAbscissa() ) {
            int tickLength = isTop ? -2 : 1;
            for ( float f = minValueX; f <= maxValueX; f += 0.25 ) {
                QPointF topPoint ( f, 0 );
                QPointF bottomPoint ( f, 0 );
                topPoint = plane->translate( topPoint );
                bottomPoint = plane->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
            }
        } else {
            int tickLength = isLeft ? -1 : 1;
            for ( float f = minValueY; f <= maxValueY; f += 0.25 ) {
                QPointF leftPoint ( 0, f );
                QPointF rightPoint ( 0, f );
                leftPoint = plane->translate( leftPoint );
                rightPoint = plane->translate( rightPoint );
                leftPoint.setX( fourthRulerRef.x() + tickLength );
                rightPoint.setX( fourthRulerRef.x() );
                ptr->drawLine( leftPoint, rightPoint );
          }
      }
    }

    if ( drawHalfRulers ) {
        if ( isAbscissa() ) {
            int tickLength = isTop ? -3 : 2;
            for ( float f = minValueX; f <= maxValueX; f += 0.5 ) {
                QPointF topPoint ( f, 0 );
                QPointF bottomPoint ( f, 0 );
                topPoint = plane->translate( topPoint );
                bottomPoint = plane->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
            }
        } else {
            int tickLength = isLeft ? -3 : 2;
            for ( float f = minValueY; f <= maxValueY; f += 0.5 ) {
                QPointF leftPoint ( 0, f );
                QPointF rightPoint ( 0, f );
                leftPoint = plane->translate( leftPoint );
                rightPoint = plane->translate( rightPoint );
                leftPoint.setX( fourthRulerRef.x() + tickLength );
                rightPoint.setX( fourthRulerRef.x() );
                ptr->drawLine( leftPoint, rightPoint );
          }
      }
    }
*/
    if ( drawUnitRulers ) {
qDebug() << isOrdinate();
        const int hardLabelsCount  = labels().count();
        const int shortLabelsCount = shortLabels().count();
        bool useShortLabels = false;

        QStringList headerLabels;
        if( useItemCountLabels ){
            headerLabels =
                isOrdinate()
                ? d->diagram->datasetLabels()
                : d->diagram->itemRowLabels();
        }
        const int headerLabelsCount = headerLabels.count();

        const AbstractCoordinatePlane* plane = coordinatePlane();
        const QWidget* referenceArea =
            plane
            ? static_cast<QWidget *>( plane->parent() )
            : static_cast<QWidget *>( parent() );
        TextLayoutItem* labelItem =
            drawLabels
            ? new TextLayoutItem( QString::number( minValueY ),
                      ta,
                      referenceArea,
                      KDChartEnums::MeasureOrientationMinimum,
                      Qt::AlignLeft )
            : 0;
        const QFontMetricsF met(
            drawLabels
            ? labelItem->realFont()
            : QFontMetricsF( QApplication::font() ) );

        if ( isAbscissa() ) {
            int tickLength = isTop ? -4 : 3;

            // If we have a labels list AND a short labels list, we first find out,
            // if there is enough space for the labels: if not, use the short labels.
            if( drawLabels && hardLabelsCount && shortLabelsCount ){
                bool labelsAreOverlapping = false;
                QRegion combinedRegion;
                int iLabel = 0;
                for ( qreal i = minValueX; i <= maxValueX && ! labelsAreOverlapping; i+=1.0 ) {
                    labelItem->setText( labels()[ iLabel ] );
                    // No need to call labelItem->setParentWidget(), since we are using
                    // the layout item temporarily only.
                    QPointF topPoint ( i, 0.0 );
                    topPoint = plane->translate( topPoint );
                    topPoint.setX( topPoint.x() - met.height() );
                    topPoint.setY( fourthRulerRef.y() + tickLength );
                    topPoint.setY(
                        isTop
                        ? ( topPoint.y() -        met.height()  )
                        : ( topPoint.y() + (2.0 * met.height()) ) );

		    const QRect r( QPoint( static_cast<int>(topPoint.x()), 
					   static_cast<int>(topPoint.y()) ),
				   labelItem->sizeHint() );
		    const QRegion region( r.adjusted( -1,-1,1,1) );// a minimum of 2 pixels between the labels

                    labelsAreOverlapping = ! combinedRegion.intersect( region ).isEmpty();
                    combinedRegion += region;
                    if( iLabel >= hardLabelsCount-1 )
                        iLabel = 0;
                    else
                        ++iLabel;
                }
                useShortLabels = labelsAreOverlapping;
            }

            int iLabel = 0;
            for ( qreal i = minValueX; i <= maxValueX; i+=1.0 ) {
                QPointF topPoint ( i + (useItemCountLabels ? 0.5 : 0.0), 0.0 );
                QPointF bottomPoint ( topPoint );
                topPoint = plane->translate( topPoint );
                bottomPoint = plane->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
                if ( drawLabels ) {
                    labelItem->setText( hardLabelsCount
                        ? ( useShortLabels    ? shortLabels()[ iLabel ] : labels()[ iLabel ] )
                        : ( headerLabelsCount ? headerLabels[  iLabel ] : QString::number( minValueX ) ) );
                    // No need to call labelItem->setParentWidget(), since we are using
                    // the layout item temporarily only.
                    const QSize size( labelItem->sizeHint() );
                    labelItem->setGeometry(
                        QRect(
                            QPoint(
                                static_cast<int>( topPoint.x() - size.width() / 2 ),
                                static_cast<int>( topPoint.y()
                                    + met.height() * (isBottom ? 0.5 : -1.5 )) ),
                            size ) );
                    labelItem->paint( ptr );
                    if( hardLabelsCount ){
                        if( iLabel >= hardLabelsCount  -1 )
                            iLabel = 0;
                        else
                            ++iLabel;
                    }else if( headerLabelsCount ){
                        if( iLabel >= headerLabelsCount-1 )
                            iLabel = 0;
                        else
                            ++iLabel;
                    }else{
                        minValueX += 1.0;
                    }
                }
            }
        } else {
            double maxLimit, steg;
            bool percent = d->diagram->percentMode();
            int tickLength = isLeft ? -4 : 3;
            if ( percent ) {
                maxLimit = maxValueY*10.0;
                steg = maxValueY;
            } else {
                maxLimit = maxValueY;
                steg = dimY.stepWidth;
            }
            int maxLabelsWidth = 0;
            qreal labelValue;
            if( drawLabels && isRight ){
                // Find the wides label, so we to know how much we need to right-shift
                // our labels, to get them drawn right aligned:
                labelValue = minValueY;
                for ( qreal f = minValueY; f <= maxLimit; f+= steg ) {
                    labelItem->setText( QString::number( labelValue ) );
                    maxLabelsWidth = qMax( maxLabelsWidth, labelItem->sizeHint().width() );
                    d->diagram->percentMode() ? labelValue += 10.0 : labelValue += dimY.stepWidth;
                }
            }
            labelValue = minValueY;
qDebug("minValueY: %f   maxLimit: %f   steg: %f", minValueY, maxLimit, steg);
            for ( qreal f = minValueY; f <= maxLimit; f+= steg ) {
qDebug("f: %f",f);
                QPointF leftPoint (  0.0, percent ? f/numberOfUnitRulers : f );
                QPointF rightPoint ( 0.0, percent ? f/numberOfUnitRulers : f );
                leftPoint  = plane->translate( leftPoint );
                rightPoint = plane->translate( rightPoint );
                leftPoint.setX( fourthRulerRef.x() + tickLength );
                rightPoint.setX( fourthRulerRef.x() );
                ptr->drawLine( leftPoint, rightPoint );
                if ( drawLabels ) {
                    labelItem->setText( QString::number( labelValue ) );
                    // No need to call labelItem->setParentWidget(), since we are using
                    // the layout item temporarily only.
                    const QSize labelSize( labelItem->sizeHint() );
                    leftPoint.setX( leftPoint.x()
                         );
                    const int x =
                        static_cast<int>( leftPoint.x() + met.height() * (isLeft ? -0.5 : 0.5) )
                        - ( isLeft ? labelSize.width() : (labelSize.width() - maxLabelsWidth) );
                    int y;
                    if( f == minValueY ){
                        // first label of the ordinate?
                        // shift it up a bit, to prevent it from being clipped away
                        y = static_cast<int>( leftPoint.y() - met.ascent() * 0.7 );
                    }else if( f + steg > maxLimit ){
                        // last label of the ordinate?
                        // shift it down a bit, to prevent it from being clipped away
                        y = static_cast<int>( leftPoint.y() - met.ascent() * 0.35 );
                    }else{
                        y = static_cast<int>( leftPoint.y() - met.ascent() * 0.5 );
                    }
                    --y;

                    labelItem->setGeometry( QRect( QPoint(x, y), labelSize ) );
                    labelItem->paint( ptr );

                    d->diagram->percentMode() ? labelValue += 10.0 : labelValue += dimY.stepWidth;
                }
            }
            //Pending Michel: reset to default - is that really what we want?
            d->diagram->setPercentMode( false );
        }
        if( labelItem )
            delete labelItem;
    }
    //for debugging: ptr->drawRect(geoRect);
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
