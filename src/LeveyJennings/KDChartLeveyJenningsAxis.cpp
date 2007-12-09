/****************************************************************************
 ** Copyright (C) 2007 Klarävdalens Datakonsult AB.  All rights reserved.
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
#include "KDChartLeveyJenningsAxis.h"
#include "KDChartLeveyJenningsAxis_p.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractGrid.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartLayoutItems.h"
#include "KDChartBarDiagram.h"
#include "KDChartPrintingParameters.h"

#include <KDABLibFakes>

#include <limits>

using namespace KDChart;

#define d (d_func())

LeveyJenningsAxis::LeveyJenningsAxis ( LeveyJenningsDiagram* diagram )
    : CartesianAxis ( new Private( diagram, this ), diagram )
{
    init();
}

static void calculateNextLabel( qreal& labelValue, qreal step, bool isLogarithmic)
{
    if ( isLogarithmic ){
        labelValue *= 10.0;
        if( labelValue == 0.0 )
            labelValue = 1.0;//std::numeric_limits< double >::epsilon();
    }else{
        //qDebug() << "new axis label:" << labelValue << "+" << step << "=" << labelValue+step;
        labelValue += step;
        if( qAbs(labelValue) < 1.0e-15 )
            labelValue = 0.0;
    }
}

LeveyJenningsAxis::~LeveyJenningsAxis ()
{
    // when we remove the first axis it will unregister itself and
    // propagate the next one to the primary, thus the while loop
    while ( d->mDiagram ) {
        LeveyJenningsDiagram *cd = qobject_cast< LeveyJenningsDiagram* >( d->mDiagram );
        cd->takeAxis( this );
    }
    Q_FOREACH( AbstractDiagram *diagram, d->secondaryDiagrams ) {
        LeveyJenningsDiagram *cd = qobject_cast< LeveyJenningsDiagram* >( diagram );
        cd->takeAxis( this );
    }
}

void LeveyJenningsAxis::init ()
{
    setType( LeveyJenningsGridAttributes::Expected );
    const QStringList labels = QStringList() << tr( "-3sd" ) << tr( "-2sd" ) << tr( "mean" )
                                             << tr( "2sd" ) << tr( "3sd" );

    setLabels( labels );
}

/**
  * @return The axis' type.
  */
LeveyJenningsGridAttributes::GridType LeveyJenningsAxis::type() const
{
    return d->type;
}

/**
  * Sets the type of the axis to \a type.
  * This method colors the label to the default color of the
  * respective type.
  * Please make sure to re-set the colors after calling this,
  * if you want them different.
  */
void LeveyJenningsAxis::setType( LeveyJenningsGridAttributes::GridType type )
{
    if( type != d->type )
    {
        TextAttributes ta = textAttributes();
        QPen pen = ta.pen();
        QColor color = type == LeveyJenningsGridAttributes::Expected ? Qt::black : Qt::blue;
        if( qobject_cast< const LeveyJenningsDiagram* >( d->diagram() ) && 
            qobject_cast< const LeveyJenningsCoordinatePlane* >( d->diagram()->coordinatePlane() ) )
        {
            color = qobject_cast< const LeveyJenningsCoordinatePlane* >( d->diagram()->coordinatePlane() )->gridAttributes().gridPen( type ).color();
        }
        pen.setColor( color );
        ta.setPen( pen );
        setTextAttributes( ta );
    }
    d->type = type;
}

bool LeveyJenningsAxis::compare( const LeveyJenningsAxis* other )const
{
    if( other == this ) return true;
    if( ! other ){
        //qDebug() << "CartesianAxis::compare() cannot compare to Null pointer";
        return false;
    }
    return  ( static_cast<const CartesianAxis*>(this)->compare( other ) );
//            ( position()            == other->position() ) &&
//            ( titleText()           == other->titleText() ) &&
//            ( titleTextAttributes() == other->titleTextAttributes() );
}

void LeveyJenningsAxis::paintCtx( PaintContext* context )
{

    Q_ASSERT_X ( d->diagram(), "LeveyJenningsAxis::paint",
                 "Function call not allowed: The axis is not assigned to any diagram." );

    LeveyJenningsCoordinatePlane* plane = dynamic_cast<LeveyJenningsCoordinatePlane*>(context->coordinatePlane());
    Q_ASSERT_X ( plane, "LeveyJenningsAxis::paint",
                 "Bad function call: PaintContext::coodinatePlane() NOT a levey jennings plane." );

    // note: Not having any data model assigned is no bug
    //       but we can not draw an axis then either.
    if( ! d->diagram()->model() )
        return;



    const LeveyJenningsDiagram* const diag = dynamic_cast< const LeveyJenningsDiagram* >( d->diagram() );

    Q_ASSERT_X( isOrdinate(), "LeveyJenningsAxis::paintCtx", "Levei jennings axes can only be drawn at left or right side" );

    
    const qreal meanValue =         type() == LeveyJenningsGridAttributes::Expected ? diag->expectedMeanValue() 
                                                                                    : diag->calculatedMeanValue();
    const qreal standardDeviation = type() == LeveyJenningsGridAttributes::Expected ? diag->expectedStandardDeviation() 
                                                                                    : diag->calculatedStandardDeviation();
    const TextAttributes labelTA = textAttributes();
    const bool drawLabels = labelTA.isVisible();

    // nothing to draw, since we've no ticks
    if( !drawLabels )
        return;
    
    const QObject* referenceArea = plane->parent();

    const QVector< qreal > values = QVector< qreal >() << ( meanValue - 3 * standardDeviation )
                                                       << ( meanValue - 2 * standardDeviation )
                                                       << ( meanValue )
                                                       << ( meanValue + 2 * standardDeviation )
                                                       << ( meanValue + 3 * standardDeviation );

    Q_ASSERT_X( values.count() <= labels().count(), "LeveyJenningsAxis::paintCtx", "Need to have at least 5 labels" );

    TextLayoutItem labelItem( tr( "mean" ), 
                              labelTA,
                              referenceArea,
                              KDChartEnums::MeasureOrientationMinimum,
                              Qt::AlignLeft );

    QPainter* const painter = context->painter();
    const PainterSaver ps( painter );
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->setClipping( false );
    
    painter->setPen ( PrintingParameters::scalePen( labelTA.pen() ) ); // perhaps we want to add a setter method later?

    for( int i = 0; i < values.count(); ++i )
    {
        const QPointF labelPos = plane->translate( QPointF( 0.0, values.at( i ) ) );
        const QString label = customizedLabel( labels().at( i ) );
        labelItem.setText( label );
        const QSize size = labelItem.sizeHint();
        const float xPos = position() == Left ? geometry().right() - size.width() : geometry().left();
        labelItem.setGeometry( QRectF( QPointF( xPos, labelPos.y() - size.height() / 2.0 ), size ).toRect() );
        labelItem.paint( painter );
    }    

    return;


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
    Q_ASSERT_X ( dimensions.count() == 2, "LeveyJenningsAxis::paint",
                 "Error: plane->gridDimensionsList() did not return exactly two dimensions." );
    DataDimension dimX =
            AbstractGrid::adjustedLowerUpperRange( dimensions.first(), true, true );
    const DataDimension dimY =
            AbstractGrid::adjustedLowerUpperRange( dimensions.last(), true, true );
    const DataDimension& dim = (isAbscissa() ? dimX : dimY);

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

    const bool drawUnitRulers = screenRange / ( numberOfUnitRulers / dimX.stepWidth ) > MinimumPixelsBetweenRulers;
    const bool drawSubUnitRulers =
        (numberOfSubUnitRulers != 0.0) &&
        (screenRange / numberOfSubUnitRulers > MinimumPixelsBetweenRulers);

    // - find the reference point at which to start drawing and the increment (line distance);
    QPointF rulerRef;
    const QRect areaGeoRect( areaGeometry() );
    const QRect geoRect( geometry() );
    QRectF rulerRect;
    double rulerWidth;
    double rulerHeight;

    QPainter* const ptr = context->painter();

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

    // solving issue #4075 in a quick way:
    ptr->setPen ( PrintingParameters::scalePen( labelTA.pen() ) ); // perhaps we want to add a setter method later?

    //ptr->setPen ( Qt::black );

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

    const bool isBarDiagram = false;

    // this draws the unit rulers
    if ( drawUnitRulers ) {
        const QStringList labelsList(      labels() );
        const QStringList shortLabelsList( shortLabels() );
        const int hardLabelsCount  = labelsList.count();
        const int shortLabelsCount = shortLabelsList.count();
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
                double value = dimX.start;// headerLabels.isEmpty() ? 0.0 : headerLabels.first().toDouble();
                configuredStepsLabels << QString::number( value );
                
                for( int i = 0; i < numberOfUnitRulers; i++ )
                {
                    //qDebug() << value;
                    value += dimX.stepWidth;
                    configuredStepsLabels.append( d->diagram()->unitPrefix( i, Qt::Horizontal, true ) +
                                                  QString::number( value ) +
                                                  d->diagram()->unitSuffix( i, Qt::Horizontal, true ) );
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
            : QFontMetricsF( QApplication::font(), GlobalMeasureScaling::paintDevice() ) );
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
                        labelItem->setText(  customizedLabel(QString::number( i, 'f', 0 )) );
                        labelItem2->setText( customizedLabel(QString::number( i + dimX.stepWidth, 'f', 0 )) );
                    } else {

                        int index = iLabel;
                        labelItem->setText(  customizedLabel(labelsList[ index < hardLabelsCount ? index : 0 ]) );
                        labelItem2->setText( customizedLabel(labelsList[ index < hardLabelsCount - 1 ? index + 1 : 0]) );
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
            //      qDebug() << "initial labelDiff " << labelDiff;
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
                            labelItem->setText( customizedLabel(headerLabels[ iLabel   ]) );
                            labelItem2->setText(customizedLabel(headerLabels[ iLabel+1 ]) );
                        }else{
                            //qDebug() << "i + labelDiff " << i + labelDiff;
                            labelItem->setText( customizedLabel(headerLabelsCount > i && i >= 0 ? 
                                    headerLabels[static_cast<int>(i)] :
                                    QString::number( i, 'f', precision )) );
                            //           qDebug() << "1 - labelItem->text() " << labelItem->text();
                            //qDebug() << "labelDiff" << labelDiff
                            //        << "  index" << i+labelDiff << "  count" << headerLabelsCount;
                            labelItem2->setText( customizedLabel(headerLabelsCount > i + labelDiff && i + labelDiff >= 0 ? 
                                    headerLabels[static_cast<int>(i+labelDiff)] :
                                    QString::number( i + labelDiff, 'f', precision )) );
                            //qDebug() << "2 - labelItem->text() " << labelItem->text();
                            //qDebug() << "labelItem2->text() " << labelItem2->text();
                        }
                    } else {
                        const int idx = (iLabel < hardLabelsCount    ) ? iLabel     : 0;
                        const int idx2= (iLabel < hardLabelsCount - 1) ? iLabel + 1 : 0;
                        const int shortIdx =  (iLabel < shortLabelsCount    ) ? iLabel     : 0;
                        const int shortIdx2 = (iLabel < shortLabelsCount - 1) ? iLabel + 1 : 0;
                        labelItem->setText(  customizedLabel(
                                useShortLabels ? shortLabelsList[ shortIdx ] : labelsList[ idx ] ) );
                        labelItem2->setText( customizedLabel(
                                useShortLabels ? shortLabelsList[ shortIdx2 ] : labelsList[ idx2 ] ) );
                    }

                    QPointF firstPos( i, 0.0 );
                    firstPos = plane->translate( firstPos );

                    QPointF secondPos( i + labelDiff, 0.0 );
                    secondPos = plane->translate( secondPos );


                    if ( labelItem->intersects( *labelItem2, firstPos, secondPos ) )
                    {
                        i = minValueX;

                        // fix for issue #4179:
                        labelDiff *= 10.0;
                        // old code:
                        // labelDiff += labelDiff;

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
            //qDebug() << iLabelF;
            qreal i = minValueX;
            qreal labelStep = 0.0;
            //    qDebug() << "dimX.stepWidth:" << dimX.stepWidth  << "labelDiff:" << labelDiff;
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

                const qreal translatedValue = topPoint.x();
                const bool bIsVisibleLabel =
                        ( translatedValue >= geoRect.left() && translatedValue <= geoRect.right() && !isLogarithmicX || i != 0.0 );

                // fix for issue #4179:
                bool painttick = bIsVisibleLabel && labelStep <= 0;;
                // old code:
                // bool painttick = true;

                //Dont paint more ticks than we need
                //when diagram type is Bar
                if (  isBarDiagram && i == maxValueX )
                    painttick = false;

                if ( bIsVisibleLabel && painttick )
                    ptr->drawLine( topPoint, bottomPoint );

                drawnXTicks.append( static_cast<int>( topPoint.x() ) );
                if( drawLabels ) {
                    if( bIsVisibleLabel ){
                        if ( isLogarithmicX )
                            labelItem->setText( customizedLabel(QString::number( i, 'f', 0 )) );
                        /* We dont need that
                        * it causes header labels to be skipped even if there is enough
                        * space for them to displayed.
                        * Commenting for now - I need to test more in details - Let me know if I am wrong here.
                        */
                        /*
                        else if( (dimX.stepWidth != 1.0) && ! dimX.isCalculated ) {
                        labelItem->setText( customizedLabel(QString::number( i, 'f', 0 )) );
                        }
                        */
                        else {
                            const int idx = idxLabel + static_cast<int>(minValueX);
                            labelItem->setText(
                                    customizedLabel(
                                          hardLabelsCount
                                    ? ( useShortLabels    ? shortLabelsList[ idx ] : labelsList[ idx ] )
                                : ( headerLabelsCount ? headerLabels[ idx ] : QString::number( iLabelF ))));
                            //qDebug() << "x - labelItem->text() " << labelItem->text() << headerLabelsCount;
                        }
                        // No need to call labelItem->setParentWidget(), since we are using
                        // the layout item temporarily only.
                        if( labelStep <= 0 ) {
                            const PainterSaver p( ptr );
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

                            QRect labelGeo = labelItem->geometry();
                            // if our item would only half fit, we disable clipping for that one
                            if( labelGeo.left() < geoRect.left() && labelGeo.right() > geoRect.left() )
                                ptr->setClipping( false );
                            else if( labelGeo.left() < geoRect.right() && labelGeo.right() > geoRect.right() )
                                ptr->setClipping( false );

                            labelItem->setGeometry( labelGeo );

                           
                            if( !isLogarithmicX )
                                labelStep = labelDiff - dimX.stepWidth;

                            labelItem->paint( ptr );

                            // do not call customizedLabel() again:
                            labelItem2->setText( labelItem->text() );

                        } else {
                            labelStep -= dimX.stepWidth;
                        }
                    }

                    if( hardLabelsCount ) {
                        if( useShortLabels && idxLabel >= shortLabelsCount - 1 )
                            idxLabel = 0;
                        else if( !useShortLabels && idxLabel >= hardLabelsCount - 1 )
                            idxLabel = 0;
                        else{
                            idxLabel += static_cast<int>(dimX.stepWidth);
                            //qDebug() << "dimX.stepWidth:" << dimX.stepWidth << "  idxLabel:" << idxLabel;
                        }
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
                {
                    i *= 10.0;
                    if( i == 0.0 )
                        i = 1.0;//std::numeric_limits< double >::epsilon();
                }
                else
                {
                    i += dimX.stepWidth;
                }
            }
        } else {
            const PainterSaver p( ptr );
            const double maxLimit = maxValueY;
            const double steg = dimY.stepWidth;
            int maxLabelsWidth = 0;
            qreal labelValue;
            if( drawLabels && position() == Right ){
                // Find the widest label, so we to know how much we need to right-shift
                // our labels, to get them drawn right aligned:
                labelValue = minValueY;
                while ( labelValue <= maxLimit ) {
                    const QString labelText = diagram()->unitPrefix( static_cast< int >( labelValue ), Qt::Vertical, true ) + 
                                              QString::number( labelValue ) +
                                              diagram()->unitSuffix( static_cast< int >( labelValue ), Qt::Vertical, true );
                    labelItem->setText( customizedLabel( labelText ) );
                    maxLabelsWidth = qMax( maxLabelsWidth, labelItem->sizeHint().width() );

                    calculateNextLabel( labelValue, steg, isLogarithmicY );
                }
            }

            ptr->setClipping( false );
            labelValue = minValueY;
            qreal step = steg;
            bool nextLabel = false;
            //qDebug("minValueY: %f   maxLimit: %f   steg: %f", minValueY, maxLimit, steg);

            if( drawLabels )
            {
                // first calculate the steps depending on labels colision
                while( labelValue <= maxLimit ) {
                    QPointF leftPoint = plane->translate( QPointF( 0, labelValue ) );
                    const qreal translatedValue = leftPoint.y();
                    //qDebug() << "geoRect:" << geoRect << "   geoRect.top()" << geoRect.top()
                    //<< "geoRect.bottom()" << geoRect.bottom() << "  translatedValue:" << translatedValue;
                    if( translatedValue > geoRect.top() && translatedValue <= geoRect.bottom() ){
                        const QString labelText = diagram()->unitPrefix( static_cast< int >( labelValue ), Qt::Vertical, true ) +
                                                  QString::number( labelValue ) +
                                                  diagram()->unitSuffix( static_cast< int >( labelValue ), Qt::Vertical, true );
                        const QString label2Text = diagram()->unitPrefix( static_cast< int >( labelValue + step ), Qt::Vertical, true ) +
                                                   QString::number( labelValue + step ) +
                                                   diagram()->unitSuffix( static_cast< int >( labelValue + step ), Qt::Vertical, true );
                        labelItem->setText(  customizedLabel( labelText ) );
                        labelItem2->setText( customizedLabel( QString::number( labelValue + step ) ) );
                        QPointF nextPoint = plane->translate(  QPointF( 0,  labelValue + step ) );
                        if ( labelItem->intersects( *labelItem2, leftPoint, nextPoint ) )
                        {
                            step += steg;
                            nextLabel = false;
                        }else{
                            nextLabel = true;
                        }
                    }else{
                        nextLabel = true;
                    }

                    if ( nextLabel || isLogarithmicY )
                        calculateNextLabel( labelValue, step, isLogarithmicY );
                    else
                        labelValue = minValueY;
                }

                // Second - Paint the labels
                labelValue = minValueY;
                //qDebug() << "axis labels starting at" << labelValue << "step width" << step;
                while( labelValue <= maxLimit ) {
                    //qDebug() << "value now" << labelValue;
                    const QString labelText = diagram()->unitPrefix( static_cast< int >( labelValue ), Qt::Vertical, true ) + 
                                              QString::number( labelValue ) + 
                                              diagram()->unitSuffix( static_cast< int >( labelValue ), Qt::Vertical, true );
                    labelItem->setText( customizedLabel( labelText ) );
                    QPointF leftPoint = plane->translate( QPointF( 0, labelValue ) );
                    QPointF rightPoint ( 0.0, labelValue );
                    rightPoint = plane->translate( rightPoint );
                    leftPoint.setX( rulerRef.x() + tickLength() );
                    rightPoint.setX( rulerRef.x() );

                    const qreal translatedValue = rightPoint.y();
                    const bool bIsVisibleLabel =
                            ( translatedValue >= geoRect.top() && translatedValue <= geoRect.bottom() && !isLogarithmicY || labelValue != 0.0 );

                    if( bIsVisibleLabel ){
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
                    }

                    calculateNextLabel( labelValue, step, isLogarithmicY );
                }
            }
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
