/* -*- Mode: C++ -*-
   KD Tools - a set of useful widgets for Qt
*/

/****************************************************************************
** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Tools library.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid commercial KD Tools licenses may use this file in
** accordance with the KD Tools Commercial License Agreement provided with
** the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.klaralvdalens-datakonsult.se/?page=products for
**   information about KD Tools Commercial License Agreements.
**
** Contact info@klaralvdalens-datakonsult.se if any conditions of this
** licensing are not clear to you.
**
**********************************************************************/

#include <qpainter.h>
#include <qbrush.h>

#include <KDFrame1.h>
#include <KDFrame1ProfileSection.h>
//#include <KDXMLTools1.h>

#if defined( SUN7 ) || defined( _SGIAPI )
#include <math.h>
#else
#include <cmath>
#endif
#ifdef Q_WS_WIN
#define M_PI 3.14159265358979323846
#endif


KDFrame1::~KDFrame1()
{
    deleteProfileSections();
}

KDFrame1::KDFrame1Corner::~KDFrame1Corner()
{
    // Intentionally left blank for now.
}


void KDFrame1::deleteProfileSections()
{
    if( _profileSectionsAutoDelete ){
        qDeleteAll( _profileSections );
        _profileSections.clear();
    }
}



void KDFrame1::paintBackground( QPainter& painter, const QRect& innerRect ) const
{
    /* first draw the brush (may contain a pixmap)*/
    if( Qt::NoBrush != _background.style() ) {
        QPen   oldPen(   painter.pen() );
        QPoint oldOrig(  painter.brushOrigin() );
        QBrush oldBrush( painter.brush() );
        painter.setPen( Qt::NoPen );
        //const QPoint newTopLeft( painter.xForm( innerRect.topLeft() ) );
        const QPoint newTopLeft( innerRect.topLeft() );
        painter.setBrushOrigin( newTopLeft.x(), newTopLeft.y() );
        painter.setBrush( _background );
        painter.drawRect( innerRect.x(), innerRect.y(),
                          innerRect.width(), innerRect.height() );
        painter.setPen(         oldPen );
        painter.setBrushOrigin( oldOrig );
        painter.setBrush(       oldBrush );
    }
    /* next draw the backPixmap over the brush */
    if( ! _backPixmap.isNull() ) {
        QPoint ol = innerRect.topLeft();
        if( PixCentered == _backPixmapMode )
        {
            ol.setX( innerRect.center().x() - _backPixmap.width() / 2 );
            ol.setY( innerRect.center().y() - _backPixmap.height()/ 2 );
            //bitBlt( painter.device(), ol, &_backPixmap );
            painter.drawPixmap(ol, _backPixmap, QRect(QPoint(0,0), _backPixmap.size()));
        } else {
            QMatrix m;
            double zW = (double)innerRect.width()  / (double)_backPixmap.width();
            double zH = (double)innerRect.height() / (double)_backPixmap.height();
            switch ( _backPixmapMode ) {
                case PixCentered:
                    break;
                case PixScaled: {
                                    double z;
                                    z = qMin( zW, zH );
                                    m.scale( z, z );
                                }
                                break;
                case PixStretched:
                                m.scale( zW, zH );
                                break;
            }
            //QPixmap pm = _backPixmap.xForm( m );
            QPixmap pm = _backPixmap.transformed( m );
            ol.setX( innerRect.center().x() - pm.width() / 2 );
            ol.setY( innerRect.center().y() - pm.height()/ 2 );
            //bitBlt( painter.device(), ol, &pm );
            painter.drawPixmap(ol, pm, QRect(QPoint(0,0), pm.size()));
        }
    }
}


void KDFrame1::paintEdges( QPainter& painter, const QRect& innerRect ) const
    {

/*
    Note: The following code OF COURSE is only dummy-code and will be replaced.

          At the moment it is used to draw the simple frame which was set by
          the setSimpleFrame() function.
*/
    if( !_topProfile.isEmpty() ){

        KDFrame1ProfileSection* section = ((KDFrame1Profile)_topProfile).last();
        if( section ){
            const QPen   oldPen   = painter.pen();
            const QBrush oldBrush = painter.brush();
            QPen thePen;
            thePen = section->pen();
            int penWidth = qMax(thePen.width(), 1) * qMax(section->width(), 1);
//qDebug("paintEdges: thePen.width() = %i", thePen.width());
//qDebug("paintEdges: section->width() = %i", section->width());
//qDebug("paintEdges: penWidth = %i", penWidth);
            thePen.setWidth( penWidth );
            painter.setPen( thePen );
            painter.setBrush( Qt::NoBrush );
            painter.drawRect( innerRect.x()-penWidth,
                              innerRect.y()-penWidth,
                              innerRect.width()  +2*penWidth,
                              innerRect.height() +2*penWidth );
            painter.setBrush( oldBrush );
            painter.setPen( oldPen );
        }
    }
}


void KDFrame1::paintCorners( QPainter& /*painter*/, const QRect& /*innerRect*/ ) const
{
    // At the moment nothing is done here since the setSimpleFrame() function
    // does not allow specifying corners: all predefined frames have normal
    // (default) corners.
    ;
}


void KDFrame1::paint( QPainter* painter,
        KDFrame1PaintSteps steps,
        QRect innerRect ) const
{
    if( painter ) {
        const QRect& rect = innerRect.isValid() ? innerRect : _innerRect;
        switch( steps ) {
            case PaintBackground:
                paintBackground( *painter, rect );
                break;
            case PaintEdges:
                paintEdges(      *painter, rect );
                break;
            case PaintCorners:
                paintCorners(    *painter, rect );
                break;
            case PaintBorder:
                paintEdges(      *painter, rect );
                paintCorners(    *painter, rect );
                break;
            case PaintAll:
                paintBackground( *painter, rect );
                paintEdges(      *painter, rect );
                paintCorners(    *painter, rect );
                break;
        }
    }
}


void KDFrame1::clearProfile( ProfileName name )
{
    switch( name ) {
        case ProfileTop:   _topProfile.clear();
                           break;
        case ProfileRight: _rightProfile.clear();
                           break;
        case ProfileBottom:_bottomProfile.clear();
                           break;
        case ProfileLeft:  _leftProfile.clear();
                           break;
    }
}

void KDFrame1::addProfileSection( ProfileName      name,
                                 int              wid,
                                 QPen             pen,
                                 KDFrame1ProfileSection::Direction dir,
                                 KDFrame1ProfileSection::Curvature curv )
{
    switch( name ) {
        case ProfileTop:   _topProfile.append(   new KDFrame1ProfileSection( dir, curv, wid, pen ) );
                           break;
        case ProfileRight: _rightProfile.append( new KDFrame1ProfileSection( dir, curv, wid, pen ) );
                           break;
        case ProfileBottom:_bottomProfile.append(new KDFrame1ProfileSection( dir, curv, wid, pen ) );
                           break;
        case ProfileLeft:  _leftProfile.append(  new KDFrame1ProfileSection( dir, curv, wid, pen ) );
                           break;
    }
}

void KDFrame1::setProfile( ProfileName name, const KDFrame1Profile& profile )
{
    switch( name ) {
        case ProfileTop:   _topProfile    = profile;
                           break;
        case ProfileRight: _rightProfile  = profile;
                           break;
        case ProfileBottom:_bottomProfile = profile;
                           break;
        case ProfileLeft:  _leftProfile   = profile;
                           break;
    }
}

const KDFrame1Profile& KDFrame1::profile( ProfileName name ) const
{
    switch( name ) {
        case ProfileTop:   return _topProfile;
                           break;
        case ProfileRight: return _rightProfile;
                           break;
        case ProfileBottom:return _bottomProfile;
                           break;
        default:           return _leftProfile;
    }

    return _leftProfile;
}


void KDFrame1::setSimpleFrame( SimpleFrame frame,
                              int         lineWidth,
                              int         midLineWidth,
                              QPen        pen,
                              QBrush      background,
                              const QPixmap* backPixmap,
                              BackPixmapMode backPixmapMode )
{
    deleteProfileSections();
    _topProfile.clear();
    _rightProfile.clear();
    _bottomProfile.clear();
    _leftProfile.clear();
    _background = background;
    _backPixmap = backPixmap ? *backPixmap : QPixmap();
    _backPixmapMode = backPixmapMode;
    if( FrameFlat == frame ) {
        //qDebug("_profileSections.count() before = %i", _profileSections.count());
        KDFrame1ProfileSection* newsection =
            new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                    KDFrame1ProfileSection::CvtPlain,
                    lineWidth, pen );
        _profileSections.append( newsection );
        //qDebug( "_profileSections.count() after = %i,    lineWidth = %i",
        //        _profileSections.count(),
        //        lineWidth );
        _topProfile.append( newsection );
        _rightProfile  = _topProfile;
        _bottomProfile = _topProfile;
        _leftProfile   = _topProfile;
    }
    else {
        switch( frame ) {
            case FrameElegance: {
                                    int line1 = lineWidth / 8;
                                    int line2 = lineWidth / 16;
                                    int line3 = line2;
                                    int gap2  = line2 * 4;
                                    int gap1  = lineWidth - line1 - line2 - line3 - gap2;
                                    QPen noP( Qt::NoPen );
                                    KDFrame1ProfileSection* newsection;
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                                            KDFrame1ProfileSection::CvtPlain,
                                            line1, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                                            KDFrame1ProfileSection::CvtPlain,
                                            gap1,  noP );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                                            KDFrame1ProfileSection::CvtPlain,
                                            line2, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                                            KDFrame1ProfileSection::CvtPlain,
                                            gap2,  noP );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection =  new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                                            KDFrame1ProfileSection::CvtPlain,
                                            line3, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                }
                                break;
            case FrameBoxRaized:
                                {
                                    KDFrame1ProfileSection* newsection;
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirRaising,
                                            KDFrame1ProfileSection::CvtPlain,
                                            lineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                                            KDFrame1ProfileSection::CvtPlain,
                                            midLineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection =  new KDFrame1ProfileSection( KDFrame1ProfileSection::DirSinking,
                                            KDFrame1ProfileSection::CvtPlain,
                                            lineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    break;
                                }
            case FrameBoxSunken:
                                {
                                    KDFrame1ProfileSection* newsection;
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirSinking,
                                            KDFrame1ProfileSection::CvtPlain,
                                            lineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                                            KDFrame1ProfileSection::CvtPlain,
                                            midLineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirRaising,
                                            KDFrame1ProfileSection::CvtPlain,
                                            lineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                }
                                break;
            case FramePanelRaized:
                                {
                                    KDFrame1ProfileSection* newsection;
                                    newsection =  new KDFrame1ProfileSection( KDFrame1ProfileSection::DirRaising,
                                            KDFrame1ProfileSection::CvtPlain,
                                            lineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    break;
                                }
            case FramePanelSunken:
                                {
                                    KDFrame1ProfileSection* newsection;
                                    newsection =  new KDFrame1ProfileSection( KDFrame1ProfileSection::DirSinking,
                                            KDFrame1ProfileSection::CvtPlain,
                                            lineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                }
                                break;
            case FrameSemicircular:
                                {
                                    KDFrame1ProfileSection* newsection;
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirRaising,
                                            KDFrame1ProfileSection::CvtConvex,
                                            lineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection =  new KDFrame1ProfileSection( KDFrame1ProfileSection::DirPlain,
                                            KDFrame1ProfileSection::CvtPlain,
                                            midLineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                    newsection = new KDFrame1ProfileSection( KDFrame1ProfileSection::DirSinking,
                                            KDFrame1ProfileSection::CvtConcave,
                                            lineWidth, pen );
                                    _profileSections.append( newsection );
                                    _topProfile.append( newsection );
                                }
                                break;
            default:
                                break;
        }
    }
    _rightProfile  = _topProfile;
    _bottomProfile = _topProfile;
    _leftProfile   = _topProfile;
    setCorners( CornerNormal, 0 );
}


void KDFrame1::createFrameNode( QDomDocument& document, QDomNode& parent,
        const QString& elementName,
        const KDFrame1& frame )
{
    Q_UNUSED( document );
    Q_UNUSED( parent );
    Q_UNUSED( elementName );
    Q_UNUSED( frame );
    return;

    /*
    QDomElement frameElement = document.createElement( elementName );
    parent.appendChild( frameElement );
    KDXML::createIntNode( document, frameElement, "ShadowWidth",
            frame._shadowWidth );
    KDXML::createStringNode( document, frameElement, "CornerName",
            cornerNameToString( frame._sunPos ) );

    KDXML::createBrushNode( document, frameElement, "Background",
            frame._background );
    KDXML::createPixmapNode( document, frameElement, "BackPixmap",
            frame._backPixmap );
    KDXML::createStringNode( document, frameElement, "BackPixmapMode",
            backPixmapModeToString( frame._backPixmapMode ) );

    KDXML::createRectNode( document, frameElement, "InnerRect",
            frame._innerRect );
    createFrameProfileNode( document, frameElement, "TopProfile",
            frame._topProfile );
    createFrameProfileNode( document, frameElement, "RightProfile",
            frame._rightProfile );
    createFrameProfileNode( document, frameElement, "BottomProfile",
            frame._bottomProfile );
    createFrameProfileNode( document, frameElement, "LeftProfile",
            frame._leftProfile );
    KDFrame1Corner::createFrameCornerNode( document, frameElement, "CornerTL",
            frame._cornerTL );
    KDFrame1Corner::createFrameCornerNode( document, frameElement, "CornerTR",
            frame._cornerTR );
    KDFrame1Corner::createFrameCornerNode( document, frameElement, "CornerBL",
            frame._cornerBL );
    KDFrame1Corner::createFrameCornerNode( document, frameElement, "CornerBR",
            frame._cornerBR );
    */
}

void KDFrame1::createFrameProfileNode( QDomDocument& document, QDomNode& parent,
        const QString& elementName,
        KDFrame1Profile profile )
{
    QDomElement profileElement = document.createElement( elementName );
    parent.appendChild( profileElement );

    for (int i = 0; i < profile.size(); ++i) {
        KDFrame1ProfileSection::createFrameProfileSectionNode( document,
                profileElement,
                "ProfileSection",
                profile.at(i) );
    }
}


void KDFrame1::KDFrame1Corner::createFrameCornerNode( QDomDocument& document,
        QDomNode& parent,
        const QString& elementName,
        const KDFrame1Corner& corner )
{
    Q_UNUSED( document );
    Q_UNUSED( parent );
    Q_UNUSED( elementName );
    Q_UNUSED( corner );
    return;

    /*
    QDomElement cornerElement = document.createElement( elementName );
    parent.appendChild( cornerElement );
    KDXML::createStringNode( document, cornerElement, "Style",
            KDFrame1::cornerStyleToString( corner._style ) );
    KDXML::createIntNode( document, cornerElement, "Width",
            corner._width );
    createFrameProfileNode( document, cornerElement, "Profile",
            corner._profile );
    */
}

bool KDFrame1::readFrameNode( const QDomElement& element, KDFrame1& frame )
{
    Q_UNUSED( element );
    Q_UNUSED( frame );
    return false;

    /*
    bool ok = true;
    int tempShadowWidth;
    CornerName tempCornerName=CornerTopLeft;
    QBrush tempBackground;
    QPixmap tempBackPixmap;
    BackPixmapMode tempBackPixmapMode=PixStretched;
    QRect tempInnerRect;
    KDFrame1Profile tempTopProfile, tempRightProfile,
    tempBottomProfile, tempLeftProfile;
    KDFrame1Corner tempCornerTL, tempCornerTR, tempCornerBL, tempCornerBR;
    QDomNode node = element.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "ShadowWidth" ) {
                ok = ok & KDXML::readIntNode( element, tempShadowWidth );
            } else if( tagName == "CornerName" ) {
                QString value;
                ok = ok & KDXML::readStringNode( element, value );
                tempCornerName = stringToCornerName( value );
            } else if( tagName == "Background" ) {
                ok = ok & KDXML::readBrushNode( element, tempBackground );
            } else if( tagName == "BackPixmap" ) {
                ok = ok & KDXML::readPixmapNode( element, tempBackPixmap );
            } else if( tagName == "BackPixmapMode" ) {
                QString value;
                ok = ok & KDXML::readStringNode( element, value );
                tempBackPixmapMode = stringToBackPixmapMode( value );
            } else if( tagName == "InnerRect" ) {
                ok = ok & KDXML::readRectNode( element, tempInnerRect );
            } else if( tagName == "TopProfile" ) {
                ok = ok & readFrameProfileNode( element, tempTopProfile );
            } else if( tagName == "RightProfile" ) {
                ok = ok & readFrameProfileNode( element, tempRightProfile );
            } else if( tagName == "BottomProfile" ) {
                ok = ok & readFrameProfileNode( element, tempBottomProfile );
            } else if( tagName == "LeftProfile" ) {
                ok = ok & readFrameProfileNode( element, tempLeftProfile );
            } else if( tagName == "CornerTL" ) {
                ok = ok & KDFrame1Corner::readFrameCornerNode( element,
                        tempCornerTL );
            } else if( tagName == "CornerTR" ) {
                ok = ok & KDFrame1Corner::readFrameCornerNode( element,
                        tempCornerTR );
            } else if( tagName == "CornerBL" ) {
                ok = ok & KDFrame1Corner::readFrameCornerNode( element,
                        tempCornerBL );
            } else if( tagName == "CornerBR" ) {
                ok = ok & KDFrame1Corner::readFrameCornerNode( element,
                        tempCornerBR );
            } else {
                qDebug( "Unknown tag in frame" );
            }
        }
        node = node.nextSibling();
    }

    if( ok ) {
        frame._shadowWidth = tempShadowWidth;
        frame._sunPos = tempCornerName;
        frame._background = tempBackground;
        frame._backPixmap = tempBackPixmap;
        frame._backPixmapMode = tempBackPixmapMode;
        frame._innerRect = tempInnerRect;
        frame._topProfile = tempTopProfile;
        frame._rightProfile = tempRightProfile;
        frame._bottomProfile = tempBottomProfile;
        frame._leftProfile = tempLeftProfile;
        frame._cornerTL = tempCornerTL;
        frame._cornerTR = tempCornerTR;
        frame._cornerBL = tempCornerBL;
        frame._cornerBR = tempCornerBR;
    }

    return ok;
    */
}


bool KDFrame1::readFrameProfileNode( const QDomElement& element,
        KDFrame1Profile& profile )
{
    Q_UNUSED( element );
    Q_UNUSED( profile );
    return false;

    /*
    QDomNode node = element.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "ProfileSection" ) {
                KDFrame1ProfileSection* section = new KDFrame1ProfileSection();
                KDFrame1ProfileSection::readFrameProfileSectionNode( element,
                        section );
                profile.append( section );
            } else {
                qDebug( "Unknown tag in double map" );
                return false;
            }
        }
        node = node.nextSibling();
    }

    return true;
    */
}


bool KDFrame1::KDFrame1Corner::readFrameCornerNode( const QDomElement& element,
        KDFrame1Corner& corner )
{
    Q_UNUSED( element );
    Q_UNUSED( corner );
    return false;

    /*
    bool ok = true;
    CornerStyle tempStyle=CornerNormal;
    int tempWidth;
    KDFrame1Profile tempProfile;
    QDomNode node = element.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Style" ) {
                QString value;
                ok = ok & KDXML::readStringNode( element, value );
                tempStyle = stringToCornerStyle( value );
            } else if( tagName == "Width" ) {
                ok = ok & KDXML::readIntNode( element, tempWidth );
            } else if( tagName == "Profile" ) {
                KDFrame1Profile profile;
                ok = ok & readFrameProfileNode( element, profile );
            } else {
                qDebug( "Unknown tag in frame" );
            }
        }
        node = node.nextSibling();
    }

    if( ok ) {
        corner._style = tempStyle;
        corner._width = tempWidth;
        corner._profile = tempProfile;
    }

    return ok;
    */
}
