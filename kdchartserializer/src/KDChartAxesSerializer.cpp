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
        CartesianAxisList& axes,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

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
        PolarAxisList& axes,
        const QDomElement* styleList=0 )const
{
    // ...
    return true;
}
*/

void AxesSerializer::saveCartesianAxes(
        QDomDocument& doc,
        QDomElement& e,
        const CartesianAxisList& axes,
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

    QDomElement axesListElement =
            doc.createElement( title );
    e.appendChild( axesListElement );
    Q_FOREACH ( const CartesianAxis* p, axes )
    {
        QDomElement axisElement =
                doc.createElement( "kdchart:axis" );
        axesListElement.appendChild( axisElement );

        // first save the information hold by the base class
        saveAbstractAxis( doc, axisElement, *p,
                          "kdchart:abstract-axis", styleList );

        // then save any diagram type specific information
        saveCartAxis( doc, axisElement, *p,
                      "kdchart:cartesian-axis", styleList );
    }
}

//TODO once PolarAxis is implemented:
/*
void AxesSerializer::savePolarAxes(
        QDomDocument& doc,
        QDomElement& e,
        const PolarAxisList& axes,
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

    QDomElement axesListElement =
            doc.createElement( title );
    e.appendChild( axesListElement );
    Q_FOREACH ( const PolarAxis* p, axes )
    {
        QDomElement axisElement =
                doc.createElement( "kdchart:axis" );
        axesListElement.appendChild( axisElement );

        // first save the information hold by the base class
        saveAbstractAxis( doc, axisElement, *p,
                          "kdchart:abstract-axis", styleList );

        // then save any diagram type specific information
        savePolAxis( doc, axisElement, *p,
                     "kdchart:polar-axis", styleList );
    }
}
*/

void AxesSerializer::saveAbstractAxis(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractAxis& axis,
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

    QDomElement axisElement =
        doc.createElement( title );
    e.appendChild( axisElement );

    // ...
}

void AxesSerializer::saveCartAxis(
        QDomDocument& doc,
        QDomElement& e,
        const CartesianAxis& axis,
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

    QDomElement axisElement =
        doc.createElement( title );
    e.appendChild( axisElement );

    // ...
}

//TODO once PolarAxis is implemented:
/*
void AxesSerializer::savePolPlane(
        QDomDocument& doc,
        QDomElement& e,
        const PolarAxis& axis,
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

            QDomElement axisElement =
            doc.createElement( title );
    e.appendChild( axisElement );

    // ...
}
*/