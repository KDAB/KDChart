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
#include "KDChartChart.h"
#include "KDChartCartesianAxis.h"
#include "KDChartCartesianAxis_p.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartLayoutItems.h"

#include <KDABLibFakes>


using namespace KDChart;

#define d (d_func())

CartesianAxis::CartesianAxis ( AbstractCartesianDiagram* diagram )
    : AbstractAxis ( new Private( diagram, this ), diagram )
{
    init();
}

CartesianAxis::~CartesianAxis ()
{
}

void CartesianAxis::init ()
{
    d->position = Bottom;
}

void CartesianAxis::setTitleText( const QString& text )
{
    //FIXME(khz): Call update al all places where axis internals are changed!
    d->titleText = text;
    //d->needRebuild = true;
}

QString CartesianAxis::titleText() const
{
    return d->titleText;
}

void CartesianAxis::setTitleTextAttributes( const TextAttributes &a )
{
    d->titleTextAttributes = a;
    d->useDefaultTextAttributes = false;
    //d->needRebuild = true;
}

TextAttributes CartesianAxis::titleTextAttributes() const
{
    if( hasDefaultTitleTextAttributes() ){
        TextAttributes ta( textAttributes() );
        Measure me( ta.fontSize() );
        me.setValue( me.value() * 1.5 );
        ta.setFontSize( me );
        return ta;
    }
    return d->titleTextAttributes;
}

void CartesianAxis::resetTitleTextAttributes()
{
    d->useDefaultTextAttributes = true;
    //d->needRebuild = true;
}

bool CartesianAxis::hasDefaultTitleTextAttributes() const
{
    return d->useDefaultTextAttributes;
}

void CartesianAxis::setPosition ( Position p )
{
    d->position = p;
}

const CartesianAxis::Position CartesianAxis::position() const
{
    return d->position;
}

bool CartesianAxis::isAbscissa() const
{
    return position() == Bottom || position() == Top;
}

bool CartesianAxis::isOrdinate() const
{
    return position() == Left || position() == Right;
}

/*
void CartesianAxis::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );

    if( ! d->diagram() || ! d->diagram()->coordinatePlane() ) return;

    PaintContext context;
    QPainter painter( this );
    context.setPainter( &painter );
    AbstractCoordinatePlane* plane = d->diagram()->coordinatePlane();
    context.setCoordinatePlane( plane );
    QRectF rect = QRectF ( 1, 1, plane->width() - 3, plane->height() - 3 );
    context.setRectangle( rect );
    d->geometry.setSize( size() );
    paintCtx( &context );
}
*/

void CartesianAxis::paint( QPainter* painter )
{
    //qDebug() << "KDChart::CartesianAxis::paint() called";
    if( ! d->diagram() || ! d->diagram()->coordinatePlane() ) return;
    PaintContext ctx;
    ctx.setPainter ( painter );
    ctx.setCoordinatePlane( d->diagram()->coordinatePlane() );
    const QRect rect( areaGeometry() );
    ctx.setRectangle(
        QRectF (
            //QPointF(0, 0),
            QPointF(rect.left(), rect.top()),
            QSizeF(rect.width(), rect.height() ) ) );
    paintCtx( &ctx );
    //qDebug() << "KDChart::CartesianAxis::paint() done.";
}

#define ptr (context->painter())
void CartesianAxis::paintCtx( PaintContext* context )
{
    //qDebug() << "KDChart::CartesianAxis::paintCtx() called";

    Q_ASSERT_X ( d->diagram(), "CartesianAxis::paint",
                 "Function call not allowed: The axis is not assigned to any diagram." );

    CartesianCoordinatePlane* plane = dynamic_cast<CartesianCoordinatePlane*>(context->coordinatePlane());
    Q_ASSERT_X ( plane, "CartesianAxis::paint",
                 "Bad function call: PaintContext::coodinatePlane() NOT a cartesian plane." );

    const int MinimumPixelsBetweenRulers = 5;
    DataDimensionsList dimensions( plane->gridDimensionsList() );
    // test for programming errors: critical
    Q_ASSERT_X ( dimensions.count() == 2, "CartesianAxis::paint",
                 "Error: plane->gridDimensionsList() did not return exactly two dimensions." );
    DataDimension& dimX = dimensions.first();
    const DataDimension& dimY = dimensions.last();
    const DataDimension& dim = (isAbscissa() ? dimensions.first() : dimensions.last());

    const bool isLeft   = position() == Left;
    const bool isRight  = position() == Right;
    const bool isTop    = position() == Top;
    const bool isBottom = position() == Bottom;

    // preparations:
    // - calculate the range that will be displayed:
    const qreal absRange = qAbs( dim.distance() );

    // Fixme Michel: Need to find the type of chart here - Line or Bar
    // if Bars calculate the number of groups

    qreal numberOfUnitRulers;
    if ( isAbscissa() )
        numberOfUnitRulers = d->diagram()->model()->rowCount() - 1.0;
    else {
        if ( d->diagram()->percentMode() )
            numberOfUnitRulers = 10.0;
        else{
            numberOfUnitRulers = absRange / qAbs( dimY.stepWidth ) + 1.0;
            //qDebug() << "absRange" << absRange << "dimY.stepWidth:" << dimY.stepWidth << "numberOfUnitRulers:" << numberOfUnitRulers;
        }
    }
    qreal numberOfSubUnitRulers;
    if ( isAbscissa() )
        numberOfSubUnitRulers = 0.0;
    else {
        if ( d->diagram()->percentMode() )
            numberOfSubUnitRulers = 20.0;
        else{
            numberOfSubUnitRulers = absRange / qAbs( dimY.subStepWidth ) + 1.0;
            //qDebug() << "dimY.subStepWidth:" << dimY.stepWidth << "numberOfSubUnitRulers:" << numberOfSubUnitRulers;
        }
    }

    // - calculate the absolute range in screen pixels:
    const QPointF p1 = plane->translate( QPointF(dimX.start, dimY.start) );
    const QPointF p2 = plane->translate( QPointF(dimX.end, dimY.end) );

    double screenRange;
    if ( isAbscissa() )
    {
        screenRange = qAbs ( p1.x() - p2.x() );
    } else {
        screenRange = qAbs ( p1.y() - p2.y() );
    }

    const bool useItemCountLabels = isAbscissa() && d->diagram()->datasetDimension() == 1;




    //FIXME(khz): Remove this code, and do the calculation in the grid calc function
    if( isAbscissa() && ! dimX.isCalculated ){
        dimX.stepWidth = 1.0;
        while( screenRange / numberOfUnitRulers <= MinimumPixelsBetweenRulers ){
            dimX.stepWidth *= 10.0;
            //qDebug() << "adjusting dimX.stepWidth to" << dimX.stepWidth;
            numberOfUnitRulers = qAbs( dimX.distance() / dimX.stepWidth );
        }
    }

    const bool drawUnitRulers = screenRange / numberOfUnitRulers > MinimumPixelsBetweenRulers;
    const bool drawSubUnitRulers =
        (numberOfSubUnitRulers != 0.0) &&
        (screenRange / numberOfSubUnitRulers > MinimumPixelsBetweenRulers);

    const TextAttributes labelTA = textAttributes();
    const bool drawLabels = labelTA.isVisible();

    // - find the reference point at which to start drawing and the increment (line distance);
    QPointF rulerRef;
    QRect geoRect( areaGeometry() );
    QRectF rulerRect;
    double rulerWidth;
    double rulerHeight;

    //for debugging: if( isAbscissa() )ptr->drawRect(geoRect);
    //qDebug() << "         " << (isAbscissa() ? "Abscissa":"Ordinate") << "axis painting with geometry" << geoRect;

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
            << "-- screenRange: " << screenRange << endl
            << "-- drawUnitRulers: " << drawUnitRulers << endl
            << "-- drawLabels: " << drawLabels << endl
            << "-- ruler reference point:: " << rulerRef << endl;
    #endif

    ptr->setPen ( Qt::black );
    ptr->setBrush ( Qt::red ); // PENDING(michel) What is this for?
    QPointF fourthRulerRef ( rulerRef );
    qreal minValueY = dimY.start;
    qreal maxValueY = dimY.end;
    qreal minValueX = dimX.start;
    qreal maxValueX = dimX.end;

    if ( drawSubUnitRulers ) {
        if ( isAbscissa() ) {
            int tickLength = isTop ? -3 : 2;
            for ( float f = minValueX; f <= maxValueX; f += dimX.subStepWidth ) {
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
            for ( float f = minValueY; f <= maxValueY; f += dimY.subStepWidth ) {
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

    const QObject* referenceArea = plane->parent();

    if ( drawUnitRulers ) {
//qDebug() << isOrdinate();
        const int hardLabelsCount  = labels().count();
        const int shortLabelsCount = shortLabels().count();
        bool useShortLabels = false;

        QStringList headerLabels;
        if( useItemCountLabels ){
            headerLabels =
                isOrdinate()
                ? d->diagram()->datasetLabels()
                : d->diagram()->itemRowLabels();
        }
        const int headerLabelsCount = headerLabels.count();

        TextLayoutItem* labelItem =
            drawLabels
            ? new TextLayoutItem( QString::number( minValueY ),
                      labelTA,
                      referenceArea,
                      KDChartEnums::MeasureOrientationMinimum,
                      Qt::AlignLeft )
            : 0;
        TextLayoutItem* labelItem2 =
            drawLabels
            ? new TextLayoutItem( QString::number( minValueY ),
                      labelTA,
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
                int iLabel = 0;
                for ( qreal i = minValueX; i < maxValueX && !labelsAreOverlapping; i += dimX.stepWidth )
                {
                    if ( dimX.stepWidth != 1.0 && ! dim.isCalculated )
                    {
                        labelItem->setText( QString::number( i, 'f', 0 ) );
                        labelItem2->setText( QString::number( i + dimX.stepWidth, 'f', 0 ) );
                    } else {
                        labelItem->setText( labels()[ iLabel ] );
                        labelItem->setText( labels()[ iLabel + 1 >= hardLabelsCount ? 0 : iLabel + 1 ] );
                    }
                    QPointF firstPos( i, 0.0 );
                    firstPos = plane->translate( firstPos );
                    firstPos.setX( firstPos.x() - met.height() );
                    firstPos.setY( fourthRulerRef.y() + tickLength );
                    firstPos.setY( isTop ? firstPos.y() - met.height() : firstPos.y() + ( 2.0 * met.height() ) );

                    QPointF secondPos( i + dimX.stepWidth, 0.0 );
                    secondPos = plane->translate( secondPos );
                    secondPos.setX( secondPos.x() - met.height() );
                    secondPos.setY( fourthRulerRef.y() + tickLength );
                    secondPos.setY( isTop ? secondPos.y() - met.height() : secondPos.y() + ( 2.0 * met.height() ) );

                    labelsAreOverlapping = labelItem->intersects( *labelItem2, firstPos, secondPos );

                    if ( iLabel++ > hardLabelsCount - 1 )
                        iLabel = 0;
                }

                useShortLabels = labelsAreOverlapping;

            }

            int iLabel = 0;
            for ( qreal i = minValueX; i < maxValueX; i += dimX.stepWidth ) {
                QPointF topPoint ( i + ( useItemCountLabels ? 0.5 : 0.0 ), 0.0 );
                QPointF bottomPoint ( topPoint );
                topPoint = plane->translate( topPoint );
                bottomPoint = plane->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
                if ( drawLabels ) {
                    if( (dimX.stepWidth != 1.0) && ! dimX.isCalculated )
                        labelItem->setText( QString::number(i, 'f', 0) );
                    else
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
                                    static_cast<int>( topPoint.y() + ( isBottom ? (met.height() * 0.5) : (size.height() * -1.0) ) ) ),
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
                        minValueX += dimX.stepWidth;
                    }
                }
            }
        } else {
            double maxLimit, steg;
            bool percent = d->diagram()->percentMode();
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
                    d->diagram()->percentMode() ? labelValue += 10.0 : labelValue += dimY.stepWidth;
                }
            }
            labelValue = minValueY;
//qDebug("minValueY: %f   maxLimit: %f   steg: %f", minValueY, maxLimit, steg);
            for ( qreal f = minValueY; f <= maxLimit; f+= steg ) {
//qDebug("f: %f",f);
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

                    d->diagram()->percentMode() ? labelValue += 10.0 : labelValue += dimY.stepWidth;
                }
            }
            //Pending Michel: reset to default - is that really what we want?
            //d->diagram()->setPercentMode( false );
        }
        if( labelItem )
            delete labelItem;
    }

    if( ! titleText().isEmpty() ){
        const TextAttributes titleTA( titleTextAttributes() );
        TextLayoutItem titleItem( titleText(),
                      titleTA,
                      referenceArea,
                      KDChartEnums::MeasureOrientationMinimum,
                      Qt::AlignHCenter|Qt::AlignVCenter );
        QPointF point;
        const QSize size( titleItem.sizeHint() );
        if( isTop ){
            point.setX( geoRect.left() + geoRect.width() / 2.0);
            point.setY( geoRect.top() );
        }else if( isBottom ){
            point.setX( geoRect.left() + geoRect.width() / 2.0);
            point.setY( geoRect.bottom() - size.height() );
        }else if( isLeft ){
            point.setX( geoRect.left() );
            point.setY( geoRect.top() + geoRect.height() / 2.0);
        }else /*if( isRight )*/ {
            point.setX( geoRect.right() - size.height() );
            point.setY( geoRect.top() + geoRect.height() / 2.0);
        }
        PainterSaver painterSaver( ptr );
        ptr->translate( point );
        if( isOrdinate() )
            ptr->rotate( 270.0 );
        titleItem.setGeometry( QRect( QPoint(-size.width() / 2, 0), size ) );
        titleItem.paint( ptr );
    }

    //qDebug() << "KDChart::CartesianAxis::paintCtx() done.";
}
#undef ptr

QDomDocumentFragment CartesianAxis::toXML() const
{
    Q_ASSERT_X ( false, "CartesianAxis::toXML()", "not implemented" );
    return QDomDocumentFragment();
}

/* pure virtual in QLayoutItem */
bool CartesianAxis::isEmpty() const
{
    return false; // if the axis exists, it has some (perhaps default) content
}
/* pure virtual in QLayoutItem */
Qt::Orientations CartesianAxis::expandingDirections() const
{
    Qt::Orientations ret;
    switch ( position() )
    {
    case Bottom:
    case Top:
        ret = Qt::Horizontal;
        break;
    case Left:
    case Right:
        ret = Qt::Vertical;
        break;
    default:
        Q_ASSERT( false ); // all positions need to be handeld
        break;
    };
    return ret;
}
/* pure virtual in QLayoutItem */
QSize CartesianAxis::maximumSize() const
{
    // FIXME return real values:
    QSize result;

    const TextAttributes titleTA( titleTextAttributes() );
    const TextAttributes labelTA = textAttributes();
    TextLayoutItem titleItem( titleText(), titleTA, 0, KDChartEnums::MeasureOrientationMinimum, Qt::AlignHCenter | Qt::AlignVCenter );
    TextLayoutItem labelItem( "", labelTA, 0, KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft );
    int h = 0;

    switch ( position() )
    {
    case Bottom:
    case Top:
        // enough space for the labels to fit:
        for ( int i = 0; i < labels().count(); ++i )
        {
            labelItem.setText( labels()[ i ] );
            int lh = labelItem.sizeHint().height();
            if ( h < lh )
                h = lh;
        }
        // space for a possible title:
        if ( ! titleText().isEmpty() ) {
            h += titleItem.sizeHint().height();
        }
        result = QSize ( 10, h + 20 );
        break;
    case Left:
    case Right:
        // enough space for the labels to fit:
        for ( int i = 0; i < labels().count(); ++i )
        {
            labelItem.setText( labels()[ i ] );
            int lh = labelItem.sizeHint().width();
            if ( h < lh )
                h = lh;
        }
        // space for a possible title:
        if ( ! titleText().isEmpty() ) {
            h += titleItem.sizeHint().width();
        }
        result = QSize ( h + 20, 10 );
        break;
    default:
        Q_ASSERT( false ); // all positions need to be handled
        break;
    };

    return result;
}
/* pure virtual in QLayoutItem */
QSize CartesianAxis::minimumSize() const
{
    return maximumSize();
}
/* pure virtual in QLayoutItem */
QSize CartesianAxis::sizeHint() const
{
    return maximumSize();
}
/* pure virtual in QLayoutItem */
void CartesianAxis::setGeometry( const QRect& r )
{
//    qDebug() << "KDChart::CartesianAxis::setGeometry(" << r << ") called"
//             << (isAbscissa() ? "for Abscissa":"for Ordinate") << "axis";
    d->geometry = r;
}
/* pure virtual in QLayoutItem */
QRect CartesianAxis::geometry() const
{
    return d->geometry;
}
