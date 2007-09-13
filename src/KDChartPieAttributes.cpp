/****************************************************************************
 ** Copyright (C) 2007 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartPieAttributes.h"
#include "KDChartPieAttributes_p.h"

#include <QDebug>

#include <KDABLibFakes>

#define d d_func()


using namespace KDChart;


PieAttributes::Private::Private()
    : explodeFactor( 0.0 )
{
}


PieAttributes::PieAttributes()
    : _d( new Private() )
{
}

PieAttributes::PieAttributes( const PieAttributes& r )
    : _d( new Private( *r.d ) )
{
}

PieAttributes& PieAttributes::operator= ( const PieAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

PieAttributes::~PieAttributes()
{
    delete _d; _d = 0;
}


bool PieAttributes::operator==( const PieAttributes& r ) const
{
    if( explodeFactor() == r.explodeFactor() )
        return true;
    else
        return false;
}


void PieAttributes::init( )
{

}

void PieAttributes::setExplode( bool enabled )
{
    d->explodeFactor = (enabled ? 0.1 : 0.0);
}

bool PieAttributes::explode() const
{
    return (d->explodeFactor != 0.0);
}

void PieAttributes::setExplodeFactor( qreal factor )
{
    d->explodeFactor = factor;
}

qreal PieAttributes::explodeFactor() const
{
    return d->explodeFactor;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::PieAttributes& a)
{
    dbg << "KDChart::PieAttributes(";
    dbg << "explodeFactor="<< a.explodeFactor() << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */

