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
#ifndef __KDCHART1DATAREGION__
#define __KDCHART1DATAREGION__

#include <qregion.h>
#include <QPolygon>

#include "KDChart1Global.h"

/*!
  \internal
  */
struct KDCHART1_EXPORT KDChart1DataRegion
{
    typedef QList<QPolygon> PolygonList;

    KDChart1DataRegion()
    {
        init();
    }

    KDChart1DataRegion( QRegion region, uint r, uint c, uint ch )
    {
        init();
        pRegion = new QRegion( region );
        row     = r;
        col     = c;
        chart   = ch;
    }

    KDChart1DataRegion( uint r, uint c, uint ch, QPolygon array )
    {
        init();
        pPolygon = new QPolygon( array );
        row     = r;
        col     = c;
        chart   = ch;
    }

    KDChart1DataRegion( uint r, uint c, uint ch, QRect rect )
    {
        init();
        pRect = new QRect( rect );
        row   = r;
        col   = c;
        chart = ch;
    }
    KDChart1DataRegion( uint r, uint c, uint ch, PolygonList list )
    {
        init();
        pPolygonList = new PolygonList( list );
        row   = r;
        col   = c;
        chart = ch;
    }

    ~KDChart1DataRegion()
    {
        //qDebug ("~KDChart1DataRegion");
        if( pPolygonList )
            delete pPolygonList;
        if( pRegion )
            delete pRegion;
        if( pPolygon )
            delete pPolygon;
        if( pRect )
            delete pRect;
        if( pTextRegion )
            delete pTextRegion;
    }

    QRegion region() const
    {
        if( pPolygonList && ! pPolygonList->empty() ){
            QRegion region;
            PolygonList::iterator it;
            for ( it = pPolygonList->begin(); it != pPolygonList->end(); ++it ){
                region += QRegion( *it );
            }
            return region;
        }
        if( pRegion )
            return *pRegion;
        if( pPolygon )
            return QRegion( *pPolygon );
        if( pRect )
            return QRegion( *pRect );
        return QRegion();
    }

    QRect rect() const
    {
        if( pPolygonList && ! pPolygonList->empty() ){
            QRect rect;
            PolygonList::iterator it;
            for ( it = pPolygonList->begin(); it != pPolygonList->end(); ++it ){
                rect = rect.unite( (*it).boundingRect() );
            }
            return QRect();
        }
        if( pRegion )
            return pRegion->boundingRect();
        if( pPolygon )
            return pPolygon->boundingRect();
        if( pRect )
            return *pRect;
        return QRect();
    }

    bool contains(const QPoint & p) const
    {
        if( pPolygonList && ! pPolygonList->empty() ){
            PolygonList::iterator it;
            for ( it = pPolygonList->begin(); it != pPolygonList->end(); ++it ){
                QRegion region( points );
                if( region.contains( p ) )
                    return true;
            }
            return false;
        }
        if( pRegion )
            return pRegion->contains( p );
        if( pPolygon )
            return QRegion( *pPolygon ).contains( p );
        if( pRect )
            return pRect->contains( p );
        return false;
    }

    void init()
    {
        pRegion      = 0;
        pPolygon     = 0;
        pRect        = 0;
        pTextRegion  = 0;
        pPolygonList = 0;
        row    = 0;
        col    = 0;
        chart  = 0;
        negative = false; // default value (useful if value is a string)
        points.resize( 9 );
        startAngle = 1440;
        angleLen   =    1;
    }

    QRegion*  pRegion;
    QPolygon* pPolygon;
    QRect*    pRect;
    QRegion*  pTextRegion;  // for the data values text

    // for three-dimensional bars, and for comparable structures, needing
    // more than one QPointArray, we use this list:
    PolygonList* pPolygonList;

    // For rectangular data representation  (bar, line, area, point, ...)
    // we use the above declared 'pRect'.
    // For curved data representations (pie slice, ring segment, ...)
    // we store the following additional anchor information:

    // store 9 elements: one for each value of KDChart1Enums::PositionFlag
    QPolygon points;

    int startAngle; // Note: 5760 makes a full circle, 2880 is left 'corner'.
    int angleLen;

    uint row;
    uint col;
    // members needed for calculation of data values texts
    uint chart;
    QString text;        // the data values text
    bool    negative;    // stores whether the data value is less than zero
};


/**
  \class KDChart1DataRegionList KDChart1DataRegion.h

  \brief The collection class used by KD Chart to store data region information.

  This class is derived from QPtrList, so all of the Qt documentation for this class
  is valid for KDChart1DataRegionList too.

  \note Normally there is no need to use this class yourself, since it
  is instantiated by the KDChart1Widget. If however you are not using the
  KDChart1Widget class but calling the painting methods of KDChart1 directly,
  make sure to either free the pointer stored in KDChart1DataRegionList manually,
  or to call setAutoDelete( true ) to let your KDChart1DataRegionList own these pointers:
  in this case please also make sure to call the clear() method whenever you want
  your KDChart1DataRegionList to free these pointers.
  Note that all of this ONLY applies in case of NOT using the KDChart1Widget.

  \sa KDChart1, KDChart1Widget
  */
typedef QList < KDChart1DataRegion* > KDChart1DataRegionList;

#endif
