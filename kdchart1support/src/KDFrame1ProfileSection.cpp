/* -*- Mode: C++ -*-
   KDChart1 - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart1 library.
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
#include <KDFrame1ProfileSection.h>
//#include <KDXMLTools1.h>

KDFrame1ProfileSection::~KDFrame1ProfileSection()
{
    // Intentionally left blank for now.
}


void KDFrame1ProfileSection::createFrameProfileSectionNode( QDomDocument& document,
        QDomNode& parent,
        const QString& elementName,
        const KDFrame1ProfileSection* section )

{
    return;

    /*
    QDomElement sectionElement = document.createElement( elementName );
    parent.appendChild( sectionElement );
    KDXML::createStringNode( document, sectionElement, "Direction",
            KDFrame1ProfileSection::directionToString( section->_direction ) );
    KDXML::createStringNode( document, sectionElement, "Curvature",
            KDFrame1ProfileSection::curvatureToString( section->_curvature ) );
    KDXML::createIntNode( document, sectionElement, "Width", section->_width );
    KDXML::createPenNode( document, sectionElement, "Style", section->_pen );
    */
}


bool KDFrame1ProfileSection::readFrameProfileSectionNode( const QDomElement& element,
        KDFrame1ProfileSection* section )
{
    return false;

    /*
    bool ok = true;
    Direction tempDirection = DirPlain;
    Curvature tempCurvature = CvtPlain;
    int tempWidth;
    QPen tempPen;
    QDomNode node = element.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Direction" ) {
                QString value;
                ok = ok & KDXML::readStringNode( element, value );
                tempDirection = stringToDirection( value );
            } else if( tagName == "Curvature" ) {
                QString value;
                ok = ok & KDXML::readStringNode( element, value );
                tempCurvature = stringToCurvature( value );
            } else if( tagName == "Width" ) {
                ok = ok & KDXML::readIntNode( element, tempWidth );
            } else if( tagName == "Pen" ) {
                ok = ok & KDXML::readPenNode( element, tempPen );
            } else {
                qDebug( "Unknown tag in frame" );
            }
        }
        node = node.nextSibling();
    }

    if( ok ) {
        section->_direction = tempDirection;
        section->_curvature = tempCurvature;
        section->_width = tempWidth;
        section->_pen = tempPen;
    }

    return ok;
    */
}
