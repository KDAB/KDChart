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


bool TextAreaSerializer::parseHeadersFooters(
        const QDomElement& container,
        KDChart::HeaderFooterList& areas )
{
    bool bOK = true;
    return bOK;
}

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
                "KDChart::HeaderFooter",
                p,
                wasFound );
        if( ! wasFound )
            saveHeaderFooter( doc, hdFtElement, *p );
    }
}


bool TextAreaSerializer::parseHeaderFooter(
        const QDomElement& container,
        HeaderFooter& hdFt )
{
    bool bOK = true;
    QString s;
    if( KDXML::findStringAttribute( container, "type", s ) ){
        if( ! s.isEmpty() ){
            if( s.compare("Header", Qt::CaseInsensitive) == 0 ){
                hdFt.setType( HeaderFooter::Header );
            }else if( s.compare("Footer", Qt::CaseInsensitive) == 0 ){
                hdFt.setType( HeaderFooter::Footer );
            }else{
                qDebug() << "Empty type attribute found in HeaderFooter.";
                bOK = false;
            }
        } else {
            qDebug() << "Invalid type attribute found in HeaderFooter: \"" << s << "\"";
            bOK = false;
        }
    } else {
        qDebug() << "No type attribute found in HeaderFooter element.";
        bOK = false;
    }
    if( bOK ){
        QDomNode node = container.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "Position" ) {
                    QString s;
                    if( KDXML::readStringNode( element, s ) )
                        hdFt.setPosition( Position::fromName( s.toLatin1() ) );
                    else
                        bOK = false;
                } else if( tagName == "kdchart:text-area" ) {
                    // parse the base class:
                    if( ! parseTextArea( element, hdFt ) )
                        bOK = false;
                } else {
                    qDebug() << "Unknown subelement of HeaderFooter found:" << tagName;
                    bOK = false;
                }
            }
            node = node.nextSibling();
        }
    }
    return bOK;
}

void TextAreaSerializer::saveHeaderFooter(
        QDomDocument& doc,
        QDomElement& hdFtElement,
        const KDChart::HeaderFooter& hdFt )
{
    // first save the information hold by the base class
    QString type;
    switch( hdFt.type() ){
        case HeaderFooter::Header:
            type = "Header";
            break;
        case HeaderFooter::Footer:
            type = "Footer";
            break;
        default:
            Q_ASSERT( false ); // all of the types need to be handled
            break;
    }
    hdFtElement.setAttribute( "type", type );
    saveTextArea( doc, hdFtElement, hdFt,
                    "kdchart:text-area" );

    // then save what is stored in the derived class
    KDXML::createStringNode( doc, hdFtElement, "Position", hdFt.position().name() );
}


bool TextAreaSerializer::parseTextArea(
        const QDomElement& container,
        TextArea& area )
{
    bool bOK = true;
    QDomNode node = container.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "kdchart:abstract-area-base" ) {
                if( ! AbstractAreaBaseSerializer::parseAbstractAreaBase( element, area ) )
                    bOK = false;
            } else if( tagName == "kdchart:text-layout-item" ) {
                if( ! parseTextLayoutItem( element, area ) )
                    bOK = false;
            } else {
                qDebug() << "Unknown subelement of TextArea found:" << tagName;
                bOK = false;
            }
        }
        node = node.nextSibling();
    }
    return bOK;
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


bool TextAreaSerializer::parseTextLayoutItem(
        const QDomElement& container,
        TextLayoutItem& item )
{
    bool bOK = true;
    QDomNode node = container.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "AutoReferenceArea" ) {
                QObject* ptr;
                if( AttributesSerializer::parseQObjectPointerNode( element.firstChild(), ptr ) )
                    item.setAutoReferenceArea( ptr );
                else
                    bOK = false;
            } else if( tagName == "Text" ) {
                QString s;
                if( KDXML::readStringNode( element, s ) )
                    item.setText( s );
                else
                    bOK = false;
            } else if( tagName == "TextAttributes" ) {
                TextAttributes ta;
                if( AttributesSerializer::parseTextAttributes( element, ta ) )
                    item.setTextAttributes( ta );
                else
                    bOK = false;
            } else {
                qDebug() << "Unknown subelement of TextLayoutItem found:" << tagName;
                bOK = false;
            }
        }
        node = node.nextSibling();
    }
    return bOK;
}

void TextAreaSerializer::saveTextLayoutItem(
        QDomDocument& doc,
        QDomElement& e,
        const TextLayoutItem& item,
        const QString& title )
{
    QDomElement element = doc.createElement( title );
    e.appendChild( element );

    AttributesSerializer::saveQObjectPointer(
            doc, element, item.autoReferenceArea(), "AutoReferenceArea" );
    KDXML::createStringNode( doc, element, "Text", item.text() );
    AttributesSerializer::saveTextAttributes(
            doc, element, item.textAttributes(), "TextAttributes" );
}
