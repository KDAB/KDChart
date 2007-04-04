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
 **   information about KD Chart Commercial License Agreements.
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
#include "KDChartBarDiagram.h"

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
    // when we remove the first axis it will unregister itself and
    // propagate the next one to the primary, thus the while loop
    while ( d->mDiagram ) {
        AbstractCartesianDiagram *cd = qobject_cast<AbstractCartesianDiagram*>( d->mDiagram );
        cd->takeAxis( this );
    }
    Q_FOREACH( AbstractDiagram *diagram, d->secondaryDiagrams ) {
        AbstractCartesianDiagram *cd = qobject_cast<AbstractCartesianDiagram*>( diagram );
        cd->takeAxis( this );
    }
}

void CartesianAxis::init ()
{
    d->position = Bottom;
}

void CartesianAxis::setTitleText( const QString& text )
{
    //FIXME(khz): Call update al all places where axis internals are changed!
    d->titleText = text;
    layoutPlanes();
}

QString CartesianAxis::titleText() const
{
    return d->titleText;
}

void CartesianAxis::setTitleTextAttributes( const TextAttributes &a )
{
    d->titleTextAttributes = a;
    d->useDefaultTextAttributes = false;
    layoutPlanes();
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
    layoutPlanes();
}

bool CartesianAxis::hasDefaultTitleTextAttributes() const
{
    return d->useDefaultTextAttributes;
}


void CartesianAxis::setPosition ( Position p )
{
    d->position = p;
    layoutPlanes();
}

const CartesianAxis::Position CartesianAxis::position() const
{
    return d->position;
}

void CartesianAxis::layoutPlanes()
{
    //qDebug() << "CartesianAxis::layoutPlanes()";
    if( ! d->diagram() || ! d->diagram()->coordinatePlane() ) {
        //qDebug() << "CartesianAxis::layoutPlanes(): Sorry, found no plane.";
        return;
    }
    AbstractCoordinatePlane* plane = d->diagram()->coordinatePlane();
    if( plane ){
        plane->layoutPlanes();
        //qDebug() << "CartesianAxis::layoutPlanes() OK";
    }
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
    QRegion clipRegion( rect.adjusted( -1, -1, 1, 1 ) );
    painter->save();
    painter->setClipRegion( clipRegion );
    paintCtx( &ctx );
    painter->restore();
    //qDebug() << "KDChart::CartesianAxis::paint() done.";
}

void CartesianAxis::Private::drawSubUnitRulers( QPainter* painter, CartesianCoordinatePlane* plane, const DataDimension& dim,
                                                const QPointF& rulerRef, const QVector<int>& drawnTicks ) const
{
    const QRect geoRect( axis()->geometry() );
    int nextMayBeTick = 0;
    int mayBeTick = 0;
    int logSubstep = 0;
    float f = dim.start;
    qreal fLogSubstep = f;
    const bool isAbscissa = axis()->isAbscissa();
    const bool isLogarithmic = (dim.calcMode == AbstractCoordinatePlane::Logarithmic );
    const int subUnitTickLength = axis()->tickLength( true );
    while ( f <= dim.end ) {
        if( drawnTicks.count() > nextMayBeTick )
            mayBeTick = drawnTicks[ nextMayBeTick ];
        if ( isAbscissa ) {
            // for the x-axis
            QPointF topPoint ( f, 0 );
            QPointF bottomPoint ( f, 0 );
            // we don't draw the sub ticks, if we are at the same position as a normal tick
            topPoint = plane->translate( topPoint );
            bottomPoint = plane->translate( bottomPoint );
            topPoint.setY( rulerRef.y() + subUnitTickLength );
            bottomPoint.setY( rulerRef.y() );
            if( qAbs( mayBeTick - topPoint.x() ) > 1 )
                painter->drawLine( topPoint, bottomPoint );
            else {
                ++nextMayBeTick;
            }
        } else {
            // for the y-axis
            QPointF leftPoint = plane->translate( QPointF( 0, f ) );
            //qDebug() << "geoRect:" << geoRect << "   geoRect.top()" << geoRect.top() << "geoRect.bottom()" << geoRect.bottom() << "  translatedValue:" << translatedValue;
            // we don't draw the sub ticks, if we are at the same position as a normal tick
            if( qAbs( mayBeTick - leftPoint.y() ) > 1 ){
                const qreal translatedValue = leftPoint.y();
                if( translatedValue > geoRect.top() && translatedValue <= geoRect.bottom() ){
                    QPointF rightPoint ( 0, f );
                    rightPoint = plane->translate( rightPoint );
                    leftPoint.setX( rulerRef.x() + subUnitTickLength );
                    rightPoint.setX( rulerRef.x() );
                    painter->drawLine( leftPoint, rightPoint );
                }
            } else {
                ++nextMayBeTick;
            }
        }
        if ( isLogarithmic ){
            if( logSubstep == 9 ){
                fLogSubstep *= 10.0;
                logSubstep = 0;
            }
            f += fLogSubstep;
            ++logSubstep;
        }else{
            f += dim.subStepWidth;
        }
    }
}

void CartesianAxis::Private::drawTitleText( QPainter* painter, CartesianCoordinatePlane* plane, const QRect& areaGeoRect ) const
{
    const TextAttributes titleTA( axis()->titleTextAttributes() );
    if( titleTA.isVisible() ) {
        TextLayoutItem titleItem( titleText,
                                  titleTA,
                                  plane->parent(),
                                  KDChartEnums::MeasureOrientationMinimum,
                                  Qt::AlignHCenter|Qt::AlignVCenter );
        QPointF point;
        const QSize size( titleItem.sizeHint() );
        //FIXME(khz): We definitely need to provide a way that users can decide
        //            the position of an axis title.
        switch( position )
        {
        case Top:
            point.setX( areaGeoRect.left() + areaGeoRect.width() / 2.0);
            point.setY( areaGeoRect.top() );
            break;
        case Bottom:
            point.setX( areaGeoRect.left() + areaGeoRect.width() / 2.0);
            point.setY( areaGeoRect.bottom() - size.height() );
            break;
        case Left:
            point.setX( areaGeoRect.left() );
            point.setY( areaGeoRect.top() + areaGeoRect.height() / 2.0);
            break;
        case Right:
            point.setX( areaGeoRect.right() - size.height() );
            point.setY( areaGeoRect.top() + areaGeoRect.height() / 2.0);
            break;
        }
        PainterSaver painterSaver( painter );
        painter->translate( point );
        if( axis()->isOrdinate() )
            painter->rotate( 270.0 );
        titleItem.setGeometry( QRect( QPoint(-size.width() / 2, 0), size ) );
        //ptr->drawRect(titleItem.geometry().adjusted(0,0,-1,-1));
        titleItem.paint( painter );
    }
}

void CartesianAxis::paintCtx( PaintContext* context )
{

    Q_ASSERT_X ( d->diagram(), "CartesianAxis::paint",
                 "Function call not allowed: The axis is not assigned to any diagram." );

    CartesianCoordinatePlane* plane = dynamic_cast<CartesianCoordinatePlane*>(context->coordinatePlane());
    Q_ASSERT_X ( plane, "CartesianAxis::paint",
                 "Bad function call: PaintContext::coodinatePlane() NOT a cartesian plane." );

    /*
     * let us paint the labels at a
     * smaller resolution
     * Same mini pixel value as for
     * Cartesian Grid
     */
    //const qreal MinimumPixelsBetweenRulers = 1.0;
    DataDimensionsList dimensions( plane->gridDimensionsList() );
    //qDebug("CartesianAxis::paintCtx() gets DataDimensionsList.first():   start: %f   end: %f   stepWidth: %f", dimensions.first().start, dimensions.first().end, dimensions.first().stepWidth);

    // test for programming errors: critical
    Q_ASSERT_X ( dimensions.count() == 2, "CartesianAxis::paint",
                 "Error: plane->gridDimensionsList() did not return exactly two dimensions." );
    DataDimension& dimX = dimensions.first();
    const DataDimension& dimY = dimensions.last();
    const DataDimension& dim = (isAbscissa() ? dimensions.first() : dimensions.last());

    /*
    if(isAbscissa())
        qDebug() << "         " << "Abscissa:" << dimX.start <<".."<<dimX.end <<"  step"<<dimX.stepWidth;
    else
        qDebug() << "         " << "Ordinate:" << dimY.start <<".."<<dimY.end <<"  step"<<dimY.stepWidth;
    */


    /*
     * let us paint the labels at a
     * smaller resolution
     * Same mini pixel value as for
     * Cartesian Grid
     */
    const qreal MinimumPixelsBetweenRulers = qMin(  dimX.stepWidth,  dimY.stepWidth );//1.0;

    // preparations:
    // - calculate the range that will be displayed:
    const qreal absRange = qAbs( dim.distance() );

    qreal numberOfUnitRulers;
    if ( isAbscissa() ) {
        if( dimX.isCalculated )
            numberOfUnitRulers = absRange / qAbs( dimX.stepWidth ) + 1.0;
        else
            numberOfUnitRulers = d->diagram()->model()->rowCount() - 1.0;
    }else{
        numberOfUnitRulers = absRange / qAbs( dimY.stepWidth ) + 1.0;

    }

    //    qDebug() << "absRange" << absRange << "dimY.stepWidth:" << dimY.stepWidth << "numberOfUnitRulers:" << numberOfUnitRulers;

    qreal numberOfSubUnitRulers;
    if ( isAbscissa() ){
        if( dimX.isCalculated )
            numberOfSubUnitRulers = absRange / qAbs( dimX.subStepWidth ) + 1.0;
        else
            numberOfSubUnitRulers = dimX.subStepWidth>0 ? absRange / qAbs( dimX.subStepWidth ) + 1.0 : 0.0;
    }else{
        numberOfSubUnitRulers = absRange / qAbs( dimY.subStepWidth ) + 1.0;
    }

    // - calculate the absolute range in screen pixels:
    const QPointF p1 = plane->translate( QPointF(dimX.start, dimY.start) );
    const QPointF p2 = plane->translate( QPointF(dimX.end,   dimY.end) );

    double screenRange;
    if ( isAbscissa() )
    {
        screenRange = qAbs ( p1.x() - p2.x() );
    } else {
        screenRange = qAbs ( p1.y() - p2.y() );
    }

    const bool useItemCountLabels = isAbscissa() && ! dimX.isCalculated;
    //qDebug() << "CartesianAxis::paintCtx useItemCountLabels "<< useItemCountLabels;

    //qDebug() << "isAbscissa():" << isAbscissa() << "   dimX.isCalculated:" << dimX.isCalculated << "   dimX.stepWidth: "<<dimX.stepWidth;
    //FIXME(khz): Remove this code, and do the calculation in the grid calc function
    if( isAbscissa() && ! dimX.isCalculated ){
        // dont ignore the users settings
        dimX.stepWidth = dimX.stepWidth ? dimX.stepWidth : 1.0;
        //qDebug() << "screenRange / numberOfUnitRulers <= MinimumPixelsBetweenRulers" << screenRange <<"/" << numberOfUnitRulers <<"<=" << MinimumPixelsBetweenRulers;
        while( screenRange / numberOfUnitRulers <= MinimumPixelsBetweenRulers ){
            dimX.stepWidth *= 10.0;
            dimX.subStepWidth  *= 10.0;
            numberOfUnitRulers = qAbs( dimX.distance() / dimX.stepWidth );
        }
    }

    const bool drawUnitRulers = screenRange / ( numberOfUnitRulers / dimX.stepWidth ) > MinimumPixelsBetweenRulers;
    const bool drawSubUnitRulers =
        (numberOfSubUnitRulers != 0.0) &&
        (screenRange / numberOfSubUnitRulers > MinimumPixelsBetweenRulers);

    const TextAttributes labelTA = textAttributes();
    const bool drawLabels = labelTA.isVisible();

    // - find the reference point at which to start drawing and the increment (line distance);
    QPointF rulerRef;
    const QRect areaGeoRect( areaGeometry() );
    const QRect geoRect( geometry() );
    QRectF rulerRect;
    double rulerWidth;
    double rulerHeight;

    QPainter* ptr = context->painter();

    //for debugging: if( isAbscissa() )ptr->drawRect(areaGeoRect.adjusted(0,0,-1,-1));
    //qDebug() << "         " << (isAbscissa() ? "Abscissa":"Ordinate") << "axis painting with geometry" << areaGeoRect;

    // FIXME references are of course different for all locations:
    rulerWidth = areaGeoRect.width();
    rulerHeight =  areaGeoRect.height();
    switch( position() )
    {
    case Top:
        rulerRef.setX( areaGeoRect.topLeft().x() );
        rulerRef.setY( areaGeoRect.topLeft().y() + rulerHeight );
        break;
    case Bottom:
        rulerRef.setX( areaGeoRect.bottomLeft().x() );
        rulerRef.setY( areaGeoRect.bottomLeft().y() - rulerHeight );
        break;
    case Right:
        rulerRef.setX( areaGeoRect.bottomRight().x() - rulerWidth );
        rulerRef.setY( areaGeoRect.bottomRight().y() );
        break;
    case Left:
        rulerRef.setX( areaGeoRect.bottomLeft().x() + rulerWidth );
        rulerRef.setY( areaGeoRect.bottomLeft().y() );
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
    const bool isLogarithmicX = (dimX.calcMode == AbstractCoordinatePlane::Logarithmic );
    const bool isLogarithmicY = (dimY.calcMode == AbstractCoordinatePlane::Logarithmic );
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

    const QObject* referenceArea = plane->parent();

    // that QVector contains all drawn x-ticks (so no subticks are drawn there also)
    QVector< int > drawnXTicks;
    // and that does the same for the y-ticks
    QVector< int > drawnYTicks;

    /*
     * Find out if it is a bar diagram
     * bar diagrams display their data per column
     * we need to handle the last label another way
     * 1 - Last label == QString null ( Header Labels )
     * 2 - Display labels and ticks in the middle of the column
     */

    bool isBarDiagram;
    AbstractDiagram * const dia = const_cast<AbstractDiagram*>( d->diagram() );
    if ( qobject_cast< BarDiagram* >( dia ) )
        isBarDiagram = true;
    else
        isBarDiagram = false;

    // this draws the unit rulers
    if ( drawUnitRulers ) {
        const int hardLabelsCount  = labels().count();
        const int shortLabelsCount = shortLabels().count();
        bool useShortLabels = false;


        bool useConfiguredStepsLabels = false;
        QStringList headerLabels;
        if( useItemCountLabels ){
            //qDebug() << (isOrdinate() ? "is Ordinate" : "is Abscissa");
            headerLabels =
                isOrdinate()
                ? d->diagram()->datasetLabels()
                : d->diagram()->itemRowLabels();
            // check if configured stepWidth
            useConfiguredStepsLabels = isAbscissa() &&
                    dimX.stepWidth &&
                    (( (headerLabels.count() - 1)/ dimX.stepWidth ) != numberOfUnitRulers);
            if( useConfiguredStepsLabels ) {
                numberOfUnitRulers = ( headerLabels.count() - 1 )/ dimX.stepWidth;
                // we need to register data values for the steps
                // in case it is configured by the user
                QStringList configuredStepsLabels;
                double value = headerLabels.first().toDouble();
                configuredStepsLabels << QString::number( value );
                for (  int i = 0; i < numberOfUnitRulers; i++ ) {
                    value += dimX.stepWidth;
                    configuredStepsLabels.append( QString::number( value ) );
                }
                headerLabels = configuredStepsLabels;
            }

            if (  isBarDiagram )
                headerLabels.append( QString::null );
        }


        const int headerLabelsCount = headerLabels.count();
        //qDebug() << "headerLabelsCount" << headerLabelsCount;

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
                qreal i = minValueX;
                while ( i < maxValueX && !labelsAreOverlapping )
                {
                    if ( dimX.stepWidth != 1.0 && ! dim.isCalculated )
                    {
                        labelItem->setText( QString::number( i, 'f', 0 ) );
                        labelItem2->setText( QString::number( i + dimX.stepWidth, 'f', 0 ) );
                    } else {

                        int index = iLabel;
                        labelItem->setText( labels()[ index < hardLabelsCount ? index : 0 ] );
                        labelItem2->setText( labels()[ index < hardLabelsCount - 1 ? index + 1 : 0] );
                    }
                    QPointF firstPos( i, 0.0 );
                    firstPos = plane->translate( firstPos );

                    QPointF secondPos( i + dimX.stepWidth, 0.0 );
                    secondPos = plane->translate( secondPos );

                    labelsAreOverlapping = labelItem->intersects( *labelItem2, firstPos, secondPos );
                    if ( iLabel++ > hardLabelsCount - 1 )
                        iLabel = 0;
                    if ( isLogarithmicX )
                        i *= 10.0;
                    else
                        i += dimX.stepWidth;

                }

                useShortLabels = labelsAreOverlapping;
            }

            qreal labelDiff = dimX.stepWidth;
            //qDebug() << "labelDiff " << labelDiff;
            if ( drawLabels )
            {

                qreal i = minValueX;
                int iLabel = 0;
                const int precision = ( QString::number( labelDiff  ).section( QLatin1Char('.'), 1,  2 ) ).length();

                while ( i + labelDiff < maxValueX )
                {

                    //qDebug() << "drawLabels" << drawLabels << "  hardLabelsCount" << hardLabelsCount
                    //        << "  dimX.stepWidth" << dimX.stepWidth << "  dim.isCalculated" << dim.isCalculated;
                    if ( !drawLabels || hardLabelsCount < 1 || ( dimX.stepWidth != 1.0 && ! dim.isCalculated ) )
                    {
                        // Check intersects for the header label - we need to pass the full string
                        // here and not only the i value.
                        if( useConfiguredStepsLabels ){
                            labelItem->setText( headerLabels[ iLabel   ] );
                            labelItem2->setText(headerLabels[ iLabel+1 ] );
                        }else{
                            //qDebug() << "i + labelDiff " << i + labelDiff;
                            labelItem->setText( headerLabelsCount ? headerLabels[static_cast<int>(i)]
                                : QString::number( i, 'f', precision ));
                            //           qDebug() << "1 - labelItem->text() " << labelItem->text();
                            //qDebug() << "labelDiff" << labelDiff
                            //        << "  index" << i+labelDiff << "  count" << headerLabelsCount;
                            labelItem2->setText( headerLabelsCount ? headerLabels[static_cast<int>(i+labelDiff)]
                                : QString::number( i + labelDiff, 'f', precision ));
                            //qDebug() << "2 - labelItem->text() " << labelItem->text();
                            //qDebug() << "labelItem2->text() " << labelItem2->text();
                        }
                    } else {
                        int index = iLabel;
                        labelItem->setText( labels()[ index < hardLabelsCount ? index : 0 ] );
                        labelItem2->setText( labels()[ index < hardLabelsCount - 1 ? index + 1 : 0 ] );
                    }

                    QPointF firstPos( i, 0.0 );
                    firstPos = plane->translate( firstPos );

                    QPointF secondPos( i + labelDiff, 0.0 );
                    secondPos = plane->translate( secondPos );


                    if ( labelItem->intersects( *labelItem2, firstPos, secondPos ) )
                    {
                        i = minValueX;
                        labelDiff += labelDiff;
                        iLabel = 0;
                    }
                    else
                    {
                        i += labelDiff;
                    }

                    ++iLabel;
                    if ( (iLabel > hardLabelsCount - 1) && !useConfiguredStepsLabels )
                    {
                        iLabel = 0;
                    }
                }
            }

            int idxLabel = 0;
            qreal iLabelF = minValueX;
            qreal i = minValueX;
            qreal labelStep = 0.0;
            //qDebug() << "dimX.stepWidth:" << dimX.stepWidth;
            //dimX.stepWidth = 0.5;
            while( i <= maxValueX ) {
                // Line charts: we want the first tick to begin at 0.0 not at 0.5 otherwise labels and
                // values does not fit each others
                QPointF topPoint ( i + ( isBarDiagram ? 0.5 : 0.0 ), 0.0 );
                QPointF bottomPoint ( topPoint );
                topPoint = plane->translate( topPoint );
                bottomPoint = plane->translate( bottomPoint );
                topPoint.setY( rulerRef.y() + tickLength() );
                bottomPoint.setY( rulerRef.y() );

                //Dont paint more ticks than we need
                //when diagram type is Bar
                bool painttick = true;
                if (  isBarDiagram && i == maxValueX )
                    painttick = false;

                if ( painttick )
                    ptr->drawLine( topPoint, bottomPoint );

                drawnXTicks.append( static_cast<int>( topPoint.x() ) );
                if( drawLabels ) {
                    if ( isLogarithmicX )
                        labelItem->setText( QString::number( i, 'f', 0 ) );
                    /* We dont need that
                     * it causes header labels to be skipped even if there is enough
                     * space for them to displayed.
                     * Commenting for now - I need to test more in details - Let me know if I am wrong here.
                     */
                    /*
                      else if( (dimX.stepWidth != 1.0) && ! dimX.isCalculated ) {
                      labelItem->setText( QString::number( i, 'f', 0 ) );
                      }
                    */
                    else {
                        labelItem->setText( hardLabelsCount
                                            ? ( useShortLabels    ? shortLabels()[ idxLabel ] : labels()[ idxLabel ] )
                                            : ( headerLabelsCount ? headerLabels[  idxLabel ] : QString::number( iLabelF )));
                    }
                    // No need to call labelItem->setParentWidget(), since we are using
                    // the layout item temporarily only.
                    if( labelStep <= 0 ) {
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

                        bool origClipping = ptr->hasClipping();

                        QRect labelGeo = labelItem->geometry();
                        // if our item would only half fit, we disable clipping for that one
                        if( labelGeo.left() < geoRect.left() && labelGeo.right() > geoRect.left() )
                            ptr->setClipping( false );
                        if( labelGeo.left() < geoRect.right() && labelGeo.right() > geoRect.right() )
                            ptr->setClipping( false );

                        labelItem->setGeometry( labelGeo );

                        labelStep = labelDiff - dimX.stepWidth;
                        labelItem->paint( ptr );
                        labelItem2->setText( labelItem->text() );

                        // maybe enable clipping afterwards
                        ptr->setClipping( origClipping );
                    } else {
                        labelStep -= dimX.stepWidth;
                    }

                    if( hardLabelsCount ) {
                        if( idxLabel >= hardLabelsCount  -1 )
                            idxLabel = 0;
                        else
                            ++idxLabel;
                    } else if( headerLabelsCount ) {
                        if( idxLabel >= headerLabelsCount - 1 ) {
                            idxLabel = 0;
                        }else
                            ++idxLabel;
                    } else {
                        iLabelF += dimX.stepWidth;
                    }
                }
                if ( isLogarithmicX )
                    i *= 10.0;
                else
                    i += dimX.stepWidth;
            }
        } else {
            const double maxLimit = maxValueY;
            const double steg = dimY.stepWidth;
            int maxLabelsWidth = 0;
            qreal labelValue;
            if( drawLabels && position() == Right ){
                // Find the widest label, so we to know how much we need to right-shift
                // our labels, to get them drawn right aligned:
                labelValue = minValueY;
                while ( labelValue <= maxLimit ) {
                    labelItem->setText( QString::number( labelValue ) );
                    maxLabelsWidth = qMax( maxLabelsWidth, labelItem->sizeHint().width() );
                    if ( isLogarithmicY )
                        labelValue *= 10.0;
                    else
                        labelValue += steg;
                }
            }

            bool origClipping = ptr->hasClipping();
            ptr->setClipping( false );
            labelValue = minValueY;
            qreal step = steg;
            bool nextLabel = false;
            //qDebug("minValueY: %f   maxLimit: %f   steg: %f", minValueY, maxLimit, steg);

            // first calculate the steps depending on labels colision
            while ( labelValue <= maxLimit ) {
                QPointF leftPoint = plane->translate( QPointF( 0, labelValue ) );
                const qreal translatedValue = leftPoint.y();
                //qDebug() << "geoRect:" << geoRect << "   geoRect.top()" << geoRect.top()
                //<< "geoRect.bottom()" << geoRect.bottom() << "  translatedValue:" << translatedValue;
                if( translatedValue > geoRect.top() && translatedValue <= geoRect.bottom() ){
                    if ( drawLabels ) {
                        labelItem->setText( QString::number( labelValue ) );
                        labelItem2->setText( QString::number( labelValue + step ) );
                        QPointF nextPoint = plane->translate(  QPointF( 0,  labelValue + step ) );
                        if ( labelItem->intersects( *labelItem2, leftPoint, nextPoint ) )
                        {
                            step += steg;
                            nextLabel = false;
                        }
                        else
                            nextLabel = true;
                    }
                }
                if ( isLogarithmicY )
                    labelValue *= 10.0;
                else {
                    if ( nextLabel )
                        labelValue += step;
                    else
                        labelValue = minValueY;
                }
            }

            // Second - Paint the labels
            labelValue = minValueY;
            while ( labelValue <= maxLimit ) {
                labelItem->setText( QString::number( labelValue ) );
                QPointF leftPoint = plane->translate( QPointF( 0, labelValue ) );
                QPointF rightPoint ( 0.0, labelValue );
                rightPoint = plane->translate( rightPoint );
                leftPoint.setX( rulerRef.x() + tickLength() );
                rightPoint.setX( rulerRef.x() );
                ptr->drawLine( leftPoint, rightPoint );
                drawnYTicks.append( static_cast<int>( leftPoint.y() ) );
                const QSize labelSize( labelItem->sizeHint() );
                leftPoint.setX( leftPoint.x() );
                const int x =
                    static_cast<int>( leftPoint.x() + met.height() * ( position() == Left ? -0.5 : 0.5) )
                    - ( position() == Left ? labelSize.width() : (labelSize.width() - maxLabelsWidth) );
                const int y =
                    static_cast<int>( leftPoint.y() - ( met.ascent() + met.descent() ) * 0.6 );
                labelItem->setGeometry( QRect( QPoint( x, y ), labelSize ) );
                labelItem->paint( ptr );

                if ( isLogarithmicY )
                    labelValue *= 10.0;
                else
                    labelValue += step;
            }

            ptr->setClipping( origClipping );
        }
        delete labelItem;
        delete labelItem2;
    }

    // this draws the subunit rulers
    if ( drawSubUnitRulers ) {
        d->drawSubUnitRulers( ptr, plane, dim, rulerRef, isAbscissa() ? drawnXTicks : drawnYTicks );
    }

    if( ! titleText().isEmpty() ){
        d->drawTitleText( ptr, plane, areaGeoRect );
    }

    //qDebug() << "KDChart::CartesianAxis::paintCtx() done.";
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
    if ( !d->diagram() )
        return result;

    const TextAttributes labelTA = textAttributes();
    const bool drawLabels = labelTA.isVisible();

    const TextAttributes titleTA( titleTextAttributes() );
    const bool drawTitle = titleTA.isVisible() && ! titleText().isEmpty();

    AbstractCoordinatePlane* plane = d->diagram()->coordinatePlane();
    //qDebug() << this<<"::maximumSize() uses plane geometry" << plane->geometry();
    QObject* refArea = plane->parent();
    TextLayoutItem labelItem( QString::null, labelTA, refArea,
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
        qreal w = 10.0;
        qreal h = 0.0;
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
            w = titleItem.sizeHint().width() + 2.0;
        }
        // space for the ticks
        h += qAbs( tickLength() ) * 3.0;
        result = QSize ( static_cast<int>( w ), static_cast<int>( h ) );
    }
        break;
    case Left:
    case Right: {
        qreal w = 0.0;
        qreal h = 10.0;
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
            h = titleItem.sizeHint().width() + 2.0;
        }
        // space for the ticks
        w += qAbs( tickLength() ) * 3.0;

        result = QSize ( static_cast<int>( w ), static_cast<int>( h ) );
        //qDebug() << "left/right axis width:" << result << "   w:" << w;
    }
        break;
    default:
        Q_ASSERT( false ); // all positions need to be handled
        break;
    };
//qDebug() << "*******************" << result;
    //result=QSize(0,0);
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
