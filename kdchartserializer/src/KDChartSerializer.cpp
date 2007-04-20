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

#include "KDChartSerializer.h"
#include "KDChartSerializeCollector.h"
#include "KDChartIdMapper.h"
#include "KDChartAttributesSerializer.h"
#include "KDChartCoordPlanesSerializer.h"
#include "KDChartTextAreaSerializer.h"
#include "KDChartLegendsSerializer.h"

#include "KDXMLTools.h"

#include <qglobal.h>
#include <QMessageBox>


/**
  \class KDChart::Serializer KDChartSerializer.h

  \brief Allows reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


Serializer::Serializer( Chart* chart )
    : mChart( chart )
{
    mProgramName = tr("KD Chart Serializer");
    mCoordS = new CoordPlanesSerializer();
    // instantiate (or re-set, resp.) the singletons:
    IdMapper::instance()->clear();
    SerializeCollector::instance()->clear();
}

Serializer::~Serializer()
{
    delete mCoordS;
    // delete the singletons:
    //delete IdMapper::instance();
    //delete SerializeCollector::instance();
}


bool Serializer::read(QIODevice *device)
{
    if( ! mChart ){
        QMessageBox::information(0 , mProgramName,
                                 tr("No chart was specified, sorry, can not proceed."));
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc( "KDChart" );
    if( ! doc.setContent(device, true, &errorStr, &errorLine,
         &errorColumn)) {
        QMessageBox::information(0 , mProgramName,
            tr("Parse error at line %1, column %2:\n%3")
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorStr));
        return false;
    }

    const bool bOK = parseChartElement( doc.documentElement() );
    if( bOK )
        mChart->update();
    return bOK;
}


bool Serializer::write(QIODevice *device) const
{
    // Create an inital DOM document
    QString docstart = "<kdchart:kdchart/>";

    QDomDocument doc( "KDChart" );

    doc.setContent( docstart );

    QDomElement docRoot = doc.documentElement();
    if( saveRootElement( doc, docRoot ) ){
        const int IndentSize = 4;

        QTextStream out(device);
        doc.save(out, IndentSize);
        return true;
    }

    return false;
}


bool Serializer::parseChartElement( const QDomElement& root )const
{
    //qDebug() << root.tagName();
    bool bOK = true;
    if (root.tagName() != "kdchart") {
        QMessageBox::information(
                0 , mProgramName,
                tr("The file is not a KD Chart file."));
        return false;
    } else if (root.hasAttribute("kdchart:version")
               && ! root.attribute("kdchart:version").startsWith("2.") ) {
        QMessageBox::information(
                0 , mProgramName,
                tr("The file is not a KD Chart version 2.x file."));
        return false;
    }

    if( ! SerializeCollector::initializeGlobalPointers( root ) ){
        QMessageBox::information(
                0 , mProgramName,
                tr("Error while parsing the root node: Can not initialize global pointers."));
        return false;
    }

    QDomNode n = root.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            // the node really is an element
            if( e.tagName() == "global-leading" ){
                // we initialize the leading values, since
                // it might be that not all of them are stored
                int left   = mChart->globalLeadingLeft();
                int top    = mChart->globalLeadingTop();
                int right  = mChart->globalLeadingRight();
                int bottom = mChart->globalLeadingBottom();
                if( AttributesSerializer::parseLeading( e,  left, top, right, bottom ) )
                    mChart->setGlobalLeading( left, top, right, bottom );
            } else if( e.tagName() == "frame-attributes" ){
                FrameAttributes a;
                if( AttributesSerializer::parseFrameAttributes( e, a ) )
                    mChart->setFrameAttributes( a );
            } else if( e.tagName() == "background-attributes" ){
                BackgroundAttributes a;
                if( AttributesSerializer::parseBackgroundAttributes( e, a ) )
                    mChart->setBackgroundAttributes( a );
            } else if( e.tagName() == "kdchart:coordinate-planes:pointers" ){
                bool bFirstPlane=true;
                QDomNode node2 = e.firstChild();
                while( ! node2.isNull() ) {
                    AbstractCoordinatePlane* plane;
                    if( mCoordS->parsePlane( root, node2, plane ) ){
                        if( bFirstPlane ){
                            bFirstPlane = false;
                            mChart->replaceCoordinatePlane( plane );
                        }else{
                            mChart->addCoordinatePlane( plane );
                        }
                    }else{
                        qDebug()<< "Could not parse Chart / kdchart:coordinate-planes:pointers. Global pointer is not a KDChart::AbstractCoordinatePlane-ptr.";
                        bOK = false;
                    }
                    node2 = node2.nextSibling();
                }
            } else if( e.tagName() == "kdchart:headers-footers:pointers" ){
                QDomNode node2 = e.firstChild();
                while( ! node2.isNull() ) {
                    HeaderFooter* hdFt;
                    if( TextAreaSerializer::parseHeaderFooter( root, node2, hdFt ) ){
                        mChart->addHeaderFooter( hdFt );
                    }else{
                        qDebug()<< "Could not parse Chart / kdchart:headers-footers:pointers. Global pointer is not a KDChart::HeaderFooter-ptr.";
                        bOK = false;
                    }
                    node2 = node2.nextSibling();
                }
            } else if( e.tagName() == "kdchart:legends:pointers" ){
               QDomNode node2 = e.firstChild();
                while( ! node2.isNull() ) {
                    Legend* legend;
                    if( LegendsSerializer::parseLegend( root, node2, legend ) ){
                        mChart->addLegend( legend );
                    }else{
                        qDebug()<< "Could not parse Chart / kdchart:legends-footers:pointers. Global pointer is not a KDChart::Legend-ptr.";
                        bOK = false;
                    }
                    node2 = node2.nextSibling();
                }
            } else {
                qDebug() << "Unknown subelement of KDChart::Chart found:" << e.tagName();
                bOK = false;
            }
        }
        n = n.nextSibling();
    }
    return bOK;
}


bool Serializer::saveRootElement(
        QDomDocument& doc,
        QDomElement& docRoot ) const
{
    if( ! mChart ){
        QMessageBox::information(0 , mProgramName,
                                 tr("Can not save Chart Root element: mChart was not set!"));
        return false;
    }

    // Create an inital DOM document
    docRoot.setAttribute( "xmlns", "http://www.klaralvdalens-datakonsult.se/kdchart2" );
    docRoot.setAttribute( "xmlns:xsi", "http://www.w3.org/2000/10/XMLSchema-instance" );
    docRoot.setAttribute( "xsi:schemaLocation", "http://www.klaralvdalens-datakonsult.se/kdchart2" );

    docRoot.setAttribute( "kdchart:version", "2.1" );

    bool bOK = saveChartElement( doc, docRoot );

    if( bOK ){
        // So far the document is having this content only:
        //
        // <kdchart:kdchart ... >
        //   <kdchart:body>
        //     <kdchart:charts:pointers>
        //       <kdchart:pointer>kdchart:chart:1</kdchart:pointer>
        //     </kdchart:charts:pointers>
        //   </kdchart:body>
        // </kdchart:kdchart>
        //
        // Now save all collected data:
        // attribute-models, axes, charts, coordinate-planes,
        // diagrams, headers-footers, legends, ...
        SerializeCollector::instance()->appendDataToElement(
                doc, docRoot );

        // Last step: Try to resolve all entries that
        // were stored as unresolved pointers before.
        SerializeCollector::instance()->resolvePointers( doc, docRoot );
    }
    return bOK;
}


bool Serializer::saveChartElement(
        QDomDocument& doc,
        QDomElement& e ) const
{
    if( ! mChart ){
        QMessageBox::information(0 , mProgramName,
                                 tr("Can not save Chart element: mChart was not set!"));
        return false;
    }

    //qDebug() << "Serializer saving chart element:" << mChart;

    QDomElement bodyElement =
            doc.createElement( "kdchart:body" );
    e.appendChild( bodyElement );

    // note: The following structure can be easily extended
    //       to allow saving of more than one chart.
    // Every chart is added to the SerializeCollector in the
    // same way as we do for coordinate-planes, diagrams, ...

    const QString title( "kdchart:charts" );

    QDomElement* chartsList =
            SerializeCollector::instance()->findOrMakeElement( doc, title );

    // create the local list holding names pointing into the global list
    QDomElement pointersList =
            SerializeCollector::createPointersList( doc, bodyElement, title );

    bool wasFound;
    QDomElement chartElement =
            SerializeCollector::findOrMakeChild(
            doc,
            *chartsList,
            pointersList,
            "kdchart:chart",
            "KDChart::Chart",
            mChart,
            wasFound );
    // as of yet, wasFound will be FALSE always, but never mind
    if( ! wasFound ){
        // save the global leading
        AttributesSerializer::saveLeading(
                doc, chartElement,
                mChart->globalLeadingLeft(),
                mChart->globalLeadingTop(),
                mChart->globalLeadingRight(),
                mChart->globalLeadingBottom(),
                "kdchart:global-leading" );

        // save the frame attributes
        AttributesSerializer::saveFrameAttributes(
                doc, chartElement,
                mChart->frameAttributes(),
                "kdchart:frame-attributes" );

        // save the background attributes
        AttributesSerializer::saveBackgroundAttributes(
                doc, chartElement,
                mChart->backgroundAttributes(),
                "kdchart:background-attributes" );

        // save the coordinate planes:
        // Data will be stored by the SerializeCollector.
        mCoordS->savePlanes(
                doc, chartElement,
                mChart->coordinatePlanes(),
                "kdchart:coordinate-planes" );

        // save the headers / footers
        TextAreaSerializer::saveHeadersFooters(
                doc, chartElement,
                mChart->headerFooters(),
                "kdchart:headers-footers" );

        // save the legends
        LegendsSerializer::saveLegends(
                doc, chartElement,
                mChart->legends(),
                "kdchart:legends" );
    }
    return true;
}
