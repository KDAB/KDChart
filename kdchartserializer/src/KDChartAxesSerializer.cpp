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

bool AxesSerializer::parseCartesianAxes(
        const QDomElement& e,
        CartesianAxisList& axes )const
{
    bool bOK = true;
/*
    QDomNode node = e.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();

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

        }
        node = node.nextSibling();
    }
*/
    return bOK;
}

//TODO once PolarAxis is implemented:
/*
bool AxesSerializer::parsePolarAxes(
        const QDomElement& e,
        PolarAxisList& axes )const
{
    // ...
    return true;
}
*/

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
                        p,
                        wasFound );
        if( ! wasFound ){
            // first save the information hold by the base class
            saveAbstractAxis( doc, axisElement, *p,
                            "kdchart:abstract-axis" );

            // then save any diagram type specific information
            saveCartAxis( doc, axisElement, *p,
                        "kdchart:cartesian-axis" );
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
        QDomElement axisElement =
                doc.createElement( "kdchart:axis" );
        axesList.appendChild( axisElement );

        // first save the information hold by the base class
        saveAbstractAxis( doc, axisElement, *p,
                          "kdchart:abstract-axis" );

        // then save any diagram type specific information
        savePolAxis( doc, axisElement, *p,
                     "kdchart:polar-axis" );
    }
}
*/

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
    KDXML::createStringListNodes(
            doc, axisElement,
            "Labels", &list );
    // save the short labels
    list = axis.shortLabels();
    KDXML::createStringListNodes(
            doc, axisElement,
            "ShortLabels", &list );
}

void AxesSerializer::saveCartAxis(
        QDomDocument& doc,
        QDomElement& e,
        const CartesianAxis& axis,
        const QString& title )const
{
    QDomElement axisElement =
        doc.createElement( title );
    e.appendChild( axisElement );
    // save the title
    KDXML::createStringNode(
            doc, axisElement,
            "Title", axis.titleText() );
    // save the title text attributes
    AttributesSerializer::saveTextAttributes(
            doc, axisElement,
            axis.titleTextAttributes(),
            "TitleTextAttributes" );
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
    }
    KDXML::createStringNode(
            doc, axisElement,
            "Position", s );
}

//TODO once PolarAxis is implemented:
/*
void AxesSerializer::savePolPlane(
        QDomDocument& doc,
        QDomElement& e,
        const PolarAxis& axis,
        const QString& title )const
{
    QDomElement axisElement =
            doc.createElement( title );
    e.appendChild( axisElement );

    // ...
}
*/
