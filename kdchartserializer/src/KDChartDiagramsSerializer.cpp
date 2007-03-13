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
    // this space left empty intentionally
}

DiagramsSerializer::~DiagramsSerializer()
{
    // this space left empty intentionally
}

bool DiagramsSerializer::parseDiagrams(
        const QDomElement& e,
        AbstractDiagramList& diags,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

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
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

    QDomElement diagsListElement =
            doc.createElement( title );
    e.appendChild( diagsListElement );
    Q_FOREACH ( const AbstractDiagram* p, diags )
    {
        QDomElement diagElement =
                doc.createElement( "kdchart:diagram" );
        diagsListElement.appendChild( diagElement );

        // first save the information hold by the base class
        saveAbstractDiagram( doc, diagElement, *p,
                             "kdchart:abstract-diagram", styleList );

        // then save any diagram type specific information
        const AbstractCartesianDiagram* cartDiag =
                dynamic_cast<const AbstractCartesianDiagram*> ( p );
        if( cartDiag ){
            saveCartDiagram( doc, diagElement, *cartDiag,
                             "kdchart:cartesian-diagram", styleList );
        }else{
            const AbstractPolarDiagram* polDiag =
                    dynamic_cast<const AbstractPolarDiagram*> ( p );
            if( polDiag ){
                savePolDiagram( doc, diagElement, *polDiag,
                                "kdchart:polar-diagram", styleList );
            }else{
                saveOtherDiagram( doc, diagElement, *p, styleList );
            }
        }
    }
}

void DiagramsSerializer::saveAbstractDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractDiagram& diagram,
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

    QDomElement diagElement =
        doc.createElement( title );
    e.appendChild( diagElement );

    // ...
}

void DiagramsSerializer::saveCartDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractCartesianDiagram& diagram,
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

    QDomElement diagElement =
        doc.createElement( title );
    e.appendChild( diagElement );

    // ...
}

void DiagramsSerializer::savePolDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractPolarDiagram& diagram,
        const QString& title,
        const QDomElement* styleList )const
{
    Q_UNUSED(styleList)

}

void DiagramsSerializer::saveOtherDiagram(
        QDomDocument& doc,
        QDomElement& e,
        const AbstractDiagram& diagram,
        const QDomElement* styleList )const
{
    Q_UNUSED(doc)
    Q_UNUSED(e)
    Q_UNUSED(diagram)
    Q_UNUSED(styleList)
}
