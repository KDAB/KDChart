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

#include "KDChartThreeDLineAttributes.h"
#include "KDChartThreeDLineAttributes_p.h"
#include <qglobal.h>

#define d d_func()

using namespace KDChart;

ThreeDLineAttributes::Private::Private()
    : lineXRotation( 15 ),
      lineYRotation( 15 )
{
}


ThreeDLineAttributes::ThreeDLineAttributes()
    : ThreeDAttributes( new Private() )
{

}

ThreeDLineAttributes::ThreeDLineAttributes( const ThreeDLineAttributes& r )
    : ThreeDAttributes( new Private( *r.d) )
{
}

ThreeDLineAttributes& ThreeDLineAttributes::operator= ( const ThreeDLineAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

ThreeDLineAttributes::~ThreeDLineAttributes()
{
}

void ThreeDLineAttributes::init()
{
}


bool ThreeDLineAttributes::operator==( const ThreeDLineAttributes& r ) const
{
    return ( lineXRotation() == r.lineXRotation() &&
             lineYRotation() == r.lineYRotation() &&
             ThreeDAttributes::operator==(r));
}



void ThreeDLineAttributes::setLineXRotation( uint degrees )
{
    d->lineXRotation = degrees;
}

uint ThreeDLineAttributes::lineXRotation() const
{
    return d->lineXRotation;
}

void ThreeDLineAttributes::setLineYRotation( uint degrees )
{
    d->lineYRotation = degrees;
}

uint ThreeDLineAttributes::lineYRotation() const
{
    return d->lineYRotation;
}


QDomDocumentFragment ThreeDLineAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: ThreeDLineAttribute::toXML()" );
    return QDomDocumentFragment();
}


