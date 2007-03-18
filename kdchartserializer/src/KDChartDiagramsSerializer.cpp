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
#include "KDChartCoordPlanesSerializer.h"
#include "KDChartAxesSerializer.h"
#include "KDChartAttributesModelSerializer.h"

#include "KDXMLTools.h"

#include <KDChartLineDiagram>
#include <KDChartBarDiagram>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartRingDiagram>

#include <qglobal.h>
#include <QMessageBox>


/**
  \class KDChart::DiagramsSerializer KDChartDiagramsSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


DiagramsSerializer::DiagramsSerializer( CoordPlanesSerializer* coordS )
{
    mCoordS = coordS;
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
            SerializeCollector::createPointersList( doc, e, mGlobalList );

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

    const LineDiagram*  lineDiag  = dynamic_cast<const LineDiagram*> (  p );
    const BarDiagram*   barDiag   = dynamic_cast<const BarDiagram*> (   p );
    const PieDiagram*   pieDiag   = dynamic_cast<const PieDiagram*> (   p );
    const PolarDiagram* polarDiag = dynamic_cast<const PolarDiagram*> ( p );
    const RingDiagram*  ringDiag  = dynamic_cast<const RingDiagram*> (  p );

    if( lineDiag )
        saveLineDiagram( doc, e, *lineDiag,
                         "kdchart:line-diagram" );
    else if( barDiag )
        saveBarDiagram( doc, e, *barDiag,
                        "kdchart:bar-diagram" );
    else if( pieDiag )
        savePieDiagram( doc, e, *pieDiag,
                        "kdchart:pie-diagram" );
    else if( polarDiag )
        savePolarDiagram( doc, e, *polarDiag,
                          "kdchart:polar-diagram" );
    else if( ringDiag )
        saveRingDiagram( doc, e, *ringDiag,
                         "kdchart:ring-diagram" );
    else
        saveOtherDiagram( doc, e, *p );
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

    // save the attributes model and the root index
    mAttrModelS->saveAttributesModel(
        doc,
        diagElement,
        diagram.attributesModel() );
    KDXML::createModelIndexNode( doc, diagElement, "RootIndex",
                                 diagram.rootIndex() );

    // save the pointer to the associated coordinate plane,
    // and save the plane in the global structure if not there yet
    const CartesianCoordinatePlane* coordPlane =
            static_cast<const CartesianCoordinatePlane*>( diagram.coordinatePlane() );
    if( coordPlane ){
        QDomElement coordPlanePtrElement =
                doc.createElement( "CoodinatePlane" );
        diagElement.appendChild( coordPlanePtrElement );
        // access (or append, resp.) the global list
        QDomElement* planesList =
                SerializeCollector::instance()->findOrMakeElement( doc, mCoordS->globalList() );

        bool wasFound;
        QDomElement globalListElement =
                SerializeCollector::findOrMakeChild(
                doc,
                *planesList,
                coordPlanePtrElement,
                "kdchart:coordinate-plane",
                coordPlane,
                wasFound );
        if( ! wasFound ){
            // Since the diagram is stored in the global structure anyway,
            // it is save to store it right now.
            // So it will not be forgotten, in case it is not embedded in any
            // of the coordinate planes.
            // The wasFound test makes sure it will not be stored twice.
            mCoordS->saveCartPlane(
                    doc, globalListElement, *coordPlane, "hase" );
        }
    }

    KDXML::createBoolNode( doc, diagElement, "AllowOverlappingDataValueTexts",
                           diagram.allowOverlappingDataValueTexts() );
    KDXML::createBoolNode( doc, diagElement, "AntiAliasing",
                           diagram.antiAliasing() );
    KDXML::createBoolNode( doc, diagElement, "PercentMode",
                           diagram.percentMode() );
    KDXML::createIntNode( doc, diagElement, "DatasetDimension",
                          diagram.datasetDimension() );
}

void DiagramsSerializer::saveCartCoordDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractCartesianDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
        doc.createElement( title );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    saveAbstractDiagram( doc, diagElement, diagram,
                         "kdchart:abstract-diagram" );

    // save the axes
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

        bool wasFound;
        QDomElement globalListElement =
                SerializeCollector::findOrMakeChild(
                doc,
                *diagsList,
                refDiagPtrElement,
                "kdchart:diagram",
                refDiag,
                wasFound );
        if( ! wasFound ){
            // Since the diagram is stored in the global structure anyway,
            // it is save to store it right now.
            // So it will not be forgotten, in case it is not embedded in any
            // of the coordinate planes.
            // The wasFound test makes sure it will not be stored twice.
            saveDiagram( doc, globalListElement, refDiag );
        }
        KDXML::createPointFNode(
                doc, refDiagPtrElement, "Offset", diagram.referenceDiagramOffset() );
    }
}

void DiagramsSerializer::savePolCoordDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractPolarDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
            doc.createElement( title );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    saveAbstractDiagram( doc, diagElement, diagram,
                         "kdchart:abstract-diagram" );
    // that's all, there is no to-be-saved information in this class
}


void DiagramsSerializer::saveLineDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const LineDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
            doc.createElement( title );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    saveCartCoordDiagram( doc, diagElement, diagram,
                          "kdchart:cartesian-coordinate-diagram" );
    // then save what is stored in the derived class
    QString s;
    switch( diagram.type() ){
        case LineDiagram::Normal:
                s = "Normal";
                break;
        case LineDiagram::Stacked:
                s = "Stacked";
                break;
        case LineDiagram::Percent:
                s = "Percent";
                break;
        default:
            Q_ASSERT( false ); // all of the types need to be handled
            break;
    }
    KDXML::createStringNode( doc, diagElement, "LineType", s );
}

void DiagramsSerializer::saveBarDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const BarDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
            doc.createElement( title );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    saveCartCoordDiagram( doc, diagElement, diagram,
                          "kdchart:cartesian-coordinate-diagram" );
    // then save what is stored in the derived class
    QString s;
    switch( diagram.type() ){
        case BarDiagram::Normal:
            s = "Normal";
            break;
        case BarDiagram::Stacked:
            s = "Stacked";
            break;
        case BarDiagram::Percent:
            s = "Percent";
            break;
        case BarDiagram::Rows:
            s = "Rows";
            break;
        default:
            Q_ASSERT( false ); // all of the types need to be handled
            break;
    }
    KDXML::createStringNode( doc, diagElement, "BarType", s );
}

void DiagramsSerializer::saveAbstractPieDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractPieDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
            doc.createElement( title );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    savePolCoordDiagram( doc, diagElement, diagram,
                         "kdchart:polar-coordinate-diagram" );

    // then save what is stored in the derived class
    KDXML::createRealNode( doc, diagElement, "Granularity", diagram.granularity() );

    // ...
}

void DiagramsSerializer::savePieDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const PieDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
            doc.createElement( title );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    saveAbstractPieDiagram( doc, diagElement, diagram,
                            "kdchart:abstract-pie-diagram" );
}

void DiagramsSerializer::savePolarDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const PolarDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
            doc.createElement( title );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    savePolCoordDiagram( doc, diagElement, diagram,
                         "kdchart:polar-coordinate-diagram" );
}

void DiagramsSerializer::saveRingDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const RingDiagram& diagram,
        const QString& title )const
{
    QDomElement diagElement =
            doc.createElement( title );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    // first save the information hold by the base class
    saveAbstractPieDiagram( doc, diagElement, diagram,
                            "kdchart:abstract-pie-diagram" );
}

void DiagramsSerializer::saveOtherDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractDiagram& diagram )const
{
    Q_UNUSED(doc)
    Q_UNUSED(e)
    Q_UNUSED(diagram)
    /*
    // first save the information hold by the base class
    saveAbstractDiagram( doc, e, *diagElement,
            "kdchart:abstract-diagram" );
    */
}
