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

#include "KDChart1Params.h"


/**
  \class KDChart1Params::KDChart1FrameSettings KDChart1Params.h

  Stores the frame settings for one of the chart areas:
  \li frame (consisting of edges, corners, background, shadow)
  \li distance of frame to inner area
  \li distance of frame to surrounding parts of the chart

  \sa setSimpleFrame, setFrame, frameSettings
  */


/**
  Constructor. Setting default values.
  */
KDChart1Params::KDChart1FrameSettings::KDChart1FrameSettings()
{
    _dataRow = 0;
    _dataCol = 0;
    _data3rd = 0;
    _frame   = 0;
    resetFrame();
    _outerGapX = 0;
    _outerGapY = 0;
    _innerGapX = 0;
    _innerGapY = 0;
    _addFrameWidthToLayout = true;
    _addFrameHeightToLayout = true;
}


/**
  Constructor.

  \param outerGap Distance between the frame and the surrounding parts of the chart.
  \param innerGap Distance between the frame and inner area.
  \param addFrameWidthToLayout If true, shrink inner area so the area AND its frame will occupy
  the same space of the chart as the area would occupy if no frame were drawn.
  If false, the frame is drawn around the area without taking care not to override other
  content of the chart.
  \param frame The frame settings to be used for this area. The values of this parameter
  will be copied into a KDFrame1 object stored internally in this KDChart1FrameSettings so
  it is save to use the same KDFrame1 object for specifying the settings of more than one area.
  */
KDChart1Params::KDChart1FrameSettings::KDChart1FrameSettings(
        uint           dataRow,
        uint           dataCol,
        uint           data3rd,
        const KDFrame1& frame,
        int            outerGapX,
        int            outerGapY,
        int            innerGapX,
        int            innerGapY,
        bool           addFrameWidthToLayout,
        bool           addFrameHeightToLayout )
  : _dataRow( dataRow ),
    _dataCol( dataCol ),
    _data3rd( data3rd ),
    _frame( 0 ),
    _outerGapX( outerGapX ),
    _outerGapY( outerGapY ),
    _innerGapX( innerGapX ),
    _innerGapY( innerGapY ),
    _addFrameWidthToLayout(  addFrameWidthToLayout ),
_addFrameHeightToLayout( addFrameHeightToLayout )
{
    resetFrame();
    KDFrame1::deepCopy(*_frame, frame);
}



/**
  Destructor. Only defined to have it virtual.
  */
KDChart1Params::KDChart1FrameSettings::~KDChart1FrameSettings()
{
    delete _frame;
}

/**
  Reads data from a DOM element node that represents a frame
  settings object and fills a KDChart1FrameSettings object
  with the data.

  \param element the DOM element to read from
  \param settings the frame settings object to read the data into
  */
bool KDChart1Params::KDChart1FrameSettings::readFrameSettingsNode( const QDomElement& element,
        KDChart1FrameSettings& settings,
        uint& areaId )
{
    return false;
    /*
    bool ok = true;
    KDFrame1 tempFrame;
    int tempId = KDChart1Enums::AreaUNKNOWN;
    int tempDataRow = 0; // these 3 need initialization since they were not
    int tempDataCol = 0;//  existent in previous releases of KDChart1
    int tempData3rd = 0;

    int tempOuterGapX, tempOuterGapY, tempInnerGapX, tempInnerGapY;
    bool tempAddFrameWidthToLayout, tempAddFrameHeightToLayout;
    QDomNode node = element.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Frame" ) {
                ok = ok & KDFrame1::readFrameNode( element, tempFrame );
            } else if( tagName == "AreaId" ) {
                ok = ok & KDXML::readIntNode( element, tempId );
            } else if( tagName == "DataRow" ) {
                ok = ok & KDXML::readIntNode( element, tempDataRow );
            } else if( tagName == "DataCol" ) {
                ok = ok & KDXML::readIntNode( element, tempDataCol );
            } else if( tagName == "Data3rd" ) {
                ok = ok & KDXML::readIntNode( element, tempData3rd );
            } else if( tagName == "OuterGapX" ) {
                ok = ok & KDXML::readIntNode( element, tempOuterGapX );
            } else if( tagName == "OuterGapY" ) {
                ok = ok & KDXML::readIntNode( element, tempOuterGapY );
            } else if( tagName == "InnerGapX" ) {
                ok = ok & KDXML::readIntNode( element, tempInnerGapX );
            } else if( tagName == "InnerGapY" ) {
                ok = ok & KDXML::readIntNode( element, tempInnerGapY );
            } else if( tagName == "AddFrameWidthToLayout" ) {
                ok = ok & KDXML::readBoolNode( element, tempAddFrameWidthToLayout );
            } else if( tagName == "AddFrameHeightToLayout" ) {
                ok = ok & KDXML::readBoolNode( element, tempAddFrameHeightToLayout );
            } else {
                qDebug( "Unknown tag in frame settings" );
            }
        }
        node = node.nextSibling();
    }

    if( ok ) {
        settings.resetFrame();
        KDFrame1::deepCopy(*settings._frame, tempFrame);

        settings._dataRow   = tempDataRow;
        settings._dataCol   = tempDataCol;
        settings._data3rd   = tempData3rd;
        settings._outerGapX = tempOuterGapX;
        settings._outerGapY = tempOuterGapY;
        settings._innerGapX = tempInnerGapX;
        settings._innerGapY = tempInnerGapY;
        settings._addFrameWidthToLayout = tempAddFrameWidthToLayout;
        settings._addFrameHeightToLayout = tempAddFrameHeightToLayout;
        areaId = tempId;
    }

    return ok;
    */
}



/**
  \fn const KDFrame1& KDChart1Params::KDChart1FrameSettings::frame() const

  Return the KDFrame1 object used for drawing this areas frame.
  */


/**
  \fn int KDChart1Params::KDChart1FrameSettings::innerGapX() const

  Return the X-distance between the frame and the inner area.
  */

/**
  \fn int KDChart1Params::KDChart1FrameSettings::innerGapY() const

  Return the Y-distance between the frame and the inner area.
  */

/**
  \fn int KDChart1Params::KDChart1FrameSettings::outerGapX() const

  Return the X-distance between the frame and the surrounding parts of the chart.
  */

/**
  \fn int KDChart1Params::KDChart1FrameSettings::outerGapY() const

  Return the Y-distance between the frame and the surrounding parts of the chart.
  */

/**
  \fn bool KDChart1Params::KDChart1FrameSettings::addFrameWidthToLayout() const

  Return whether the inner area will shrink the area AND its frame will occupy
  the same space of the chart as the area would occupy if no frame were drawn.
  If false, the frame is drawn around the area without taking care not to override other
  content of the chart.

  \sa addFrameHeightToLayout
  */


/**
  \fn bool KDChart1Params::KDChart1FrameSettings::addFrameHeightToLayout() const

  Return whether the inner area will shrink the area AND its frame will occupy
  the same space of the chart as the area would occupy if no frame were drawn.
  If false, the frame is drawn around the area without taking care not to override other
  content of the chart.

  \sa addFrameWidthToLayout
  */



/**
  Creates a DOM element node that represents a frame settings
  object for use in a DOM document.

  \param document the DOM document to which the node will belong
  \param parent the parent node to which the new node will be appended
  \param elementName the name of the new node
  \param settings the frame settings to be represented
  */
void KDChart1Params::KDChart1FrameSettings::createFrameSettingsNode( QDomDocument& document,
        QDomNode& parent,
        const QString& elementName,
        const KDChart1Params::KDChart1FrameSettings* settings,
        uint areaId )
{
    return;
    /*
    QDomElement frameSettingsElement = document.createElement( elementName );
    parent.appendChild( frameSettingsElement );
    if( settings->_frame )
        KDFrame1::createFrameNode( document, frameSettingsElement, "Frame",
                                  *settings->_frame );
    KDXML::createIntNode( document, frameSettingsElement, "AreaId",
            areaId );
    KDXML::createIntNode( document, frameSettingsElement, "DataRow",
            settings->_dataRow );
    KDXML::createIntNode( document, frameSettingsElement, "DataCol",
            settings->_dataCol );
    KDXML::createIntNode( document, frameSettingsElement, "Data3rd",
            settings->_data3rd );
    KDXML::createIntNode( document, frameSettingsElement, "OuterGapX",
            settings->_outerGapX );
    KDXML::createIntNode( document, frameSettingsElement, "OuterGapY",
            settings->_outerGapY );
    KDXML::createIntNode( document, frameSettingsElement, "InnerGapX",
            settings->_innerGapX );
    KDXML::createIntNode( document, frameSettingsElement, "InnerGapY",
            settings->_innerGapY );
    KDXML::createBoolNode( document, frameSettingsElement,
            "AddFrameWidthToLayout",
            settings->_addFrameWidthToLayout );
    KDXML::createBoolNode( document, frameSettingsElement,
            "AddFrameHeightToLayout",
            settings->_addFrameHeightToLayout );
    */
}
