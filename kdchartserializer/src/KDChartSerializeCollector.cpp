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


// Global objects of the following classes can be
// instantiated by initializeParsedGlobalPointers()
#include "KDChartAttributesModel"
#include "KDChartCartesianAxis"
// once that class is implemented: #include "KDChartPolarAxis"
#include "KDChartChart"
#include "KDChartCartesianCoordinatePlane"
#include "KDChartPolarCoordinatePlane"
#include "KDChartBarDiagram"
#include "KDChartLineDiagram"
#include "KDChartPieDiagram"
#include "KDChartPolarDiagram"
#include "KDChartRingDiagram"
#include "KDChartHeaderFooter"
#include "KDChartLegend"


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


// ******************* parsing the data ***************************

bool SerializeCollector::initializeParsedGlobalPointers(
        const QDomElement& globalObjectsNode )
{
    instance()->parsedPointersMap().clear();

    QDomNode n = globalObjectsNode.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            // the node really is an element
            QString tagName = e.tagName();
            if( tagName == "<kdchart:attribute-models>" ||
                tagName == "<kdchart:axes>" ||
                tagName == "<kdchart:charts>" ||
                tagName == "<kdchart:coordinate-planes>" ||
                tagName == "<kdchart:diagrams>" ||
                tagName == "<kdchart:headers-footers>" ||
                tagName == "<kdchart:legends>" ){
                QDomNode n2 = e.firstChild();
                while(!n2.isNull()) {
                    QDomElement e2 = n2.toElement(); // try to convert the node to an element.
                    if(!e2.isNull()) {
                    // the node really is an element
                        QString objectName = e2.tagName();
                        QString className;
                        if( KDXML::findStringAttribute( e2, "Classname", className ) ){
                            if( className == "KDChart::AttributesModel" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new AttributesModel(0, 0);
                            } else if( objectName == "KDChart::CartesianAxis" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new CartesianAxis( 0 );
                            /* once PolarAxis is implemented:
                            } else if( objectName == "KDChart::PolarAxis" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new PolarAxis(0, 0);
                            */
                            } else if( objectName == "KDChart::Chart" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new Chart( 0 );
                            } else if( objectName == "KDChart::CartesianCoordinatePlane" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new CartesianCoordinatePlane( 0 );
                            } else if( objectName == "KDChart::PolarCoordinatePlane" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new PolarCoordinatePlane( 0 );
                            } else if( objectName == "KDChart::BarDiagram" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new BarDiagram(0, 0);
                            } else if( objectName == "KDChart::LineDiagram" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new LineDiagram(0, 0);
                            } else if( objectName == "KDChart::PieDiagram" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new PieDiagram(0, 0);
                            } else if( objectName == "KDChart::PolarDiagram" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new PolarDiagram(0, 0);
                            } else if( objectName == "KDChart::RingDiagram" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new RingDiagram(0, 0);
                            } else if( objectName == "KDChart::HeaderFooter" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new HeaderFooter( 0 );
                            } else if( objectName == "KDChart::Legend" ){
                                instance()->parsedPointersMap()[ objectName ]
                                        = new Legend( 0 );
                            } else {
                                qDebug() << "Non-critical information by SerializeCollector::initializeParsedGlobalPointers()\n"
                                        "    Unknown subelement of " << tagName
                                        << " found: " << objectName << "\n"
                                        "    Make sure to instantiate this object\n"
                                        "    and store its pointer in the map of parsed pointers:\n"
                                        "    KDChart::SerializeCollector::instance()->parsedPointersMap()";
                                // It might well be that someone has stored additional
                                // top-level information here, so we just ignore them.
                            }
                        } else {
                            qDebug() << "CRITICAL information by SerializeCollector::initializeParsedGlobalPointers()\n"
                                    "    Subelement of " << tagName
                                    << " has no \"Classname\" attribute: " << objectName << "\n"
                                    "    Can not parse that.";
                        }
                    }
                    n2 = n2.nextSibling();
                }
            } else {
                qDebug() << "Non-critical information by SerializeCollector::initializeParsedGlobalPointers()\n"
                "    Unknown subelement of " << globalObjectsNode.tagName()
                        << " found: " << tagName << "\n"
                "    Make sure to instantiate its top-level objects\n"
                "    and store these object's pointers in the map of parsed pointers:\n"
                "    KDChart::SerializeCollector::instance()->parsedPointersMap()";
                // It might well be that someone has stored additional
                // top-level information here, so we just ignore them.
            }
        }
        n = n.nextSibling();
    }
    return true;
}

ParsedPointersMap& SerializeCollector::parsedPointersMap()
{
    return mParsedPointersMap;
}

bool SerializeCollector::foundParsedPointer(
        const QString& globalName,
        QObject*& p )
{
    const bool bFound = instance()->parsedPointersMap().contains( globalName );
    if( bFound )
        p = instance()->parsedPointersMap().value( globalName );
    else
        p = 0;
    return bFound;
}



// ************** storing the data *******************************

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

void SerializeCollector::appendDataToElement(
        QDomDocument& doc,
        QDomElement& element,
        const QString& name )const
{
    QDomElement list = doc.createElement( name );
    element.appendChild( list );
    Q_FOREACH (QDomElement* e, mMap)
    {
        //qDebug() << e->tagName();
        if( e->tagName() != unresolvedMapName() ){
            list.appendChild( *e );
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
        const QString& classname,
        const void* p,
        bool& wasFound )
{
    const QString pointerName( IdMapper::instance()->findOrMakeName( p, title, wasFound ) );

    KDXML::createStringNode( doc, pointerContainer, "kdchart:pointer", pointerName );

    if( ! wasFound ){
        QDomElement storeElement = doc.createElement( pointerName );
        elementsList.appendChild( storeElement );
        storeElement.setAttribute( "Classname", classname );
        return storeElement;
    }
    return QDomElement();
}


void SerializeCollector::storeUnresolvedPointer(
        QDomDocument& doc,
        const QObject* p,
        QDomElement& pointerContainer )
{
    if( ! p ){
        KDXML::createNodeWithAttribute( doc, pointerContainer,
                                        "kdchart:pointer", "name", "Null" );
    }else{
        // access (or append, resp.) the global list
        bool wasFound;
        const QString pointerName(
                IdMapper::instance()->findOrMakeName(
                        p, "kdchart:q-object-pointer", wasFound, false ) );

        //qDebug() << "SerializeCollector::storeUnresolvedPointer() storing" << pointerName << " wasFound:" << wasFound;
        KDXML::createNodeWithAttribute( doc, pointerContainer,
                                        "kdchart:unresolved-pointer", "name", pointerName );
    }
}


void SerializeCollector::resolvePointers(
        QDomDocument& doc,
        QDomElement& rootNode )
{
    qDebug() << "SerializeCollector::resolvePointers()";

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
            const QString foundName  = IdMapper::instance()->findName( p );
            const bool wasFound = ! foundName.isEmpty();
            const QString globalName(
                    p ? (wasFound ? foundName : unresolvedTagName()): QString("Null") );

            //qDebug() << "pointer:" << p << " name:" << unresolvedIter.value()
            //        << " wasFound: " << wasFound << " globalName:" << globalName;

            //qDebug() << "unresolvedList.count() initially is" << unresolvedList.count();
            for( int i=0; i<unresolvedList.count(); ++i ){
                QDomElement e = unresolvedList.item(i).toElement();
                if( e.attribute( "name" )
                    == unresolvedIter.value() )
                {
                    //qDebug() << "resolving" << e.attribute( "name" );
                    e.removeAttribute( "name" );
                    QDomText elementContent =
                            doc.createTextNode( globalName );
                    e.appendChild( elementContent );
                    e.setTagName( "kdchart:pointer" );
                    //qDebug() << "unresolvedList.count() is" << unresolvedList.count();

                    // We go back one item, since the e.setTagName() command
                    // has automatically removed the element from the unresolvedList
                    // by some of Qt's built-in magic.
                    --i;
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
