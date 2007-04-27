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

#include <QDebug>

#include "KDChartAbstractAxis.h"
#include "KDChartAbstractAxis_p.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartEnums.h"
#include "KDChartMeasure.h"

#include <KDABLibFakes>

using namespace KDChart;

#define d d_func()

AbstractAxis::Private::Private( AbstractDiagram* diagram, AbstractAxis* axis )
    : observer( 0 )
    , mDiagram( diagram )
    , mAxis(    axis )
{
    // Note: We do NOT call setDiagram( diagram, axis );
    //       but it is called in AbstractAxis::delayedInit() instead!
}

AbstractAxis::Private::~Private()
{
    delete observer;
    observer = 0;
}

bool AbstractAxis::Private::setDiagram(
    AbstractDiagram* diagram_,
    bool delayedInit )
{
    AbstractDiagram* diagram = delayedInit ? mDiagram : diagram_;
    if( delayedInit ){
        mDiagram = 0;
    }

    // do not set a diagram again that was already set
    if (  diagram &&
        ((diagram == mDiagram) || secondaryDiagrams.contains( diagram )) )
        return false;

    bool bNewDiagramStored = false;
    if ( ! mDiagram ) {
        mDiagram = diagram;
        delete observer;
        if ( mDiagram ) {
//qDebug() << "axis" << (axis != 0);
            observer = new DiagramObserver( mDiagram, mAxis );
            bNewDiagramStored = true;
        }else{
            observer = 0;
        }
    } else {
        if ( diagram )
            secondaryDiagrams.enqueue( diagram );
    }
    return bNewDiagramStored;
}

void AbstractAxis::Private::unsetDiagram( AbstractDiagram* diagram )
{
    if ( diagram == mDiagram ) {
        mDiagram = 0;
        delete observer;
        observer = 0;
    } else {
        secondaryDiagrams.removeAll( diagram );
    }
    if( !secondaryDiagrams.isEmpty() ) {
        AbstractDiagram *nextDiagram = secondaryDiagrams.dequeue();
        setDiagram( nextDiagram );
    }
}

bool AbstractAxis::Private::hasDiagram( AbstractDiagram* diagram ) const
{
    return diagram == mDiagram || secondaryDiagrams.contains( diagram );
}

AbstractAxis::AbstractAxis ( AbstractDiagram* diagram )
    : AbstractArea( new Private( diagram, this ) )
{
    init();
    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

AbstractAxis::~AbstractAxis()
{
    d->mDiagram = 0;
    d->secondaryDiagrams.clear();
}


void AbstractAxis::init()
{
    Measure m(
        12.5,
        KDChartEnums::MeasureCalculationModeAuto,
        KDChartEnums::MeasureOrientationAuto );
    d->textAttributes.setFontSize( m  );
    m.setValue( 5 );
    m.setCalculationMode( KDChartEnums::MeasureCalculationModeAbsolute );
    d->textAttributes.setMinimalFontSize( m  );
}

void AbstractAxis::delayedInit()
{
    // We call setDiagram() here, because the c'tor of Private
    // only has stored the pointers, but it did not call setDiagram().
    if( d )
        d->setDiagram( 0, true /* delayedInit */ );
}

bool AbstractAxis::compare( const AbstractAxis* other )const
{
    if( other == this ) return true;
    if( ! other ){
        //qDebug() << "CartesianAxis::compare() cannot compare to Null pointer";
        return false;
    }
    /*
    qDebug() << (textAttributes() == other->textAttributes());
    qDebug() << (labels()         == other->labels());
    qDebug() << (shortLabels()    == other->shortLabels());
    */
    return  ( static_cast<const AbstractAreaBase*>(this)->compare( other ) ) &&
            (textAttributes() == other->textAttributes()) &&
            (labels()         == other->labels()) &&
            (shortLabels()    == other->shortLabels());
}


const QString AbstractAxis::customizedLabel( const QString& label )const
{
    return label;
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
void AbstractAxis::createObserver( AbstractDiagram* diagram )
{
    if( d->setDiagram( diagram ) )
        connectSignals();
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
void AbstractAxis::deleteObserver( AbstractDiagram* diagram )
{
    d->unsetDiagram( diagram );
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
    if( d->observer ){
        connect( d->observer, SIGNAL( diagramDataChanged( AbstractDiagram *) ),
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
    d->textAttributes = a;
}

/**
  \brief Returns the text attributes to be used for axis labels.

  \sa setTextAttributes
*/
TextAttributes AbstractAxis::textAttributes() const
{
    return d->textAttributes;
}

/**
  \brief Use this to specify your own set of strings, to be used as axis labels.

  Labels specified via setLabels take precedence:
  If a non-empty list is passed, KD Chart will use these strings as axis labels,
  instead of calculating them.

  If you a smaller number of strings than the number of labels drawn at this
  axis, KD Chart will iterate over the list, repeating the strings, until all
  labels are drawn.
  As an example you could specify the seven days of the week as abscissa labels,
  which would be repeatedly used then.

  By passing an empty QStringList you can reset the default behaviour.

  \sa labels, setShortLabels
*/
void AbstractAxis::setLabels( const QStringList& list )
{
    d->hardLabels = list;
}

/**
  Returns a list of strings, that are used as axis labels, as set via setLabels.

  \sa setLabels
*/
QStringList AbstractAxis::labels() const
{
    return d->hardLabels;
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
    d->hardShortLabels = list;
}

/**
  Returns a list of strings, that are used as axis labels, as set via setShortLabels.

  \note Setting done via setShortLabels will be ignored, if you did not pass
  a non-empty string list via setLabels too!

  \sa setShortLabels
*/
QStringList AbstractAxis::shortLabels() const
{
    return d->hardShortLabels;
}

/**
    \brief Convenience function, returns the coordinate plane, in which this axis is used.

    If the axis is not used in a coordinate plane, the return value is Zero.
 */
const AbstractCoordinatePlane* AbstractAxis::coordinatePlane() const
{
    if( d->diagram() )
        return d->diagram()->coordinatePlane();
    return 0;
}

const AbstractDiagram * KDChart::AbstractAxis::diagram() const
{
    return d->diagram();
}

bool KDChart::AbstractAxis::observedBy( AbstractDiagram * diagram ) const
{
    return d->hasDiagram( diagram );
}

void KDChart::AbstractAxis::update()
{
    if( d->diagram() )
        d->diagram()->update();
}
