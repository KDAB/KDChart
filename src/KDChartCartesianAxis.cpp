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

    //qDebug() << "CartesianAxis::paint( QPainter* painter )  " << " areaGeometry()():" << rect << " sizeHint():" << sizeHint();

    ctx.setRectangle(
        QRectF (
            //QPointF(0, 0),
            QPointF(rect.left(), rect.top()),
            QSizeF(rect.width(), rect.height() ) ) );
    // enabling clipping so that we're not drawing outside
    QRegion clipRegion( rect );
    painter->setClipRegion( clipRegion );
    paintCtx( &ctx );
    painter->setClipping( false );
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

/*    const bool isLeft   = position() == Left;
    const bool isRight  = position() == Right;
    const bool isTop    = position() == Top;
    const bool isBottom = position() == Bottom;*/

    // preparations:
    // - calculate the range that will be displayed:
    const qreal absRange = qAbs( dim.distance() );

    // Fixme Michel: Need to find the type of chart here - Line or Bar
    // if Bars calculate the number of groups

    qreal numberOfUnitRulers;
    if ( isAbscissa() )
        numberOfUnitRulers = d->diagram()->model()->rowCount() - 1.0;
    else {
        numberOfUnitRulers = absRange / qAbs( dimY.stepWidth ) + 1.0;
        //qDebug() << "absRange" << absRange << "dimY.stepWidth:" << dimY.stepWidth << "numberOfUnitRulers:" << numberOfUnitRulers;
    }
    qreal numberOfSubUnitRulers;
    if ( isAbscissa() )
        numberOfSubUnitRulers = 0.0;
    else {
        numberOfSubUnitRulers = absRange / qAbs( dimY.subStepWidth ) + 1.0;
        //qDebug() << "dimY.subStepWidth:" << dimY.stepWidth << "numberOfSubUnitRulers:" << numberOfSubUnitRulers;
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

    //for debugging: if( isAbscissa() )ptr->drawRect(geoRect.adjusted(0,0,-1,-1));
    //qDebug() << "         " << (isAbscissa() ? "Abscissa":"Ordinate") << "axis painting with geometry" << geoRect;

    // FIXME references are of course different for all locations:
    rulerWidth = geoRect.width();
    rulerHeight =  geoRect.height();
    switch( position() )
    {
    case Top:
        rulerRef.setX( geoRect.topLeft().x() );
        rulerRef.setY( geoRect.topLeft().y() + rulerHeight );
        break;
    case Bottom:
        rulerRef.setX( geoRect.bottomLeft().x() );
        rulerRef.setY( geoRect.bottomLeft().y() - rulerHeight );
        break;
    case Right:
        rulerRef.setX( geoRect.bottomRight().x() - rulerWidth );
        rulerRef.setY( geoRect.bottomRight().y() );
        break;
    case Left:
        rulerRef.setX( geoRect.bottomLeft().x() + rulerWidth );
        rulerRef.setY( geoRect.bottomLeft().y() );
        break;
    }

    // set up the lines to paint:

    // set up a map of integer positions,

    // - starting with the fourth
    // - the the halfs
    // - then the tens
    // this will override all halfs and fourth that hit a higher-order ruler
    // MAKE SURE TO START AT (0, 0)!

    // set up a reference point,  a step vector and a unit vector for the drawing:

    const qreal minValueY = dimY.start;
    const qreal maxValueY = dimY.end;
    const qreal minValueX = dimX.start;
    const qreal maxValueX = dimX.end;
//#define AXES_PAINTING_DEBUG 1
    #ifdef AXES_PAINTING_DEBUG
    qDebug() << "CartesianAxis::paint: reference values:" << endl
            << "-- range x/y: " << dimX.distance() << "/" << dimY.distance() << endl
            << "-- absRange: " << absRange << endl
            << "-- numberOfUnitRulers: " << numberOfUnitRulers << endl
            << "-- screenRange: " << screenRange << endl
            << "-- drawUnitRulers: " << drawUnitRulers << endl
            << "-- drawLabels: " << drawLabels << endl
            << "-- ruler reference point:: " << rulerRef << endl
            << "-- minValueX: " << minValueX << "   maxValueX: " << maxValueX << endl
            << "-- minValueY: " << minValueY << "   maxValueY: " << maxValueY << endl
            ;
    #endif

    ptr->setPen ( Qt::black );
    // Commenting this it causes the frame backround to be painted in red
    // for some reason and does not look to make sense.
    //ptr->setBrush ( Qt::red ); // PENDING(michel) What is this for?
    QPointF fourthRulerRef ( rulerRef );

    // this draws the subunit rulers
    if ( drawSubUnitRulers ) {
        // for the x-axis
        if ( isAbscissa() ) {
            for ( float f = minValueX; f <= maxValueX; f += dimX.subStepWidth ) {
                QPointF topPoint ( f, 0 );
                QPointF bottomPoint ( f, 0 );
                topPoint = plane->translate( topPoint );
                bottomPoint = plane->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength( true ) );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
            }
        // for the y-axis
        } else {
            for ( float f = minValueY; f <= maxValueY; f += dimY.subStepWidth ) {
                QPointF leftPoint ( 0, f );
                QPointF rightPoint ( 0, f );
                leftPoint = plane->translate( leftPoint );
                rightPoint = plane->translate( rightPoint );
                leftPoint.setX( fourthRulerRef.x() + tickLength( true ) );
                rightPoint.setX( fourthRulerRef.x() );
                ptr->drawLine( leftPoint, rightPoint );
          }
      }
    }

    const QObject* referenceArea = plane->parent();

    // this draws the unit rulers
    if ( drawUnitRulers ) {
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
        const qreal halfFontHeight = met.height() * 0.5;

        if ( isAbscissa() ) {
            // If we have a labels list AND a short labels list, we first find out,
            // if there is enough space for the labels: if not, use the short labels.
            if( drawLabels && hardLabelsCount > 0 && shortLabelsCount > 0 ){
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

                    QPointF secondPos( i + dimX.stepWidth, 0.0 );
                    secondPos = plane->translate( secondPos );

                    labelsAreOverlapping = labelItem->intersects( *labelItem2, firstPos, secondPos );

                    if ( iLabel++ > hardLabelsCount - 1 )
                        iLabel = 0;
                }

                useShortLabels = labelsAreOverlapping;
            }

            labelItem2->setText( "" );
            QPoint oldItemPos;
            int idxLabel = 0;
            qreal iLabelF = minValueX;
            for ( qreal i = minValueX; i < maxValueX; i += dimX.stepWidth ) {
                QPointF topPoint ( i + ( useItemCountLabels ? 0.5 : 0.0 ), 0.0 );
                QPointF bottomPoint ( topPoint );
                topPoint = plane->translate( topPoint );
                bottomPoint = plane->translate( bottomPoint );
                topPoint.setY( fourthRulerRef.y() + tickLength() );
                bottomPoint.setY( fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
                if ( drawLabels ) {
                    if( (dimX.stepWidth != 1.0) && ! dimX.isCalculated )
                        labelItem->setText( QString::number(i, 'f', 0) );
                    else
                        labelItem->setText( hardLabelsCount
                            ? ( useShortLabels    ? shortLabels()[ idxLabel ] : labels()[ idxLabel ] )
                            : ( headerLabelsCount ? headerLabels[  idxLabel ] : QString::number( iLabelF ) ) );
                    // No need to call labelItem->setParentWidget(), since we are using
                    // the layout item temporarily only.
                    const QSize size( labelItem->sizeHint() );
                    labelItem->setGeometry(
                            QRect(
                                QPoint(
                                    static_cast<int>( topPoint.x() - size.width() / 2 ),
                                    static_cast<int>( topPoint.y() +
                                        ( position() == Bottom
                                          ? halfFontHeight
                                          : ((halfFontHeight + size.height()) * -1.0) ) ) ),
                                size ) );

                    if ( ! labelItem2->intersects( *labelItem, oldItemPos, labelItem->geometry().topLeft() ) )
                    {
                        labelItem->paint( ptr );

                        labelItem2->setText( labelItem->text() );
                        oldItemPos = labelItem->geometry().topLeft();
                    }

                    if( hardLabelsCount ){
                        if( idxLabel >= hardLabelsCount  -1 )
                            idxLabel = 0;
                        else
                            ++idxLabel;
                    }else if( headerLabelsCount ){
                        if( idxLabel >= headerLabelsCount-1 )
                            idxLabel = 0;
                        else
                            ++idxLabel;
                    }else{
                        iLabelF += dimX.stepWidth;
                    }
                }
            }
        } else {
            const double maxLimit = maxValueY;
            const double steg = dimY.stepWidth;
            int maxLabelsWidth = 0;
            qreal labelValue;
            if( drawLabels && position() == Right ){
                // Find the wides label, so we to know how much we need to right-shift
                // our labels, to get them drawn right aligned:
                labelValue = minValueY;
                for ( qreal f = minValueY; f <= maxLimit; f+= steg ) {
                    labelItem->setText( QString::number( labelValue ) );
                    maxLabelsWidth = qMax( maxLabelsWidth, labelItem->sizeHint().width() );
                    labelValue += dimY.stepWidth;
                }
            }
            labelValue = minValueY;
//qDebug("minValueY: %f   maxLimit: %f   steg: %f", minValueY, maxLimit, steg);
            for ( qreal f = minValueY; f <= maxLimit; f+= steg ) {
//qDebug("f: %f",f);
                QPointF leftPoint (  0.0, f );
                QPointF rightPoint ( 0.0, f );
                leftPoint  = plane->translate( leftPoint );
                rightPoint = plane->translate( rightPoint );
                leftPoint.setX( fourthRulerRef.x() + tickLength() );
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
                        static_cast<int>( leftPoint.x() + met.height() * ( position() == Left ? -0.5 : 0.5) )
                        - ( position() == Left ? labelSize.width() : (labelSize.width() - maxLabelsWidth) );
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
                    //ptr->drawRect(labelItem->geometry().adjusted(0,0,-1,-1));
                    labelItem->paint( ptr );

                    labelValue += dimY.stepWidth;
                }
            }
        }
        if( labelItem )
            delete labelItem;
        if( labelItem2 )
            delete labelItem2;
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
        switch( position() )
        {
        case Top:
            point.setX( geoRect.left() + geoRect.width() / 2.0);
            point.setY( geoRect.top() );
            break;
        case Bottom:
            point.setX( geoRect.left() + geoRect.width() / 2.0);
            point.setY( geoRect.bottom() - size.height() );
            break;
        case Left:
            point.setX( geoRect.left() );
            point.setY( geoRect.top() + geoRect.height() / 2.0);
            break;
        case Right:
            point.setX( geoRect.right() - size.height() );
            point.setY( geoRect.top() + geoRect.height() / 2.0);
            break;
        }
        PainterSaver painterSaver( ptr );
        ptr->translate( point );
        if( isOrdinate() )
            ptr->rotate( 270.0 );
        titleItem.setGeometry( QRect( QPoint(-size.width() / 2, 0), size ) );
        //ptr->drawRect(titleItem.geometry().adjusted(0,0,-1,-1));
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
    QSize result;

    const TextAttributes titleTA( titleTextAttributes() );
    const TextAttributes labelTA = textAttributes();
    const bool drawLabels = labelTA.isVisible();
    const bool drawTitle  = ! titleText().isEmpty();
    AbstractCoordinatePlane* plane = d->diagram()->coordinatePlane();
    QObject* refArea = plane->parent();
    TextLayoutItem labelItem( "", labelTA, refArea,
        KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft );
    TextLayoutItem titleItem( titleText(), titleTA, refArea,
        KDChartEnums::MeasureOrientationMinimum, Qt::AlignHCenter | Qt::AlignVCenter );
    const qreal labelGap =
        drawLabels
        ? (QFontMetricsF( labelItem.realFont() ).height() / 3.0)
        : 0.0;
    const qreal titleGap =
        drawTitle
        ? (QFontMetricsF( titleItem.realFont() ).height() / 3.0)
        : 0.0;

    switch ( position() )
    {
    case Bottom:
    case Top: {
            qreal h = 0;
            if( drawLabels ){
                // if there're no label strings, we take the biggest needed number as height
                if ( ! labels().count() )
                {
                    labelItem.setText( QString::number( plane->gridDimensionsList().first().end, 'f', 0 ) );
                    h = labelItem.sizeHint().height();
                }else{
                    // find the longest label text:
                    for ( int i = 0; i < labels().count(); ++i )
                    {
                        labelItem.setText( labels()[ i ] );
                        qreal lh = labelItem.sizeHint().height();
                        h = qMax( h, lh );
                    }
                }
                // we leave a little gap between axis labels and bottom (or top, resp.) side of axis
                h += labelGap;
            }
            // space for a possible title:
            if ( drawTitle ) {
                // we add the title height and leave a little gap between axis labels and axis title
                h += titleItem.sizeHint().height() + titleGap;
            }
            // space for the ticks
            h += qAbs( tickLength() ) * 3.0;
            result = QSize ( 10, static_cast<int>( h ) );
        }
        break;
    case Left:
    case Right: {
            qreal w = 0;
            if( drawLabels ){
                // if there're no label strings, we take the biggest needed number as width
                if ( labels().count() == 0 )
                {
                    labelItem.setText( QString::number( plane->gridDimensionsList().last().end, 'f', 0 ) );
                    w = labelItem.sizeHint().width();
                }else{
                    // find the longest label text:
                    for ( int i = 0; i < labels().count(); ++i )
                    {
                        labelItem.setText( labels()[ i ] );
                        qreal lw = labelItem.sizeHint().width();
                        w = qMax( w, lw );
                    }
                }
                // we leave a little gap between axis labels and left (or right, resp.) side of axis
                w += labelGap;
            }
            // space for a possible title:
            if ( drawTitle ) {
                // we add the title height and leave a little gap between axis labels and axis title
                w += titleItem.sizeHint().height() + titleGap;
            }
            // space for the ticks
            w += qAbs( tickLength() ) * 3.0;

            result = QSize ( static_cast<int>( w ), 10 );
//            qDebug() << "left/right axis width:" << result << "   w:" << w;
        }
        break;
    default:
        Q_ASSERT( false ); // all positions need to be handled
        break;
    };
//qDebug() << "*******************" << result;
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

int CartesianAxis::tickLength( bool subUnitTicks ) const
{
    int result = 0;

    if ( isAbscissa() ) {
        result = position() == Top ? -4 : 3;
    } else {
        result = position() == Left ? -4 : 3;
    }

    if ( subUnitTicks )
        result = result < 0 ? result + 1 : result - 1;

    return result;
}
