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

#include "KDChartAbstractThreeDAttributes.h"
#include "KDChartAbstractThreeDAttributes_p.h"
#include <qglobal.h>

#define d d_func()

using namespace KDChart;


AbstractThreeDAttributes::Private::Private()
    : enabled( false ),
      height( 20 ),
      depth( 20 )
{
}


AbstractThreeDAttributes::AbstractThreeDAttributes()
    : _d( new Private() )
{
}

AbstractThreeDAttributes::AbstractThreeDAttributes( const AbstractThreeDAttributes& r )
    : _d( new Private( *r.d ) )
{
}

AbstractThreeDAttributes& AbstractThreeDAttributes::operator= ( const AbstractThreeDAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

AbstractThreeDAttributes::~AbstractThreeDAttributes()
{
    delete _d; _d = 0;
}


bool AbstractThreeDAttributes::operator==( const AbstractThreeDAttributes& r ) const
{
    if( isEnabled() == r.isEnabled() &&
        height() == r.height() &&
        depth() == r.depth() )
        return true;
    else
        return false;
}


void AbstractThreeDAttributes::init( )
{

}

void AbstractThreeDAttributes::setEnabled( bool enabled )
{
    d->enabled = enabled;
}

bool AbstractThreeDAttributes::isEnabled() const
{
    return d->enabled;
}

void AbstractThreeDAttributes::setHeight( int pixels )
{
    d->height = pixels;
}

int AbstractThreeDAttributes::height() const
{
    return d->height;
}

void AbstractThreeDAttributes::setDepth( int depth )
{
    d->depth = depth;
}


int AbstractThreeDAttributes::depth() const
{
    return d->depth;
}


QDomDocumentFragment AbstractThreeDAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: AbstractThreeDAttributes::toXML()" );
    return QDomDocumentFragment();
}

