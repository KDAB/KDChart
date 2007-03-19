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

#include "KDChartTextAreaSerializer.h"

#include "KDChartAttributesSerializer.h"
#include "KDChartAbstractAreaBaseSerializer.h"
#include "KDChartSerializeCollector.h"

#include "KDXMLTools.h"

#include <qglobal.h>


/**
  \class KDChart::TextAreaSerializer KDChartTextAreaSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


void TextAreaSerializer::saveHeadersFooters(
        QDomDocument& doc,
        QDomElement& e,
        const KDChart::HeaderFooterList& areas,
        const QString& title )
{
    // access (or append, resp.) the global list
    QDomElement* hdFtList =
            SerializeCollector::instance()->findOrMakeElement( doc, title );

    // create the local list holding names pointing into the global list
    QDomElement pointersList =
            SerializeCollector::createPointersList( doc, e, title );

    Q_FOREACH ( const HeaderFooter* p, areas )
    {
        bool wasFound;
        QDomElement hdFtElement =
                SerializeCollector::findOrMakeChild(
                doc,
                *hdFtList,
                pointersList,
                "kdchart:header-footer",
                p,
                wasFound );
        if( ! wasFound ){
            QString name;
            switch( p->type() ){
                case HeaderFooter::Header:
                    name = "Header";
                    break;
                case HeaderFooter::Footer:
                    name = "Footer";
                    break;
                default:
                    Q_ASSERT( false ); // all of the types need to be handled
                    break;
            }

        // first save the information hold by the base class
            saveTextArea( doc, hdFtElement, *p,
                          "kdchart:text-area" );

        // then save what is stored in the derived class
            hdFtElement.setAttribute( "Type", name );
            KDXML::createStringNode( doc, hdFtElement, "Position", p->position().name() );
        }
    }
}

void TextAreaSerializer::saveTextArea(
        QDomDocument& doc,
        QDomElement& e,
        const TextArea& area,
        const QString& title )
{
    QDomElement element = doc.createElement( title );
    e.appendChild( element );

    // first save the information hold by both of the base classes
    AbstractAreaBaseSerializer::saveAbstractAreaBase(
            doc,
            element,
            area,
            "kdchart:abstract-area-base" );
    saveTextLayoutItem( doc, element, area, "kdchart:text-layout-item" );
    // that's all, there is no to-be-saved information in this class
}


void TextAreaSerializer::saveTextLayoutItem(
        QDomDocument& doc,
        QDomElement& e,
        const TextLayoutItem& item,
        const QString& title )
{
    QDomElement element = doc.createElement( title );
    e.appendChild( element );

    KDXML::createStringNode( doc, element, "Text", item.text() );
    AttributesSerializer::saveTextAttributes(
            doc, element, item.textAttributes(), "TextAttributes" );
}
