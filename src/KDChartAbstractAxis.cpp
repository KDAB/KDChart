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

#include "KDChartAbstractAxis.h"
#include "KDChartAbstractAxis_p.h"
#include <QDebug>
#include <KDChartAbstractDiagram.h>
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartEnums.h"
#include "KDChartMeasure.h"

using namespace KDChart;


AbstractAxis::Private::Private( AbstractDiagram* diagram_ )
    : diagram( diagram_ )
{
    // PENDING(miroslav) Code from KDChartAxis::Private::Private goes here
    if( diagram )
        observer = new DiagramObserver( *diagram );
    else
        observer = 0;
}


AbstractAxis::Private::~Private()
{
    // PENDING(miroslav) Code from KDChartAxis::Private::~Private goes here
    if( observer ){
      delete observer;
      observer = 0;
    }
}


/**
  * C'tor of the base class for axes.
  *
  * \note If using a zero parent for the constructor, you NEED to call
  * your diagram's addAxis function to add your axis to the diagram.
  * Otherwise, there is no need to call addAxis, since the constructor
  * does that automatically for you, if you pass a parent as parameter.
  *
  * \sa AbstractCartesianDiagram::addAxis
  */
AbstractAxis::AbstractAxis ( AbstractDiagram* diagram )
    : AbstractArea( new Private( diagram ), diagram )
{
    init();
    if( diagram )
        createObserver( diagram );
}

AbstractAxis::~AbstractAxis()
{
    emit destroyedAxis( this );
}


void AbstractAxis::init()
{
    Measure m(
        12.5,
        KDChartEnums::MeasureCalculationModeAuto,
        KDChartEnums::MeasureOrientationAuto );
    d_func()->textAttributes.setFontSize( m  );
    m.setValue( 5 );
    m.setCalculationMode( KDChartEnums::MeasureCalculationModeAbsolute );
    d_func()->textAttributes.setMinimalFontSize( m  );
}

/**
  * \internal
  *
  * Method invoked by AbstractCartesianDiagram::addAxis().
  *
  * You should not call this function, unless you know exactly,
  * what you are doing.
  *
  * \sa connectSignals(), AbstractCartesianDiagram::addAxis()
  */
void AbstractAxis::createObserver( KDChart::AbstractDiagram* diagram )
{
    if( d_func()->diagram == diagram ) return;

    d_func()->diagram = diagram;
    if ( d_func()->observer )
    {
        delete d_func()->observer; d_func()->observer = NULL;
    }
    if ( diagram )
    {
        d_func()->observer = new DiagramObserver( *diagram, this );
        connectSignals();
    }
}

/**
  * \internal
  *
  * Method invoked by AbstractCartesianDiagram::takeAxis().
  *
  * You should not call this function, unless you know exactly,
  * what you are doing.
  *
  * \sa AbstractCartesianDiagram::takeAxis()
  */
void AbstractAxis::deleteObserver()
{
    d_func()->diagram = 0;
    if ( d_func()->observer )
    {
        delete d_func()->observer; d_func()->observer = NULL;
    }
}

/**
  * Wireing the signal/slot connections.
  *
  * This method gets called automatically, each time, when you assign
  * the axis to a diagram, either by passing a diagram* to the c'tor,
  * or by calling the diagram's setAxis method, resp.
  *
  * If overwriting this method in derived classes, make sure to call
  * this base method AbstractAxis::connectSignals(), so your axis
  * gets connected to the diagram's built-in signals.
  *
  * \sa AbstractCartesianDiagram::addAxis()
  */
void AbstractAxis::connectSignals()
{
    if( d_func()->observer ){
        connect( d_func()->observer, SIGNAL( diagramDataChanged( AbstractDiagram *) ),
                this, SLOT( update() ) );
    }
}


/**
  \brief Use this to specify the text attributes to be used for axis labels.

  By default, the reference area will be set at painting time.
  It will be the then-valid coordinate plane's parent widget,
  so normally, it will be the KDChart::Chart.
  Thus the labels of all of your axes in all of your diagrams
  within that Chart will be drawn in same font size, by default.

  \sa textAttributes, setLabels
*/
void AbstractAxis::setTextAttributes( const TextAttributes &a )
{
    d_func()->textAttributes = a;
}

/**
  \brief Returns the text attributes to be used for axis labels.

  \sa setTextAttributes
*/
TextAttributes AbstractAxis::textAttributes() const
{
    return d_func()->textAttributes;
}

/**
  \brief Use this to specify your own set of strings, to be used as axis labels.

  Labels specified via setLabels take precedence:
  If a non-empty list is passed, KD Chart will use these strings as axis labels,
  instead of calculating them.

  If you a smaller number of strings than the number od labels drawn at this
  axis, KD Chart will iterate over the list, repeating the strings, until all
  labels are drawn.
  As an example you could specify the seven days of the week as abscissa labels,
  which would be repeatedly used then.

  By passing an empty QStringList you can reset the default behaviour.

  \sa labels, setShortLabels
*/
void AbstractAxis::setLabels( const QStringList& list )
{
    d_func()->hardLabels = list;
}

/**
  Returns a list of strings, that are used as axis labels, as set via setLabels.

  \sa setLabels
*/
QStringList AbstractAxis::labels() const
{
    return d_func()->hardLabels;
}

/**
  \brief Use this to specify your own set of strings, to be used as axis labels,
  in case the normal labels are too long.

  \note Setting done via setShortLabels will be ignored, if you did not pass
  a non-empty string list via setLabels too!

  By passing an empty QStringList you can reset the default behaviour.

  \sa shortLabels, setLabels
*/
void AbstractAxis::setShortLabels( const QStringList& list )
{
    d_func()->hardShortLabels = list;
}

/**
  Returns a list of strings, that are used as axis labels, as set via setShortLabels.

  \note Setting done via setShortLabels will be ignored, if you did not pass
  a non-empty string list via setLabels too!

  \sa setShortLabels
*/
QStringList AbstractAxis::shortLabels() const
{
    return d_func()->hardShortLabels;
}

/**
    \brief Convenience function, returns the coordinate plane, in which this axis is used.

    If the axis is not used in a coordinate plane, the return value is zero.
 */
const AbstractCoordinatePlane* AbstractAxis::coordinatePlane() const
{
    if( d_func()->diagram )
        return d_func()->diagram->coordinatePlane();
    return 0;
}
