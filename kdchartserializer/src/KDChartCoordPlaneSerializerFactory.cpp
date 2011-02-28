/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include <KDChartCoordPlaneSerializerFactory.h>
#include <KDChartCoordPlaneSerializerFactory_p.h>

#include <KDChartCoordPlanesSerializer.h>

#include <KDChartCartesianCoordinatePlane>
#include <KDChartPolarCoordinatePlane>

#define d d_func()

using namespace KDChart;

CoordPlaneSerializerFactory::Private::Private( CoordPlaneSerializerFactory* qq )
    : q( qq ),
      m_instance( 0 )
{
}

CoordPlaneSerializerFactory::Private::~Private()
{
    if( m_instance != 0 )
        delete m_instance;
}

CoordPlaneSerializerFactory::CoordPlaneSerializerFactory( QObject* parent )
    : AbstractSerializerFactory( parent ),
      _d( new Private( this ) )
{
}

CoordPlaneSerializerFactory::~CoordPlaneSerializerFactory()
{
    delete _d; _d = 0;
}

void CoordPlaneSerializerFactory::init()
{
}

AbstractSerializer* CoordPlaneSerializerFactory::instance( const QString& className ) const
{
    Q_UNUSED( className );

    if( d->m_instance == 0 )
        d->m_instance = new CoordPlanesSerializer;

    return d->m_instance;
}

QObject* CoordPlaneSerializerFactory::createNewObject( const QString& className ) const
{
    if( className == CartesianCoordinatePlane::staticMetaObject.className() )
        return new CartesianCoordinatePlane;
    else if( className == PolarCoordinatePlane::staticMetaObject.className() )
        return new PolarCoordinatePlane;
    return 0;
}
