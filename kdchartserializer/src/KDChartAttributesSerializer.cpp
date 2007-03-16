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

#include "KDChartAttributesSerializer.h"

#include "KDXMLTools.h"

#include <qglobal.h>
#include <QMessageBox>


/**
  \class KDChart::AttributesSerializer KDChartAttributesSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


AttributesSerializer::AttributesSerializer()
{
    // this space left empty intentionally
}

AttributesSerializer::~AttributesSerializer()
{
    // this space left empty intentionally
}

bool AttributesSerializer::parseLeading(
        const QDomElement& e, int& left, int& top, int& right, int& bottom )const
{
    return  KDXML::findIntAttribute( e, "left",   left   ) ||
            KDXML::findIntAttribute( e, "top",    top    ) ||
            KDXML::findIntAttribute( e, "right",  right  ) ||
            KDXML::findIntAttribute( e, "bottom", bottom );
}

void AttributesSerializer::saveLeading(
        QDomDocument& doc,
        QDomElement& e,
        int left, int top, int right, int bottom,
        const QString& title )const
{
    QDomElement leadingElement =
        doc.createElement( title );
    e.appendChild( leadingElement );
    leadingElement.setAttribute( "left",   left   );
    leadingElement.setAttribute( "top",    top    );
    leadingElement.setAttribute( "right",  right  );
    leadingElement.setAttribute( "bottom", bottom );
}


bool AttributesSerializer::parseFrameAttributes(
        const QDomElement& e,
        FrameAttributes& a )const
{
    bool bOK = true;
    QDomNode node = e.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Visible" ) {
                bool b;
                if( KDXML::readBoolNode( element, b ) )
                    a.setVisible( b );
            } else if( tagName == "Pen" ) {
                QPen p;
                if( KDXML::readPenNode( element, p ) )
                    a.setPen( p );
            } else if( tagName == "Padding" ) {
                int i;
                if( KDXML::readIntNode( element, i ) )
                    a.setPadding( i );
            } else {
                qDebug() << "Unknown subelement of FrameAttributes found:" << tagName;
                bOK = false;
            }
        }
        node = node.nextSibling();
    }
    return bOK;
}

void AttributesSerializer::saveFrameAttributes(
        QDomDocument& doc,
        QDomElement& e,
        const FrameAttributes& a,
        const QString& title )const
{
    QDomElement frameAttributesElement =
            doc.createElement( title );
    e.appendChild( frameAttributesElement );
    KDXML::createBoolNode( doc, frameAttributesElement, "Visible",
                           a.isVisible() );
    KDXML::createPenNode( doc, frameAttributesElement, "Pen",
                          a.pen() );
    KDXML::createIntNode( doc, frameAttributesElement, "Padding",
                          a.padding() );
}


bool AttributesSerializer::parseBackgroundAttributes(
        const QDomElement& e,
        BackgroundAttributes& a )const
{
    bool bOK = true;
    QDomNode node = e.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Visible" ) {
                bool b;
                if( KDXML::readBoolNode( element, b ) )
                    a.setVisible( b );
            } else if( tagName == "Brush" ) {
                QBrush b;
                if( KDXML::readBrushNode( element, b ) )
                    a.setBrush( b );
            } else if( tagName == "PixmapMode" ) {
                QString s;
                if( KDXML::readStringNode( element, s ) ){
                    if( s == "BackgroundPixmapModeNone" )
                        a.setPixmapMode( BackgroundAttributes::BackgroundPixmapModeNone );
                    else if ( s == "BackgroundPixmapModeCentered" )
                        a.setPixmapMode( BackgroundAttributes::BackgroundPixmapModeCentered );
                    else if ( s == "BackgroundPixmapModeScaled" )
                        a.setPixmapMode( BackgroundAttributes::BackgroundPixmapModeScaled );
                    else if ( s == "BackgroundPixmapModeStretched" )
                        a.setPixmapMode( BackgroundAttributes::BackgroundPixmapModeStretched );
                    else{
                        qDebug() << "Unknown PixmapMode found in BackgroundAttributes:" << tagName;
                        bOK = false;
                    }
                }
            } else if( tagName == "Pixmap" ) {
                QPixmap p;
                if( KDXML::readPixmapNode( element, p ) )
                    a.setPixmap( p );
            } else {
                qDebug() << "Unknown subelement of BackgroundAttributes found:" << tagName;
                bOK = false;
            }
        }
        node = node.nextSibling();
    }
    return bOK;
}

void AttributesSerializer::saveBackgroundAttributes(
        QDomDocument& doc,
        QDomElement& e,
        const BackgroundAttributes& a,
        const QString& title )const
{
    QDomElement backgroundAttributesElement =
            doc.createElement( title );
    e.appendChild( backgroundAttributesElement );
    KDXML::createBoolNode( doc, backgroundAttributesElement, "Visible",
                           a.isVisible() );
    KDXML::createBrushNode( doc, backgroundAttributesElement, "Brush",
                            a.brush() );
    BackgroundAttributes::BackgroundPixmapMode mode = a.pixmapMode();
    bool hasPixmap = false;
    QString s;
    switch( mode ){
        case BackgroundAttributes::BackgroundPixmapModeNone:
            s = "BackgroundPixmapModeNone";
            break;
        case BackgroundAttributes::BackgroundPixmapModeCentered:
            hasPixmap = true;
            s = "BackgroundPixmapModeCentered";
            break;
        case BackgroundAttributes::BackgroundPixmapModeScaled:
            hasPixmap = true;
            s = "BackgroundPixmapModeScaled";
            break;
        case BackgroundAttributes::BackgroundPixmapModeStretched:
            hasPixmap = true;
            s = "BackgroundPixmapModeStretched";
            break;
        default:
            s = "BackgroundPixmapModeNone";
            break;
    }
    KDXML::createStringNode( doc, backgroundAttributesElement, "PixmapMode",
                             s );
    if( hasPixmap )
        KDXML::createPixmapNode( doc, backgroundAttributesElement, "Padding",
                                 a.pixmap() );
}

