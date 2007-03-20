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

#include "KDChartSerializeCollector.h"
#include "KDChartIdMapper.h"

#include "KDXMLTools.h"

#include <QDebug>

using namespace KDChart;

SerializeCollector::SerializeCollector()
{
    // this space left empty intentionally
}

SerializeCollector::~SerializeCollector()
{
    clear();
}

void SerializeCollector::clear()
{
    Q_FOREACH (QDomElement* e, mMap)
    {
        delete e;
    }
}

SerializeCollector* SerializeCollector::instance()
{
    static SerializeCollector instance;
    return &instance;
}

QDomElement* SerializeCollector::findOrMakeElement(
        QDomDocument& doc,
        const QString& name )
{
    QDomElement* e = findElement( name );
    if( ! e ){
        e = new QDomElement( doc.createElement( name ) );
        mMap[ name ] = e;
    }
    return e;
}

QDomElement* SerializeCollector::findElement( const QString& name )const
{
    if( mMap.contains( name ) )
        return mMap.value( name );
    return 0;
}

const QString SerializeCollector::unresolvedTagName()
{
    return "U_N_R_E_S_O_L_V_E_D";
}
const QString SerializeCollector::unresolvedMapName()
{
    return "kdchart:unresolved-pointers";
}

void SerializeCollector::appendDataToElement( QDomElement& element )const
{
    Q_FOREACH (QDomElement* e, mMap)
    {
        //qDebug() << e->tagName();
        if( e->tagName() != unresolvedMapName() ){
            element.appendChild( *e );
        }
    }
}

QDomElement SerializeCollector::createPointersList(
        QDomDocument& doc, QDomElement& e, const QString& title )
{
    QDomElement list = doc.createElement( title + ":pointers" );
    e.appendChild( list );
    return list;
}

QDomElement SerializeCollector::findOrMakeChild(
        QDomDocument& doc,
        QDomElement& elementsList,
        QDomElement& pointerContainer,
        const QString& title,
        const void* p,
        bool& wasFound )
{
    const QString pointerName( IdMapper::instance()->findOrMakeName( p, title, wasFound ) );

    KDXML::createStringNode( doc, pointerContainer, "kdchart:pointer", pointerName );

    if( ! wasFound ){
        QDomElement storeElement = doc.createElement( pointerName );
        elementsList.appendChild( storeElement );
        return storeElement;
    }
    return QDomElement();
}


void SerializeCollector::storeUnresolvedPointer(
        QDomDocument& doc,
        const QObject* p,
        QDomElement& pointerContainer )
{
    // access (or append, resp.) the global list
    bool wasFound;
    const QString pointerName(
            IdMapper::instance()->findOrMakeName(
                    p, "kdchart:q-object-pointer", wasFound, false ) );

    KDXML::createNodeWithAttribute( doc, pointerContainer,
            "kdchart:unresolved-pointer", "name", pointerName );
}


void SerializeCollector::resolvePointers(
        QDomDocument& doc,
        QDomElement& rootNode )
{
    // make a list of all unresolved pointer names in the rootNode
    QDomNodeList unresolvedList
            = rootNode.elementsByTagName( "kdchart:unresolved-pointer" );

    if( ! unresolvedList.isEmpty() ){
        // access (or append, resp.) the global list
        const QMap<const void*, QString> unresolvedMap
                = IdMapper::instance()->unresolvedMap();
        QMapIterator<const void*, QString> unresolvedIter( unresolvedMap );
        while( unresolvedIter.hasNext() ) {
            unresolvedIter.next();
            const void* p = unresolvedIter.key();
            QString globalName( p ? IdMapper::instance()->findName( p ) : QString("Null") );
            if( globalName.isEmpty() )
                globalName = unresolvedTagName();
            for( int i=0; i<unresolvedList.count(); ++i ){
                QDomElement e = unresolvedList.item(i).toElement();
                if( e.attribute( "name" )
                    == unresolvedIter.value() )
                {
                    e.setTagName( "kdchart:pointer" );
                    e.removeAttribute( "name" );
                    QDomText elementContent =
                            doc.createTextNode( globalName );
                    e.appendChild( elementContent );
                }
            }
        }
    }
}

void SerializeCollector::storePointerName(
        QDomDocument& doc,
        QDomElement& pointerContainer,
        const QString& pointerName )
{
    KDXML::createStringNode( doc, pointerContainer, "kdchart:pointer", pointerName );
}
