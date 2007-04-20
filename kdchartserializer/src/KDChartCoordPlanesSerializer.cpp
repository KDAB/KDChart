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
#include "KDChartIdMapper.h"
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
                nameOfClass( p ),
                p,
                wasFound );
        if( ! wasFound ){
            savePlane( doc, planeElement, p );
        }
    }
}


bool CoordPlanesSerializer::parsePlane(
        const QDomNode& rootNode,
        const QDomNode& pointerNode,
        AbstractCoordinatePlane*& planePtr )const
{
    bool bOK = true;

    AbstractCoordinatePlane* plane=0;
    QObject* ptr;
    QString ptrName;
    bool wasParsed;
    const bool pointerFound =
            AttributesSerializer::parseQObjectPointerNode(
                    pointerNode, ptr,
                    ptrName, wasParsed, true ) && ptr;

    if( ptrName.isEmpty() ){
        qDebug()<< "Could not parse coord-plane. Global pointer node is invalid.";
        bOK = false;
    }else{
        if( pointerFound ){
            plane = dynamic_cast<AbstractCoordinatePlane*>(ptr);
            if( ! plane ){
                qDebug()<< "Could not parse coord-plane. Global pointer"
                        << ptrName << "is no AbstractCoordinatePlane-ptr.";
                bOK = false;
            }
        }else{
            qDebug()<< "Could not parse coord-plane. Global pointer"
                    << ptrName << "is no AbstractCoordinatePlane-ptr.";
            bOK = false;
        }
    }


    if( bOK && wasParsed ){
        planePtr = plane;
        return true;
    }


    QDomElement container;
    if( bOK ){
        container = SerializeCollector::findStoredGlobalElement(
                rootNode, ptrName, "kdchart:coordinate-planes" );
        bOK = ! container.tagName().isEmpty();
    }

    if( bOK ) {
        SerializeCollector::instance()->setWasParsed( plane, true );

        CartesianCoordinatePlane* cartPlane = dynamic_cast<CartesianCoordinatePlane*> ( plane );
        PolarCoordinatePlane*     polPlane  = dynamic_cast<PolarCoordinatePlane*> (     plane );

        if( cartPlane ){
            bOK = parseCartPlane( container, *cartPlane );
            if( bOK )
                plane = cartPlane;
        }else if( polPlane ){
            bOK = parsePolPlane( container,  *polPlane );
            if( bOK )
                plane = polPlane;
        }else{
            bOK = parseOtherPlane( container, *plane );
            if( bOK )
                planePtr = plane;
        }
    }
    return bOK;
}

void CoordPlanesSerializer::savePlane(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractCoordinatePlane* p )const
{
    if( ! p ) return;

    const CartesianCoordinatePlane* cartPlane =
            dynamic_cast<const CartesianCoordinatePlane*> ( p );
    if( cartPlane ){
        saveCartPlane( doc, e, *cartPlane,
                       "kdchart:cartesian-coordinate-plane" );
    }else{
        const PolarCoordinatePlane* polPlane =
                dynamic_cast<const PolarCoordinatePlane*> ( p );
        if( polPlane ){
            savePolPlane( doc, e, *polPlane,
                          "kdchart:polar-coordinate-plane" );
        }else{
            saveOtherPlane( doc, e, *p );
        }
    }
}

const QString CoordPlanesSerializer::nameOfClass( const AbstractCoordinatePlane* p )const
{
    QString classname;
    if( dynamic_cast<const CartesianCoordinatePlane*> ( p ) )
        classname = "KDChart::CartesianCoordinatePlane";
    else if( dynamic_cast<const PolarCoordinatePlane*> ( p ) )
        classname = "KDChart::PolarCoordinatePlane";
    else
        classname = "UNKNOWN";
    return classname;
}


bool CoordPlanesSerializer::parseAbstractPlane(
        const QDomElement& container, AbstractCoordinatePlane& plane )const
{
    bool bOK = true;
    QDomNode node;

    bool bNoDiagramParsedYet = true;
    node = container.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "kdchart:diagrams" ) {
                QDomNode node2 = element.firstChild();
                while( ! node2.isNull() ) {
                    QDomElement ele2 = node2.toElement();
                    if( ! ele2.isNull() ) { // was really an element
                        QObject* ptr;
                        QString ptrName;
                        bool wasParsed;
                        if( AttributesSerializer::parseQObjectPointerNode(
                                    ele2, ptr,
                                    ptrName, wasParsed, true ) )
                        {
                            AbstractDiagram *diagram = dynamic_cast<AbstractDiagram*>(ptr);
                            if( diagram ){
                                if( bNoDiagramParsedYet ){
                                    plane.replaceDiagram( diagram );
                                    bNoDiagramParsedYet = false;
                                }else{
                                    plane.addDiagram( diagram );
                                }
                            }else{
                                qDebug()<< "Could not parse AbstractCoordinatePlane. Global pointer"
                                        << ele2.tagName() << "is not a KDChart::AbstractDiagram-ptr.";
                                bOK = false;
                            }
                        }else{
                            qDebug()<< "Could not parse AbstractCoordinatePlane. Global pointer"
                                    << ele2.tagName() << "was not found in global list.";
                            bOK = false;
                        }
                    }
                    node2 = node2.nextSibling();
                }
            } /*else if( tagName == "AllowOverlappingDataValueTexts" ) {
                bool b;
                if( KDXML::readBoolNode( element, b ) ){
                    diagram.setAllowOverlappingDataValueTexts( b );
                }else{
                    qDebug()<< "Could not parse AbstractCoordinatePlane. Element"
                            << tagName << "has invalid content.";
                }
            } else if( tagName == "DatasetDimension" ) {
                int i;
                if( KDXML::readIntNode( element, i ) ){
                    diagram.setDatasetDimension( i );
                }else{
                    qDebug()<< "Could not parse AbstractCoordinatePlane. Element"
                            << tagName << "has invalid content.";
                }
        } */else {
                qDebug() << "Unknown subelement of AbstractCoordinatePlane found:" << tagName;
                bOK = false;
            }
        }
        node = node.nextSibling();
    }
    return bOK;
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

    // save the associated diagrams
    mDiagS->saveDiagrams( doc,
                          planeElement,
                          plane.diagrams(),
                          "kdchart:diagrams" );

    KDXML::createDoubleNode( doc, planeElement, "ZoomFactorX",
                             plane.zoomFactorX() );
    KDXML::createDoubleNode( doc, planeElement, "ZoomFactorY",
                             plane.zoomFactorY() );
    KDXML::createPointFNode( doc, planeElement, "ZoomCenter",
                             plane.zoomCenter() );
    AttributesSerializer::saveGridAttributes(
            doc, planeElement, plane.globalGridAttributes(), "GlobalGridAttributes" );

    // save the reference plane(-pointer), if any
    const AbstractCoordinatePlane* refPlane = plane.referenceCoordinatePlane();
    if( refPlane ){
        QDomElement refPlanePtrElement =
                doc.createElement( "ReferencePlane" );
        planeElement.appendChild( refPlanePtrElement );
        // access (or append, resp.) the global list
        QDomElement* planesList =
                SerializeCollector::instance()->findOrMakeElement( doc, globalListName );

        bool wasFound;
        QDomElement globalListElement =
                SerializeCollector::findOrMakeChild(
                doc,
                *planesList,
                refPlanePtrElement,
                "kdchart:coordinate-plane",
                nameOfClass( refPlane ),
                refPlane,
                wasFound );
        if( ! wasFound ){
            // Since the plane is stored in the global structure anyway,
            // it is save to store it right now.
            // So it will not be forgotten, in case it is not embedded in a
            // chart.
            // The wasFound test makes sure it will not be stored twice.
            savePlane( doc, globalListElement, refPlane );
        }
    }

    // save the pointer to the associated chart,
    // and save the chart in the global structure if not there yet
    const Chart* chart =
            static_cast<const Chart*>( plane.parent() );
    if( chart ){
        // try to access the global charts list:
        // If there is none, searching for the chart pointer's name makes no sense.
        bool bOK = SerializeCollector::instance()->findElement( "kdchart:charts" ) != 0;
        if( bOK ){
            const QString chartName( IdMapper::instance()->findName( chart ) );
            bOK = ! chartName.isEmpty();
            if( bOK ){
                QDomElement chartPtrElement =
                        doc.createElement( "Chart" );
                planeElement.appendChild( chartPtrElement );
                SerializeCollector::instance()->storePointerName(
                        doc, chartPtrElement, chartName );
            }else{
                qDebug() << "--- CoordPlanesSerializer ---";
                qDebug() << "Warning: The coordinate plane's parent (" << chart << ") is not in the global charts list.";
                IdMapper::instance()->debugOut();
            }
        }else{
            qDebug() << "--- CoordPlanesSerializer ---";
            qDebug() << "Warning: No global charts list found.";
        }
        if( ! bOK ){
            // We warn via debug, but we do not break,
            // since the result can still be used.
            qDebug() << "Wrong usage of KD Chart Serializer ??";
            qDebug() << "KDChart::CoordPlanesSerializer::savePlanes() was called before the associated chart was stored";
            qDebug() << "So we can not store the kdchart-pointer in the coordinate planes sub-tree.";
            qDebug() << "--- CoordPlanesSerializer ---";
        }
    }
}


bool CoordPlanesSerializer::parseCartPlane(
        const QDomElement& container, CartesianCoordinatePlane& plane )const
{
    bool bOK = true;
    return bOK;
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

    // first save the information hold by the base class
    saveAbstractPlane( doc, planeElement, plane,
                       "kdchart:abstract-coordinate-plane" );

    // then save any plane type specific information
    KDXML::createBoolNode( doc, planeElement, "IsometricScaling",
                           plane.doesIsometricScaling() );
    KDXML::createRealPairNode( doc, planeElement, "HorizontalRange",
                               plane.horizontalRange() );
    KDXML::createRealPairNode( doc, planeElement, "VerticalRange",
                               plane.verticalRange() );
    KDXML::createIntNode( doc, planeElement, "AutoAdjustHorizontalRangeToData",
                          plane.autoAdjustHorizontalRangeToData() );
    KDXML::createIntNode( doc, planeElement, "AutoAdjustVerticalRangeToData",
                          plane.autoAdjustVerticalRangeToData() );
    if( plane.hasOwnGridAttributes( Qt::Horizontal ) ){
        AttributesSerializer::saveGridAttributes(
                doc, planeElement, plane.gridAttributes( Qt::Horizontal ),
                "HorizontalGridAttributes" );
    }
    if( plane.hasOwnGridAttributes( Qt::Vertical ) ){
        AttributesSerializer::saveGridAttributes(
                doc, planeElement, plane.gridAttributes( Qt::Vertical ),
                "VerticalGridAttributes" );
    }
    KDXML::createBoolNode( doc, planeElement, "AutoAdjustGridToZoom",
                           plane.autoAdjustGridToZoom() );
    saveAxesCalcMode(
            doc, planeElement, plane.axesCalcModeX(), "AxesCalcModeX" );
    saveAxesCalcMode(
            doc, planeElement, plane.axesCalcModeY(), "AxesCalcModeY" );
}


bool CoordPlanesSerializer::parsePolPlane(
        const QDomElement& container, PolarCoordinatePlane& plane )const
{
    bool bOK = true;
    return bOK;
}

void CoordPlanesSerializer::savePolPlane(
        QDomDocument& doc,
        QDomElement& e,
        const PolarCoordinatePlane& plane,
        const QString& title )const
{
    QDomElement planeElement =
            doc.createElement( title );
    e.appendChild( planeElement );

    // first save the information hold by the base class
    saveAbstractPlane( doc, planeElement, plane,
                       "kdchart:abstract-coordinate-plane" );

    // then save any plane type specific information
    KDXML::createRealNode( doc, planeElement, "StartPosition",
                           plane.startPosition() );
    if( plane.hasOwnGridAttributes( true ) ){
        AttributesSerializer::saveGridAttributes(
                doc, planeElement, plane.gridAttributes( true ),
                "CircularGridAttributes" );
    }
    if( plane.hasOwnGridAttributes( false ) ){
        AttributesSerializer::saveGridAttributes(
                doc, planeElement, plane.gridAttributes( false ),
                "SagittalGridAttributes" );
    }
}


bool CoordPlanesSerializer::parseOtherPlane(
        const QDomElement& container, AbstractCoordinatePlane& plane )const
{
    bool bOK = true;
    return bOK;
}

void CoordPlanesSerializer::saveOtherPlane(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractCoordinatePlane& plane )const
{
    QDomElement planeElement =
            doc.createElement( "kdchart:user-defined-coordinate-plane" );
    e.appendChild( planeElement );

    // first save the information hold by the base class
    saveAbstractPlane( doc, planeElement, plane,
                       "kdchart:abstract-coordinate-plane" );
    // that's all, there is no to-be-saved information in this class
}


void CoordPlanesSerializer::saveAxesCalcMode(
        QDomDocument& doc,
        QDomElement& e,
        const CartesianCoordinatePlane::AxesCalcMode& mode,
        const QString& title )const
{
    QString name;
    switch( mode ){
        case CartesianCoordinatePlane::Linear:
            name = "Linear";
            break;
        case CartesianCoordinatePlane::Logarithmic:
            name = "Logarithmic";
            break;
        default:
            Q_ASSERT( false ); // all of the types need to be handled
            break;
    }
    KDXML::createStringNode( doc, e, title, name );
}

const QString CoordPlanesSerializer::globalList()const
{
    return globalListName;
}
