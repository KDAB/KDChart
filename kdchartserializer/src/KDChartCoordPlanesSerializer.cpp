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

#include "KDChartCoordPlanesSerializer.h"
#include "KDChartSerializeCollector.h"
#include "KDChartDiagramsSerializer.h"

#include "KDXMLTools.h"

#include <qglobal.h>
#include <QMessageBox>


/**
  \class KDChart::CoordPlanesSerializer KDChartCoordPlanesSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


static QString globalListName;

CoordPlanesSerializer::CoordPlanesSerializer()
{
    mDiagS = new DiagramsSerializer( this );
    globalListName = "kdchart:diagrams"; // default value, can be
    // overwritten by the title passed to CoordPlanesSerializer::savePlanes()
}

CoordPlanesSerializer::~CoordPlanesSerializer()
{
    delete mDiagS;
}

bool CoordPlanesSerializer::parsePlanes(
        const QDomElement& e,
        CoordinatePlaneList& planes )const
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

void CoordPlanesSerializer::savePlanes(
        QDomDocument& doc,
        QDomElement& e,
        const CoordinatePlaneList& planes,
        const QString& title )const
{
    if( ! title.isEmpty() )
        globalListName = title;

    // access (or append, resp.) the global list
    QDomElement* planesList =
            SerializeCollector::instance()->findOrMakeElement( doc, globalListName );

    // create the local list holding names pointing into the global list
    QDomElement pointersList =
            SerializeCollector::createPointersList( doc, e, globalListName );

    Q_FOREACH ( AbstractCoordinatePlane* p, planes )
    {
        bool wasFound;
        QDomElement planeElement =
                SerializeCollector::findOrMakeChild(
                doc,
                *planesList,
                pointersList,
                "kdchart:coordinate-plane",
                p,
                wasFound );
        if( ! wasFound ){
            // first save the information hold by the base class
            saveAbstractPlane( doc, planeElement, *p,
                            "kdchart:abstract-coordinate-plane" );

            // then save any plane type specific information
            CartesianCoordinatePlane* cartPlane =
                    dynamic_cast<CartesianCoordinatePlane*> ( p );
            if( cartPlane ){
                saveCartPlane( doc, planeElement, *cartPlane,
                            "kdchart:cartesian-coordinate-plane" );
            }else{
                PolarCoordinatePlane* polPlane =
                        dynamic_cast<PolarCoordinatePlane*> ( p );
                if( polPlane ){
                    savePolPlane( doc, planeElement, *polPlane,
                                "kdchart:polar-coordinate-plane" );
                }else{
                    saveOtherPlane( doc, planeElement, *p );
                }
            }
        }
    }
}

void CoordPlanesSerializer::saveAbstractPlane(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractCoordinatePlane& plane,
        const QString& title )const
{
    QDomElement planeElement =
        doc.createElement( title );
    e.appendChild( planeElement );

    mDiagS->saveDiagrams( doc,
                          planeElement,
                          plane.diagrams(),
                          "kdchart:diagrams" );

}

void CoordPlanesSerializer::saveCartPlane(
        QDomDocument& doc,
        QDomElement& e,
        const CartesianCoordinatePlane& plane,
        const QString& title )const
{
    QDomElement planeElement =
        doc.createElement( title );
    e.appendChild( planeElement );

}

void CoordPlanesSerializer::savePolPlane(
        QDomDocument& doc,
        QDomElement& e,
        const PolarCoordinatePlane& plane,
        const QString& title )const
{

}

void CoordPlanesSerializer::saveOtherPlane(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractCoordinatePlane& plane )const
{
    Q_UNUSED(doc)
    Q_UNUSED(e)
    Q_UNUSED(plane)
}

const QString CoordPlanesSerializer::globalList()const
{
    return globalListName;
}
