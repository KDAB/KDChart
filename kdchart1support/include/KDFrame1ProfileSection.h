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
#ifndef __KDFRAME1PROFILESECTION__
#define __KDFRAME1PROFILESECTION__

#include <qregion.h>
#include <qpen.h>
#include <qdom.h>
#include <QList>
#include "KDChart1Global.h"


/**
  Settings for one profile section of one side of the frame.
  */
class KDCHART1_EXPORT KDFrame1ProfileSection
{
public:
    /**
    Profile Direction Mode: specifying whether a part of a profile looks embossed or
    engraved in comparison to the next outer part (or to the surrounding area, resp.).

    \li \c DirPlain the part of the profile is neither embossed nor engraved.
    \li \c DirRaising the part of the profile lookes embossed from the outer part.
    \li \c DirSinking the part of the profile lookes engraved into the outer part.

    \Note Since a frame border may consist of several parts you must specify the
    direction mode for each of these parts. This is quite different from the
    QFrame::Shadow / QFrame::Shape schema where you would select a predefined
    profile. KDFrame1 lets you specify both the number of sections used
    to compose frame border's profile and the look of each individual
    section by calling \c addProfileSection() multiple times.

    Some commonly used profiles are allready pre-defined for your convenience,
    \c setSimpleProfile is used to select one of them, see \c setProfile
    for an example how to do this.

    \sa addProfileSection, setProfile, setSimpleProfile, SimpleProfile
    */
    enum Direction { DirPlain, DirRaising, DirSinking };

    /**
    Converts the specified direction enum to a string representation.

    \param dir the direction enum to convert
    \return the string representation of the direction enum
    */
    static QString directionToString( Direction dir ) {
        switch( dir ) {
            case DirPlain:
                return "Plain";
            case DirRaising:
                return "Raising";
            case DirSinking:
                return "Sinking";
            default: // should not happen
                return "Plain";
        }
    }


    /**
    Converts the specified string to a direction enum value.

    \param string the string to convert
    \return the direction enum value
    */
    static Direction stringToDirection( const QString& string ) {
        if( string == "Plain" )
            return DirPlain;
        else if( string == "Raising" )
            return DirRaising;
        else if( string == "Sinking" )
            return DirSinking;
        else // default, should not happen
            return DirPlain;
    }



    /**
    Profile Curvature Mode: specifying the shape of a frame profile section.
    (curvature setting will be ignored for \c DirPlain profiles)

    \li \c CvtFlat looking like a evenly sloping surface.
    \li \c CvtConvex looking like quarter of a torus.
    \li \c CvtConcave looking like half of a groove.
    */
    enum Curvature { CvtPlain, CvtConvex, CvtConcave };

    /**
    Converts the specified curvature enum to a string representation.

    \param curv the curvature enum to convert
    \return the string representation of the curvature enum
    */
    static QString curvatureToString( Curvature curv ) {
        switch( curv ) {
            case CvtPlain:
                return "Plain";
            case CvtConvex:
                return "Convex";
            case CvtConcave:
                return "Concave";
            default: // should not happen
                return "Plain";
        }
    }


    /**
    Converts the specified string to a curvature enum value.

    \param string the string to convert
    \return the curvature enum value
    */
    static Curvature stringToCurvature( const QString& string ) {
        if( string == "Plain" )
            return CvtPlain;
        else if( string == "Convex" )
            return CvtConvex;
        else if( string == "Concave" )
            return CvtConcave;
        else // default, should not happen
            return CvtPlain;
    }


    /**
    Ctor of KDFrame1ProfileSection.

    \Note Instead of instantiating KDFrame1ProfileSection yourself
    you would normally rather call \c KDFrame1::addProfileSection()
    */
    KDFrame1ProfileSection( Direction direction,
                           Curvature curvature,
                           int       width,
                           QPen      pen )
        : _direction( direction ),
          _curvature( curvature ),
          _width( width ),
          _pen( pen ) {}

    /**
    Default ctor of FrameProfileSection.

    \Note Instead of instantiating KDFrame1ProfileSection yourself,
    you would normally rather call \c KDFrame1::addProfileSection()
    */
    KDFrame1ProfileSection() {
        _direction = DirPlain;
        _curvature = CvtPlain;
        _width = 1;
        _pen = QPen( Qt::SolidLine );
    }

    /**
    Destructor. Only defined to have it virtual.
    */
    virtual ~KDFrame1ProfileSection();

    /**
    Creates a DOM element node that represents a frame profile
    section for use in a DOM document.

    \param document the DOM document to which the node will belong
    \param parent the parent node to which the new node will be appended
    \param elementName the name of the new node
    \param section the section to be represented
    */
    static void createFrameProfileSectionNode( QDomDocument& document,
            QDomNode& parent,
            const QString& elementName,
            const KDFrame1ProfileSection* section );

    /**
    Reads data from a DOM element node that represents a frame
    profile section and fills a KDFrame1ProfileSection object with
    the data.

    \param element the DOM element to read from
    \param section a pointer to the frame profile section object to
    read the data into
    */
    static bool readFrameProfileSectionNode( const QDomElement& element,
            KDFrame1ProfileSection* section );

    Direction direction() const { return _direction; }
    Curvature curvature() const { return _curvature; }
    int       width()     const { return _width;     }
    QPen      pen()       const { return _pen;       }

private:
    Direction _direction;
    Curvature _curvature;
    int       _width;
    QPen      _pen;
};

/**
  Settings for all the profile sections of one side the frame.

  \sa setProfile, profile
  */
typedef QList < KDFrame1ProfileSection* > KDFrame1Profile;

#endif
