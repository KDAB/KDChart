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

#include "KDChartDiagramsSerializer.h"
#include "KDChartSerializeCollector.h"
#include "KDChartAxesSerializer.h"
#include "KDChartAttributesModelSerializer.h"

#include "KDXMLTools.h"

#include <qglobal.h>
#include <QMessageBox>


/**
  \class KDChart::CoordPlanesSerializer KDChartCoordPlanesSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


DiagramsSerializer::DiagramsSerializer()
{
    mAxesS = new AxesSerializer();
    mAttrModelS = new AttributesModelSerializer();
    mGlobalList = "kdchart:diagrams"; // default value, can be
    // overwritten by the title passed to DiagramsSerializer::saveDiagrams()
}

DiagramsSerializer::~DiagramsSerializer()
{
    delete mAttrModelS;
    delete mAxesS;
}

bool DiagramsSerializer::parseDiagrams(
        const QDomElement& e,
        AbstractDiagramList& diags )const
{
    bool bOK = true;
    QDomNode node = e.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            /*
            if( tagName == "kdchart:cartesian-coordinate-plane" ) {
                bool b;
                if( KDXML::readBoolNode( element, b ) )
                    a.setVisible( b );
            } else if( tagName == "kdchart:polar-coordinate-plane" ) {
                QPen p;
                if( KDXML::readPenNode( element, p ) )
                    a.setPen( p );
            } else {
                QPen p;
                if( KDXML::readPenNode( element, p ) )
                    a.setPen( p );
            } else {
                qDebug() << "Unknown subelement of FrameAttributes found:" << tagName;
                bOK = false;
            }
            */
        }
        node = node.nextSibling();
    }
    return bOK;
}

void DiagramsSerializer::saveDiagrams(
        QDomDocument& doc,
        QDomElement& e,
        const ConstAbstractDiagramList& diags,
        const QString& title )const
{
    if( ! title.isEmpty() )
        mGlobalList = title;

    // access (or append, resp.) the global list
    QDomElement* diagsList =
            SerializeCollector::instance()->findOrMakeElement( doc, mGlobalList );

    // create the local list holding names pointing into the global list
    QDomElement pointersList =
            SerializeCollector::createPointersList( doc, e, title );

    Q_FOREACH ( const AbstractDiagram* p, diags )
    {
        bool wasFound;
        QDomElement diagElement =
                SerializeCollector::findOrMakeChild(
                doc,
                *diagsList,
                pointersList,
                "kdchart:diagram",
                p,
                wasFound );
        if( ! wasFound ){
            saveDiagram( doc, diagElement, p );
        }
    }
}

void DiagramsSerializer::saveDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractDiagram* p )const
{
    if( ! p ) return;

    // first save the information hold by the base class
    saveAbstractDiagram( doc, e, *p,
                         "kdchart:abstract-diagram" );

    // then save any diagram type specific information
    const AbstractCartesianDiagram* cartDiag =
            dynamic_cast<const AbstractCartesianDiagram*> ( p );
    if( cartDiag ){
        saveCartDiagram( doc, e, *cartDiag,
                         "kdchart:cartesian-diagram" );
    }else{
        const AbstractPolarDiagram* polDiag =
                dynamic_cast<const AbstractPolarDiagram*> ( p );
        if( polDiag ){
            savePolDiagram( doc, e, *polDiag,
                            "kdchart:polar-diagram" );
        }else{
            saveOtherDiagram( doc, e, *p );
        }
    }
}

void DiagramsSerializer::saveAbstractDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
        doc.createElement( title );
    e.appendChild( diagElement );

    mAttrModelS->saveAttributesModel(
        doc,
        diagElement,
        diagram.attributesModel() );

    KDXML::createBoolNode( doc, diagElement, "AllowOverlappingDataValueTexts",
                           diagram.allowOverlappingDataValueTexts() );
    KDXML::createBoolNode( doc, diagElement, "AntiAliasing",
                           diagram.antiAliasing() );
    KDXML::createBoolNode( doc, diagElement, "PercentMode",
                           diagram.percentMode() );
    KDXML::createIntNode( doc, diagElement, "DatasetDimension",
                          diagram.datasetDimension() );
}

void DiagramsSerializer::saveCartDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractCartesianDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
        doc.createElement( title );
    e.appendChild( diagElement );

    mAxesS->saveCartesianAxes( doc, diagElement,
                               diagram.axes(),
                               "kdchart:axes" );

    // save the reference diagram-pointer and the diagram, if any
    const AbstractCartesianDiagram* refDiag = diagram.referenceDiagram();
    if( refDiag ){
        QDomElement refDiagPtrElement =
                doc.createElement( "ReferenceDiagram" );
        diagElement.appendChild( refDiagPtrElement );
        // access (or append, resp.) the global list
        QDomElement* diagsList =
                SerializeCollector::instance()->findOrMakeElement( doc, mGlobalList );

        // create the local list holding names pointing into the global list
        QDomElement pointersList =
                SerializeCollector::createPointersList( doc, refDiagPtrElement, mGlobalList );

        bool wasFound;
        QDomElement refListElement =
                SerializeCollector::findOrMakeChild(
                doc,
                *diagsList,
                pointersList,
                "kdchart:diagram",
                refDiag,
                wasFound );
        if( ! wasFound ){
            saveDiagram( doc, refListElement, refDiag );
        }
    }

    // ...
}

void DiagramsSerializer::savePolDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractPolarDiagram& diagram,
        const QString& title )const
{

}

void DiagramsSerializer::saveOtherDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractDiagram& diagram )const
{
    Q_UNUSED(doc)
    Q_UNUSED(e)
    Q_UNUSED(diagram)
}
