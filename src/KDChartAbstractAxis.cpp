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

using namespace KDChart;


AbstractAxis::Private::Private( AbstractDiagram* parent_ )
{
    // PENDING(miroslav) Code from KDChartAxis::Private::Private goes here
    parent = parent_;
    observer = new DiagramObserver( parent_ );
}


AbstractAxis::Private::~Private()
{
    // PENDING(miroslav) Code from KDChartAxis::Private::~Private goes here
    delete observer;
    observer = 0;
}


AbstractAxis::AbstractAxis ( AbstractDiagram* parent )
    : AbstractArea( new Private( parent ), parent )
{
    init();
}

AbstractAxis::~AbstractAxis()
{
    emit destroyedAxis( this );
}


void AbstractAxis::init()
{
    connect( d_func()->observer, SIGNAL( diagramDataChanged( AbstractDiagram *) ),
             this, SLOT( update() ) );
}

/**
  \brief Use this to specify your own set of strings, to be used as axis labels.

  Labels specified via setLabels take precedence:
  If a non-empty list is passed, KD Chart will use these strings as axis labels,
  instead of calculating them.

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
