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

const QString DiagramsSerializer::nameOfClass( const AbstractDiagram* p )const
{
    QString classname;
    if( dynamic_cast<const LineDiagram*> (  p ) )
        classname = "KDChart::LineDiagram";
    else if( dynamic_cast<const BarDiagram*> (   p ) )
        classname = "KDChart::BarDiagram";
    else if( dynamic_cast<const PieDiagram*> (   p ) )
        classname = "KDChart::PieDiagram";
    else if( dynamic_cast<const PolarDiagram*> ( p ) )
        classname = "KDChart::PolarDiagram";
    else if( dynamic_cast<const RingDiagram*> (  p ) )
        classname = "KDChart::RingDiagram";
    else
        classname = "UNKNOWN";
    return classname;
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
                nameOfClass( p ),
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

bool DiagramsSerializer::parseAbstractDiagram(
        const QDomElement& container, AbstractDiagram& diagram )const
{
    bool bOK = true;

    return bOK;
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
    if( coordPlane && mCoordS ){
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
                mCoordS->nameOfClass( coordPlane ),
                coordPlane,
                wasFound );
        if( ! wasFound ){
            // Since the diagram is stored in the global structure anyway,
            // it is save to store it right now.
            // So it will not be forgotten, in case it is not embedded in any
            // of the coordinate planes.
            // The wasFound test makes sure it will not be stored twice.
            mCoordS->saveCartPlane(
                    doc, globalListElement, *coordPlane, "diagCoordPlane" );
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


bool DiagramsSerializer::parseCartCoordDiagram(
        const QDomElement& container, AbstractCartesianDiagram& diagram )const
{
    bool bOK = true;

    return bOK;
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

    // then save what is stored in the derived class

    // save the axes
    mAxesS->saveCartesianAxes( doc, diagElement,
                               diagram.axes(),
                               "kdchart:axes" );

    // save the reference diagram(-pointer) and the respective offset, if any
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
                nameOfClass( refDiag ),
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


bool DiagramsSerializer::parseLineDiagram(
        const QDomElement& container, LineDiagram*& diagram )const
{
    bool bOK = true;
    if( !container.isNull() ) { // was really an element
        const QString diagName = container.tagName();
        //qDebug() << "\n    DiagramsSerializer::parseLineDiagram() processing" << diagName;
        QObject* p;
        if( AttributesSerializer::findQObjectPointer( diagName, p ) ){
            diagram = dynamic_cast<LineDiagram*>(p);
            if( diagram ){
                QDomNode node = container.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "kdchart:cartesian-coordinate-diagram" ) {
                            if( ! parseCartCoordDiagram( element, *diagram ) ){
                                qDebug() << "Could not parse base class of LineDiagram.";
                                bOK = false;
                            }
                        /*
                        } else if( tagName == "Visible" ) {
                            bool b;
                            if( KDXML::readBoolNode( element, b ) )
                                diagram->setVisible( b );
                            else
                                bOK = false;
                        } else if( tagName == "ReferenceArea" ) {
                            QObject* ptr;
                            //qDebug() << " a ";
                            if( AttributesSerializer::parseQObjectPointerNode( element.firstChild(), ptr ) ){
                                if( ptr ){
                                    QWidget* wPtr = dynamic_cast<QWidget*>(ptr);
                                    if( wPtr ){
                                        diagram->setReferenceArea( wPtr );
                                    }else{
                                        qDebug() << "Error: Value of LineDiagram/ReferenceArea must be a QWidget*";
                                        bOK = false;
                                    }
                                }else{
                                    diagram->setReferenceArea( 0 ); // a Null pointer means no bug
                                }
                            }else{
                                bOK = false;
                            }
                            //qDebug() << " b ";
                        } else if( tagName == "kdchart:diagrams:pointers" ) {
                            // parse the map of associated diagrams
                            QDomNode node2 = element.firstChild();
                            while( !node2.isNull() ) {
                                QObject* ptr;
                                if( AttributesSerializer::parseQObjectPointerNode( node2, ptr ) ){
                                    {
                                                     AbstractDiagram* dPtr = dynamic_cast<AbstractDiagram*>(ptr);
                                                     if( dPtr ){
                                                         diagram->addDiagram( dPtr );
                                                     }else{
                                                         qDebug() << "Error: Values of LineDiagram/kdchart:diagrams:pointers must be AbstractDiagram pointers";
                                                         bOK = false;
                                                     }
                                    }
                                }else{
                                    bOK = false;
                                }
                                node2 = node2.nextSibling();
                            }
                        } else if( tagName == "Alignment" ) {
                            Qt::Alignment a;
                            if( KDXML::readAlignmentNode( element, a ) )
                                diagram->setAlignment( a );
                            else
                                bOK = false;
                        } else if( tagName == "Position" ) {
                            QString s;
                            if( KDXML::readStringNode( element, s ) )
                                diagram->setPosition( Position::fromName( s.toLatin1() ) );
                            else
                                bOK = false;
                        } else if( tagName == "FloatingPosition" ) {
                            RelativePosition pos;
                            if( AttributesSerializer::parseRelativePosition( element, pos ) )
                                diagram->setFloatingPosition( pos );
                            else
                                bOK = false;
                        } else if( tagName == "Orientation" ) {
                            Qt::Orientation o;
                            if( KDXML::readOrientationNode( element, o ) )
                                diagram->setOrientation( o );
                            else
                                bOK = false;
                        } else if( tagName == "ShowLines" ) {
                            bool b;
                            if( KDXML::readBoolNode( element, b ) )
                                diagram->setShowLines( b );
                            else
                                bOK = false;
                        } else if( tagName == "TextsMap" ) {
                            // parse the map of explicitely set texts
                            QDomNode node2 = element.firstChild();
                            while( !node2.isNull() ) {
                                QDomElement ele2 = node2.toElement();
                                if( !ele2.isNull() ) { // was really an element
                                    QString tagName2 = ele2.tagName();
                                    if( tagName2 == "item" ) {
                                        int dataset;
                                        QString txt;
                                        if( KDXML::findIntAttribute( ele2, "dataset", dataset ) &&
                                            KDXML::findStringAttribute( ele2, "text", txt ) ){
                                            diagram->setText( dataset, txt );
                                            }else{
                                                qDebug() << "Invalid item in LineDiagram/TextsMap found.";
                                                bOK = false;
                                            }
                                    }else{
                                        qDebug() << "Unknown subelement of LineDiagram/TextsMap found:" << tagName2;
                                        bOK = false;
                                    }
                                }
                                node2 = node2.nextSibling();
                            }
                        } else if( tagName == "UseAutomaticMarkerSize" ) {
                            bool b;
                            if( KDXML::readBoolNode( element, b ) )
                                diagram->setUseAutomaticMarkerSize( b );
                            else
                                bOK = false;
                        } else if( tagName == "TextAttributes" ) {
                            TextAttributes ta;
                            if( AttributesSerializer::parseTextAttributes( element, ta ) )
                                diagram->setTextAttributes( ta );
                            else
                                bOK = false;
                        } else if( tagName == "TitleText" ) {
                            QString s;
                            if( KDXML::readStringNode( element, s ) )
                                diagram->setTitleText( s );
                            else
                                bOK = false;
                        } else if( tagName == "TitleTextAttributes" ) {
                            TextAttributes ta;
                            if( AttributesSerializer::parseTextAttributes( element, ta ) )
                                diagram->setTitleTextAttributes( ta );
                            else
                                bOK = false;
                        } else if( tagName == "Spacing" ) {
                            int i;
                            if( KDXML::readIntNode( element, i ) )
                                diagram->setSpacing( i );
                            else
                                bOK = false;
                        */
                        } else {
                            qDebug() << "Unknown subelement of LineDiagram found:" << tagName;
                            bOK = false;
                        }
                    }
                    node = node.nextSibling();
                }
            }else{
                qDebug()<< "Could not parse LineDiagram. Global pointer"
                        << diagName << "is not a KDChart::LineDiagram-ptr.";
                bOK = false;
            }
        }else{
            qDebug()<< "Could not parse LineDiagram. Pointer"
                    << diagName << "not found in global list.";
            bOK = false;
        }
    }
    return bOK;
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
    KDXML::createRealNode( doc, diagElement, "Granularity",   diagram.granularity() );
    KDXML::createIntNode(  doc, diagElement, "StartPosition", diagram.startPosition() );
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
    // that's all, there is no to-be-saved information in this class
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

    // then save what is stored in the derived class
    KDXML::createIntNode(  doc, diagElement, "ZeroDegreePosition",   diagram.zeroDegreePosition() );
    KDXML::createBoolNode( doc, diagElement, "RotateCircularLabels", diagram.rotateCircularLabels() );
    KDXML::createPositionBooleansNode(
            doc, diagElement, "ShowDelimitersAtPosition",
            diagram.showDelimitersAtPosition( Position::Unknown ),
            diagram.showDelimitersAtPosition( Position::Center ),
            diagram.showDelimitersAtPosition( Position::NorthWest ),
            diagram.showDelimitersAtPosition( Position::North ),
            diagram.showDelimitersAtPosition( Position::NorthEast ),
            diagram.showDelimitersAtPosition( Position::East ),
            diagram.showDelimitersAtPosition( Position::SouthEast ),
            diagram.showDelimitersAtPosition( Position::South ),
            diagram.showDelimitersAtPosition( Position::SouthWest ),
            diagram.showDelimitersAtPosition( Position::West ),
            diagram.showDelimitersAtPosition( Position::Floating ) );
    KDXML::createPositionBooleansNode(
            doc, diagElement, "ShowLabelsAtPosition",
            diagram.showLabelsAtPosition( Position::Unknown ),
            diagram.showLabelsAtPosition( Position::Center ),
            diagram.showLabelsAtPosition( Position::NorthWest ),
            diagram.showLabelsAtPosition( Position::North ),
            diagram.showLabelsAtPosition( Position::NorthEast ),
            diagram.showLabelsAtPosition( Position::East ),
            diagram.showLabelsAtPosition( Position::SouthEast ),
            diagram.showLabelsAtPosition( Position::South ),
            diagram.showLabelsAtPosition( Position::SouthWest ),
            diagram.showLabelsAtPosition( Position::West ),
            diagram.showLabelsAtPosition( Position::Floating ) );
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
    saveAbstractPieDiagram( doc, diagElement, diagram,
                            "kdchart:abstract-pie-diagram" );

    // then save what is stored in the derived class
    KDXML::createBoolNode(  doc, diagElement, "RelativeThickness",   diagram.relativeThickness() );
}

void DiagramsSerializer::saveOtherDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractDiagram& diagram )const
{
    QDomElement diagElement =
            doc.createElement( "kdchart:user-defined-diagram" );
    e.appendChild( diagElement );

    // first save the information hold by the base class
    saveAbstractDiagram( doc, diagElement, diagram,
                         "kdchart:abstract-diagram" );
    // that's all, there is no to-be-saved information in this class
}
