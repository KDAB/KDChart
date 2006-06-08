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
#ifndef __KDCHART1WIDGET_H__
#define __KDCHART1WIDGET_H__

#include <KDChart1Global.h>
#include <KDChart1Table.h>
#include <KDChart1DataRegion.h>
#include <qwidget.h>
#include <qpixmap.h>

#include <KDChartWidget>

/**
  \file KDChart1Widget.h

  \brief Provids the entry point into the charting, for the compatibility library.
  */

class KDChart1Params;

class KDCHART1_EXPORT KDChart1Widget : public KDChart::Widget
{
    Q_OBJECT
    Q_PROPERTY( bool activeData READ isActiveData WRITE setActiveData )
    Q_PROPERTY( bool doubleBuffered READ isDoubleBuffered WRITE setDoubleBuffered )

public:
    KDChart1Widget( QWidget* parent = 0, const char* name = 0 );
    KDChart1Widget( KDChart1Params* params,
                    KDChart1TableDataBase* data,
                    QWidget* parent = 0, const char* name = 0 );
    ~KDChart1Widget();

public slots:
    bool isActiveData() const;
    bool isDoubleBuffered() const;

    KDChart1Params* params() const;
    KDChart1TableDataBase* data() const;

    const KDChart1DataRegionList* dataRegions() const;

    void setActiveData( bool active );
    void setDoubleBuffered( bool doublebuffered );
    void setParams( KDChart1Params* params );
    void setData( KDChart1TableDataBase* data );
    /*
    // use this method to paint to low resolution devices
    void paintTo( QPainter& painter,
                  const QRect* rect = 0 );
    // use this method to paint to high res devices like printers...
    void print( QPainter& painter,
                const QRect* rect = 0  );
    */


signals:
    void dataLeftClicked( uint row, uint col );
    void dataLeftClicked( const QPoint & pnt );
    void dataMiddleClicked( uint row, uint col );
    void dataMiddleClicked( const QPoint & pnt );
    void dataRightClicked( uint row, uint col );
    void dataRightClicked( const QPoint & pnt );
    void dataLeftPressed( uint row, uint col );
    void dataLeftPressed( const QPoint & pnt );
    void dataMiddlePressed( uint row, uint col );
    void dataMiddlePressed( const QPoint & pnt );
    void dataRightPressed( uint row, uint col );
    void dataRightPressed( const QPoint & pnt );
    void dataLeftReleased( uint row, uint col );
    void dataLeftReleased( const QPoint & pnt );
    void dataMiddleReleased( uint row, uint col );
    void dataMiddleReleased( const QPoint & pnt );
    void dataRightReleased( uint row, uint col );
    void dataRightReleased( const QPoint & pnt );
    void barsDisplayed( int barsDisplayed, int barsLeft );

protected:
    //virtual void paintEvent( QPaintEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );

private:
    class Private;
    Private * d;
};

#endif
