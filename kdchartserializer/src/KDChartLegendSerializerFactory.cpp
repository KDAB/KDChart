/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <KDChartLegendSerializerFactory.h>
#include <KDChartLegendSerializerFactory_p.h>

#include <KDChartLegendsSerializer.h>

#include <KDChartLegend>

#define d d_func()

using namespace KDChart;

LegendSerializerFactory::Private::Private( LegendSerializerFactory* qq )
    : q( qq ),
      m_instance( 0 )
{
}

LegendSerializerFactory::Private::~Private()
{
    if( m_instance != 0 )
        delete m_instance;
}

LegendSerializerFactory::LegendSerializerFactory( QObject* parent )
    : AbstractSerializerFactory( parent ),
      _d( new Private( this ) )
{
}

LegendSerializerFactory::~LegendSerializerFactory()
{
    delete _d; _d = 0;
}

void LegendSerializerFactory::init()
{
}

AbstractSerializer* LegendSerializerFactory::instance( const QString& className ) const
{
    Q_UNUSED( className );

    if( d->m_instance == 0 )
        d->m_instance = new LegendsSerializer;

    return d->m_instance;
}

QObject* LegendSerializerFactory::createNewObject( const QString& className ) const
{
    if( className == Legend::staticMetaObject.className() )
        return new Legend;
    return 0;
}