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

#include "KDChartPosition.h"

#include "KDXMLTools.h"

#include <qglobal.h>
#include <QMessageBox>


/**
  \class KDChart::AttributesSerializer KDChartAttributesSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


bool AttributesSerializer::parseLeading(
        const QDomElement& e, int& left, int& top, int& right, int& bottom )
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
        const QString& title )
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
        FrameAttributes& a )
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
        const QString& title )
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


bool AttributesSerializer::parseTextAttributes(
        const QDomElement& e,
        TextAttributes& a )
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
            } else if( tagName == "FontSize" ) {
                Measure m;
                if( parseMeasure( element, m ) )
                    a.setFontSize( m );
            } else if( tagName == "MinimalFontSize" ) {
                Measure m;
                if( parseMeasure( element, m ) )
                    a.setMinimalFontSize( m );
            } else if( tagName == "AutoRotate" ) {
                bool b;
                if( KDXML::readBoolNode( element, b ) )
                    a.setAutoRotate( b );
            } else if( tagName == "AutoShrink" ) {
                bool b;
                if( KDXML::readBoolNode( element, b ) )
                    a.setAutoShrink( b );
            } else if( tagName == "Rotation" ) {
                int i;
                if( KDXML::readIntNode( element, i ) )
                    a.setRotation( i );
            } else {
                qDebug() << "Unknown subelement of TextAttributes found:" << tagName;
                bOK = false;
            }
        }
        node = node.nextSibling();
    }
    return bOK;
}

void AttributesSerializer::saveTextAttributes(
        QDomDocument& doc,
        QDomElement& e,
        const TextAttributes& a,
        const QString& title )
{
    QDomElement textAttributesElement =
            doc.createElement( title );
    e.appendChild( textAttributesElement );
    KDXML::createBoolNode( doc, textAttributesElement, "Visible",
                           a.isVisible() );
    KDXML::createPenNode( doc, textAttributesElement, "Pen",
                          a.pen() );
    KDXML::createFontNode( doc, textAttributesElement, "Font",
                           a.font() );
    saveMeasure( doc, textAttributesElement,
                 a.fontSize(), "FontSize" );
    saveMeasure( doc, textAttributesElement,
                 a.minimalFontSize(), "MinimalFontSize" );
    KDXML::createBoolNode( doc, textAttributesElement, "AutoRotate",
                           a.autoRotate() );
    KDXML::createBoolNode( doc, textAttributesElement, "AutoShrink",
                           a.autoShrink() );
    KDXML::createIntNode( doc, textAttributesElement, "Rotation",
                          a.rotation() );
}


bool AttributesSerializer::parseMeasure(
        const QDomElement& e,
        Measure& a )
{
    bool bOK = true;
    QDomNode node = e.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Value" ) {
                qreal r;
                if( KDXML::readRealNode( element, r ) )
                    a.setValue( r );
            } else if( tagName == "Mode" ) {
                QString s;
                if( KDXML::readStringNode( element, s ) )
                    a.setCalculationMode(
                            KDChartEnums::stringToMeasureCalculationMode( s ) );
            } else if( tagName == "Orientation" ) {
                QString s;
                if( KDXML::readStringNode( element, s ) )
                    a.setReferenceOrientation(
                            KDChartEnums::stringToMeasureOrientation( s ) );
            } else {
                qDebug() << "Unknown subelement of Measure found:" << tagName;
                bOK = false;
            }
        }
        node = node.nextSibling();
    }
    return bOK;
}

void AttributesSerializer::saveMeasure(
        QDomDocument& doc,
        QDomElement& e,
        const Measure& m,
        const QString& title )
{
    QDomElement measureElement =
            doc.createElement( title );
    e.appendChild( measureElement );
    KDXML::createRealNode( doc, measureElement, "Value",
                           m.value() );
    KDXML::createStringNode( doc, measureElement, "Mode",
                             KDChartEnums::measureCalculationModeToString(
                                     m.calculationMode() ) );
    KDXML::createStringNode( doc, measureElement, "Orientation",
                             KDChartEnums::measureOrientationToString(
                                     m.referenceOrientation() ) );
}

bool AttributesSerializer::parseBackgroundAttributes(
        const QDomElement& e,
        BackgroundAttributes& a )
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
        const QString& title )
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

void AttributesSerializer::saveDataValueAttributes(
        QDomDocument& doc,
        QDomElement& e,
        const DataValueAttributes& a,
        const QString& title )
{
    QDomElement element = doc.createElement( title );
    e.appendChild( element );
    KDXML::createBoolNode( doc, element, "Visible",
                           a.isVisible() );
    // save the text attributes
    saveTextAttributes(
            doc, element,
            a.textAttributes(),
            "TextAttributes" );
    // save the frame attributes
    saveFrameAttributes(
            doc, element,
            a.frameAttributes(),
            "FrameAttributes" );
    // save the background attributes
    saveBackgroundAttributes(
            doc, element,
            a.backgroundAttributes(),
            "BackgroundAttributes" );
    // save the marker attributes
    saveMarkerAttributes(
            doc, element,
            a.markerAttributes(),
            "MarkerAttributes" );
    // save the number of decimal digits
    KDXML::createIntNode( doc, element, "DecimalDigits", a.decimalDigits() );
    // save the prefix string
    KDXML::createStringNodeIfContent( doc, element, "Prefix", a.prefix() );
    // save the suffix string
    KDXML::createStringNodeIfContent( doc, element, "Suffix", a.suffix() );
    // save the data label
    KDXML::createStringNodeIfContent( doc, element, "DataLabel", a.dataLabel() );
    // save the showRepetitiveDataLabels flag
    KDXML::createBoolNode( doc, element, "ShowRepetitiveDataLabels", a.showRepetitiveDataLabels() );
    // save the power-of-ten divisor
    KDXML::createIntNode( doc, element, "PowerOfTenDivisor", a.powerOfTenDivisor() );
    // save the showInfinite flag
    KDXML::createBoolNode( doc, element, "ShowInfinite", a.showInfinite() );
    // save the negative positioning attributes
    saveRelativePosition(
            doc, element,
            a.negativePosition(),
            "NegativePosition" );
    // save the positive positioning attributes
    saveRelativePosition(
            doc, element,
            a.positivePosition(),
            "PositivePosition" );
}

QString AttributesSerializer::markerStyleToName( MarkerAttributes::MarkerStyle style )
{
    QString name;
    switch( style ){
        case MarkerAttributes::MarkerCircle:
            name = "MarkerCircle";
            break;
        case MarkerAttributes::MarkerSquare:
            name = "MarkerSquare";
            break;
        case MarkerAttributes::MarkerDiamond:
            name = "MarkerDiamond";
            break;
        case MarkerAttributes::Marker1Pixel:
            name = "Marker1Pixel";
            break;
        case MarkerAttributes::Marker4Pixels:
            name = "Marker4Pixels";
            break;
        case MarkerAttributes::MarkerRing:
            name = "MarkerRing";
            break;
        case MarkerAttributes::MarkerCross:
            name = "MarkerCross";
            break;
        case MarkerAttributes::MarkerFastCross:
            name = "MarkerFastCross";
            break;
        default:
            Q_ASSERT( false ); // all of the style types need to be handled
            break;
    }
    return name;
}

void AttributesSerializer::saveMarkerAttributes(
        QDomDocument& doc,
        QDomElement& e,
        const MarkerAttributes& a,
        const QString& title )
{
    QDomElement element = doc.createElement( title );
    e.appendChild( element );
    KDXML::createBoolNode( doc, element, "Visible",
                           a.isVisible() );
    // save the style
    QDomElement styleElement =
            doc.createElement( "MarkerStyle" );
    element.appendChild( styleElement );
    styleElement.setAttribute( "style", markerStyleToName( a.markerStyle() ) );
    // save the stylesMap
    {
        QDomElement mapElement =
                doc.createElement( "StylesMap" );
        element.appendChild( mapElement );
        MarkerAttributes::MarkerStylesMap map( a.markerStylesMap() );
        MarkerAttributes::MarkerStylesMap::const_iterator i = map.constBegin();
        while (i != map.constEnd()) {
            QDomElement styleElement =
                    doc.createElement( "MarkerStyle" );
            mapElement.appendChild( styleElement );
            styleElement.setAttribute( "key", i.key() );
            styleElement.setAttribute( "style", markerStyleToName( i.value() ) );
            ++i;
        }
    }
    // save the size
    KDXML::createSizeFNode( doc, element, "Size", a.markerSize() );
    // save the color
    KDXML::createColorNode( doc, element, "Color", a.markerColor() );
    // save the pen
    KDXML::createPenNode( doc, element, "Pen", a.pen() );
}

void AttributesSerializer::saveRelativePosition(
        QDomDocument& doc,
        QDomElement& e,
        const RelativePosition& a,
        const QString& title )
{
    QDomElement element = doc.createElement( title );
    e.appendChild( element );
    // save the reference points
    const PositionPoints points = a.referencePoints();
    if( ! points.isNull() ){
        QDomElement pointsElement =
                doc.createElement( "PositionPoints" );
        element.appendChild( pointsElement );
        // save the positions
        KDXML::createPointFNode(
                doc, element, "PositionUnknown", points.point( Position::Unknown ) );
        KDXML::createPointFNode(
                doc, element, "Center",    points.point( Position::Center ) );
        KDXML::createPointFNode(
                doc, element, "NorthWest", points.point( Position::NorthWest ) );
        KDXML::createPointFNode(
                doc, element, "North",     points.point( Position::North ) );
        KDXML::createPointFNode(
                doc, element, "NorthEast", points.point( Position::NorthEast ) );
        KDXML::createPointFNode(
                doc, element, "East",      points.point( Position::East ) );
        KDXML::createPointFNode(
                doc, element, "SouthEast", points.point( Position::SouthEast ) );
        KDXML::createPointFNode(
                doc, element, "South",     points.point( Position::South ) );
        KDXML::createPointFNode(
                doc, element, "SouthWest", points.point( Position::SouthWest ) );
        KDXML::createPointFNode(
                doc, element, "West",      points.point( Position::West ) );
    }
    // save the reference position
    KDXML::createStringNode( doc, element, "ReferencePosition", a.referencePosition().name() );
    // save the alignment
    KDXML::createAlignmentNode(
            doc, element, "Alignment", a.alignment() );
    // save padding
    saveMeasure( doc, element, a.horizontalPadding(), "HorizontalPadding" );
    saveMeasure( doc, element, a.verticalPadding(),   "VerticalPadding" );
    // save the rotation
    KDXML::createRealNode( doc, element, "Rotation", a.rotation() );
}
