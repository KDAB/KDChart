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

#include "KDChartLegendsSerializer.h"
#include "KDChartDiagramsSerializer.h"
#include "KDChartSerializeCollector.h"
#include "KDChartIdMapper.h"

#include "KDXMLTools.h"

#include <qglobal.h>


/**
  \class KDChart::LegendsSerializer KDChartLegendsSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


void LegendsSerializer::saveLegends(
        QDomDocument& doc,
        QDomElement& e,
        const LegendList& legends,
        const QString& title )
{
    // access (or append, resp.) the global list
    QDomElement* legendsList =
            SerializeCollector::instance()->findOrMakeElement( doc, title );

    // create the local list holding names pointing into the global list
    QDomElement pointersList =
            SerializeCollector::createPointersList( doc, e, title );

    Q_FOREACH ( Legend* p, legends )
    {
        bool wasFound;
        QDomElement legendElement =
                SerializeCollector::findOrMakeChild(
                doc,
                *legendsList,
                pointersList,
                "kdchart:legend",
                p,
                wasFound );
        if( ! wasFound ){
            saveLegend( doc, legendElement, *p, "kdchart:legend" );
        }
    }
}

void LegendsSerializer::saveLegend(
        QDomDocument& doc,
        QDomElement& e,
        const Legend& legend,
        const QString& title )
{
    QDomElement element = doc.createElement( title );
    e.appendChild( element );

    KDXML::createBoolNode( doc, element, "Visible", legend.isVisible() );

    // save the associated diagrams
    DiagramsSerializer diagS( 0 );
    diagS.saveDiagrams( doc,
                        element,
                        legend.constDiagrams(),
                        "kdchart:diagrams" );
    KDXML::createAlignmentNode( doc, element, "Alignment", legend.alignment() );
    KDXML::createStringNode( doc, element, "Position", legend.position().name() );
    if( legend.position() == Position::Floating )
        AttributesSerializer::saveRelativePosition(
                doc, element, legend.floatingPosition(), "FloatingPosition" );
    KDXML::createOrientationNode(
            doc, element, "Orientation", legend.orientation() );
    KDXML::createBoolNode(
            doc, element, "ShowLines", legend.showLines() );
    // save the texts map
    {
        const QMap<uint,QString> map( legend.texts() );
        if( map.count() ){
            QDomElement mapElement =
                    doc.createElement( "TextsMap" );
            element.appendChild( mapElement );
            QMap<uint,QString>::const_iterator i = map.constBegin();
            while (i != map.constEnd()) {
                QDomElement textElement =
                        doc.createElement( "item" );
                mapElement.appendChild( textElement );
                textElement.setAttribute( "dataset", i.key() );
                textElement.setAttribute( "text",    i.value() );
                ++i;
            }
        }
    }
    // save the brushes map
    {
        const QMap<uint,QBrush> map( legend.brushes() );
        if( map.count() ){
            QDomElement mapElement =
                    doc.createElement( "BrushesMap" );
            element.appendChild( mapElement );
            QMap<uint,QBrush>::const_iterator i = map.constBegin();
            while (i != map.constEnd()) {
                QDomElement brushElement =
                        doc.createElement( "item" );
                mapElement.appendChild( brushElement );
                brushElement.setAttribute( "dataset", i.key() );
                KDXML::createBrushNode( doc, brushElement, "brush", i.value() );
                ++i;
            }
        }
    }
    // save the pens map
    {
        const QMap<uint,QPen> map( legend.pens() );
        if( map.count() ){
            QDomElement mapElement =
                    doc.createElement( "PensMap" );
            element.appendChild( mapElement );
            QMap<uint,QPen>::const_iterator i = map.constBegin();
            while (i != map.constEnd()) {
                QDomElement penElement =
                        doc.createElement( "item" );
                mapElement.appendChild( penElement );
                penElement.setAttribute( "dataset", i.key() );
                KDXML::createPenNode( doc, penElement, "pen", i.value() );
                ++i;
            }
        }
    }
    // save the marker attributes map
    {
        const QMap<uint,MarkerAttributes> map( legend.markerAttributes() );
        if( map.count() ){
            QDomElement mapElement =
                    doc.createElement( "MarkerAttributesMap" );
            element.appendChild( mapElement );
            QMap<uint,MarkerAttributes>::const_iterator i = map.constBegin();
            while (i != map.constEnd()) {
                QDomElement attrsElement =
                        doc.createElement( "item" );
                mapElement.appendChild( attrsElement );
                attrsElement.setAttribute( "dataset", i.key() );
                AttributesSerializer::saveMarkerAttributes(
                        doc, attrsElement, i.value(), "MarkerAttributes" );
                ++i;
            }
        }
    }
    KDXML::createBoolNode(
            doc, element, "UseAutomaticMarkerSize", legend.useAutomaticMarkerSize() );
    AttributesSerializer::saveTextAttributes(
            doc, element, legend.textAttributes(), "TextAttributes" );
    KDXML::createStringNode(
            doc, element, "TitleText", legend.titleText() );
    KDXML::createIntNode(
            doc, element, "Spacing", legend.spacing() );
}
