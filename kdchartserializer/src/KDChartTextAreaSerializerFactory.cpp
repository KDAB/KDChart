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

#include <KDChartTextAreaSerializerFactory.h>
#include <KDChartTextAreaSerializerFactory_p.h>

#include <KDChartTextAreaSerializer.h>

#include <KDChartHeaderFooter>

#define d d_func()

using namespace KDChart;

TextAreaSerializerFactory::Private::Private( TextAreaSerializerFactory* qq )
    : q( qq ),
      m_instance( 0 )
{
}

TextAreaSerializerFactory::Private::~Private()
{
    if( m_instance != 0 )
        delete m_instance;
}

TextAreaSerializerFactory::TextAreaSerializerFactory( QObject* parent )
    : AbstractSerializerFactory( parent ),
      _d( new Private( this ) )
{
}

TextAreaSerializerFactory::~TextAreaSerializerFactory()
{
    delete _d; _d = 0;
}

void TextAreaSerializerFactory::init()
{
}

AbstractSerializer* TextAreaSerializerFactory::instance( const QString& className ) const
{
    Q_UNUSED( className );

    if( d->m_instance == 0 )
        d->m_instance = new TextAreaSerializer;

    return d->m_instance;
}

QObject* TextAreaSerializerFactory::createNewObject( const QString& className ) const
{
    if( className == HeaderFooter::staticMetaObject.className() )
        return new HeaderFooter;
    return 0;
}
