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

#include "KDChartAxesSerializer.h"
#include "KDChartAttributesSerializer.h"
#include "KDChartAbstractAreaBaseSerializer.h"
#include "KDChartSerializeCollector.h"

#include "KDXMLTools.h"

#include <qglobal.h>
#include <QMessageBox>


/**
  \class KDChart::AxesSerializer KDChartAxesSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


AxesSerializer::AxesSerializer()
{
    // this space left empty intentionally
}

AxesSerializer::~AxesSerializer()
{
    // this space left empty intentionally
}

void AxesSerializer::saveCartesianAxes(
        QDomDocument& doc,
        QDomElement& e,
        const CartesianAxisList& axes,
        const QString& title )const
{
    // access (or append, resp.) the global list
    QDomElement* axesList =
            SerializeCollector::instance()->findOrMakeElement( doc, title );

    // create the local list holding names pointing into the global list
    QDomElement pointersList =
            SerializeCollector::createPointersList( doc, e, title );

    Q_FOREACH ( const CartesianAxis* p, axes )
    {
        bool wasFound;
        QDomElement axisElement =
                SerializeCollector::findOrMakeChild(
                        doc,
                        *axesList,
                        pointersList,
                        "kdchart:axis",
                        nameOfClass( p ),
                        p,
                        wasFound );
        if( ! wasFound ){
            saveCartesianAxis( doc, axisElement, *p );
        }
    }
}

//TODO once PolarAxis is implemented:
/*
void AxesSerializer::savePolarAxes(
        QDomDocument& doc,
        QDomElement& e,
        const PolarAxisList& axes,
        const QString& title )const
{
    QDomElement axesList =
            doc.createElement( title );
    e.appendChild( axesList );
    Q_FOREACH ( const PolarAxis* p, axes )
    {
        bool wasFound;
        QDomElement axisElement =
                SerializeCollector::findOrMakeChild(
                        doc,
                        *axesList,
                        pointersList,
                        "kdchart:axis",
                        nameOfClass( p ),
                        p,
                        wasFound );
        if( ! wasFound ){
            savePolarAxis( doc, axisElement, *p );
        }
    }
}
*/

bool AxesSerializer::parseCartesianAxis(
        const QDomNode& rootNode,
        const QDomNode& pointerNode,
        CartesianAxis*& axisPtr )const
{
    bool bOK = true;
    axisPtr = 0;

    QObject* ptr;
    QString ptrName;
    bool wasParsed;
    const bool pointerFound =
            AttributesSerializer::parseQObjectPointerNode(
                    pointerNode, ptr,
                    ptrName, wasParsed, true ) && ptr;

    if( ptrName.isEmpty() ){
        qDebug()<< "Could not parse CartesianAxis. Global pointer node is invalid.";
        bOK = false;
    }else{
        if( pointerFound ){
            axisPtr = dynamic_cast<CartesianAxis*>(ptr);
            if( ! axisPtr ){
                qDebug()<< "Could not parse CartesianAxis. Global pointer"
                        << ptrName << "is no CartesianAxis-ptr.";
                bOK = false;
            }
        }else{
            qDebug()<< "Could not parse CartesianAxis. Global pointer"
                    << ptrName << "is no CartesianAxis-ptr.";
            bOK = false;
        }
    }


    if( bOK && wasParsed ) return true;


    QDomElement axisElement;
    if( bOK ){
        axisElement = SerializeCollector::findStoredGlobalElement(
                rootNode, ptrName, "kdchart:axes" );
        bOK = ! axisElement.tagName().isEmpty();
    }

    if( bOK ) {
        SerializeCollector::instance()->setWasParsed( axisPtr, true );

        const QString axisName = axisElement.tagName();
        //qDebug() << "\n    AxesSerializer::parseCartesianAxis() processing" << axisName;
        QDomNode node = axisElement.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "kdchart:abstract-axis" ) {
                    if( ! parseAbstractAxis( element, *axisPtr ) )
                        bOK = false;
                } else if( tagName == "Title" ) {
                    QString s;
                    if( KDXML::readStringNode( element, s ) )
                        axisPtr->setTitleText( s );
                    else
                        bOK = false;
                } else if( tagName == "TitleTextAttributes" ) {
                    TextAttributes ta;
                    if( AttributesSerializer::parseTextAttributes( element, ta ) )
                        axisPtr->setTitleTextAttributes( ta );
                    else
                        bOK = false;
                } else if( tagName == "Position" ) {
                    QString s;
                    if( KDXML::readStringNode( element, s ) ){
                        CartesianAxis::Position pos;
                        if( s.compare("bottom", Qt::CaseInsensitive) == 0 )
                            pos = CartesianAxis::Bottom;
                        else if( s.compare("top", Qt::CaseInsensitive) == 0 )
                            pos = CartesianAxis::Top;
                        else if( s.compare("right", Qt::CaseInsensitive) == 0 )
                            pos = CartesianAxis::Right;
                        else if( s.compare("left", Qt::CaseInsensitive) == 0 )
                            pos = CartesianAxis::Left;
                        else{
                            qDebug()<< "Unknown value of CartesianAxis/Position found:"
                                    << s;
                            bOK = false;
                        }
                        if( bOK )
                            axisPtr->setPosition( pos );
                    }else{
                        qDebug() << "Invalid CartesianAxis/Position element found.";
                        bOK = false;
                    }
                } else {
                    qDebug()<< "Unknown subelement of CartesianAxis found:"
                            << tagName;
                    bOK = false;
                }
            }
            node = node.nextSibling();
        }
    }
    return bOK;
}

void AxesSerializer::saveCartesianAxis(
        QDomDocument& doc,
        QDomElement& axisElement,
        const CartesianAxis& axis )const
{
    // first save the information hold by the base class
    saveAbstractAxis( doc, axisElement, axis,
                      "kdchart:abstract-axis" );

    // save the title
    KDXML::createStringNodeIfContent(
            doc, axisElement,
            "Title", axis.titleText() );
    if( ! axis.hasDefaultTitleTextAttributes() ){
        // save the title text attributes
        AttributesSerializer::saveTextAttributes(
                doc, axisElement,
                axis.titleTextAttributes(),
                "TitleTextAttributes" );
    }
    QString s;
    switch( axis.position() ){
        case CartesianAxis::Bottom:
            s = "bottom";
            break;
        case CartesianAxis::Top:
            s = "top";
            break;
        case CartesianAxis::Right:
            s = "right";
            break;
        case CartesianAxis::Left:
            s = "left";
            break;
        default:
            Q_ASSERT( false ); // all of the positions need to be handled
            break;
    }
    KDXML::createStringNode(
            doc, axisElement,
            "Position", s );
}

//TODO once PolarAxis is implemented:
/*
bool AxesSerializer::parsePolarAxis(
        const QDomNode& rootNode,
        const QDomNode& pointerNode,
        PolarAxis*& axisPtr )const
{
    bool bOK = true;
    // ..
    return bOK;
}

void AxesSerializer::savePolarAxis(
        QDomDocument& doc,
        QDomElement& axisElement,
        const PolarAxis& axis )const
{
    // first save the information hold by the base class
    saveAbstractAxis( doc, axisElement, axis,
                      "kdchart:abstract-axis" );

    // ...
}

*/


bool AxesSerializer::parseAbstractAxis(
        const QDomElement& container,
        AbstractAxis& axis )const
{
    bool bOK = true;
    QDomNode node = container.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "kdchart:abstract-area-base" ) {
                if( ! AbstractAreaBaseSerializer::parseAbstractAreaBase( element, axis ) )
                    bOK = false;
            } else if( tagName == "TextAttributes" ) {
                TextAttributes ta;
                if( AttributesSerializer::parseTextAttributes( element, ta ) )
                    axis.setTextAttributes( ta );
                else
                    bOK = false;
            } else if( tagName == "Labels" ) {
                QStringList list;
                if( KDXML::readStringListNode( element, list ) )
                    axis.setLabels( list );
                else
                    bOK = false;
            } else if( tagName == "ShortLabels" ) {
                QStringList list;
                if( KDXML::readStringListNode( element, list ) )
                    axis.setShortLabels( list );
                else
                    bOK = false;
            } else {
                qDebug() << "Unknown subelement of AbstractAxis found:" << tagName;
                bOK = false;
            }
        }
        node = node.nextSibling();
    }
    return bOK;
}

void AxesSerializer::saveAbstractAxis(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractAxis& axis,
        const QString& title )const
{
    QDomElement axisElement =
            doc.createElement( title );
    e.appendChild( axisElement );

    // save the area information
    AbstractAreaBaseSerializer::saveAbstractAreaBase(
            doc,
            axisElement,
            axis,
            "kdchart:abstract-area-base" );
    // save the text attributes
    AttributesSerializer::saveTextAttributes(
            doc, axisElement,
            axis.textAttributes(),
            "TextAttributes" );
    // save the labels
    QStringList list( axis.labels() );
    if( list.count() )
        KDXML::createStringListNodes(
                doc, axisElement,
                "Labels", &list );
    // save the short labels
    list = axis.shortLabels();
    if( list.count() )
        KDXML::createStringListNodes(
                doc, axisElement,
                "ShortLabels", &list );
}


const QString AxesSerializer::nameOfClass( const AbstractAxis* p )const
{
    QString classname;
    if( dynamic_cast<const CartesianAxis*> (  p ) )
        classname = "KDChart::CartesianAxis";
    /*else if( dynamic_cast<const PolarAxis*> (  p ) )
        classname = "KDChart::PolarAxis";*/
    else
        classname = "UNKNOWN";
    return classname;
}

