/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "KDChartAbstractArea.h"
#include "KDChartAbstractArea_p.h"

//FIXME(khz): use an internal libfakes library instead of this internal header file
#include "kdchart_platformdef.h"


using namespace KDChart;

AbstractArea::Private::Private() :
    AbstractAreaBase::Private()
{
    // this bloc left empty intentionally
}


AbstractArea::Private::~Private()
{
    // this bloc left empty intentionally
}


AbstractArea::AbstractArea()
    : KDChart::AbstractAreaBase()
    , KDChart::AbstractLayoutItem()
    , QObject()
{
    // this bloc left empty intentionally
}

AbstractArea::~AbstractArea()
{
    // this bloc left empty intentionally
}


void AbstractArea::paintIntoRect( QPainter& painter, const QRect& rect )
{
    const QRect oldGeometry( geometry() );
    if( oldGeometry != rect )
        setGeometry( rect );
    painter.translate( rect.left(), rect.top() );
    paintAll( painter );
    painter.translate( -rect.left(), -rect.top() );
    if( oldGeometry != rect )
        setGeometry( oldGeometry );
}

void AbstractArea::paintAll( QPainter& painter )
{
    // Paint the background and frame
    paintBackground( painter, geometry() );
    paintFrame(      painter, geometry() );

    // temporarily adjust the widget size, to be sure all content gets calculated
    // to fit into the inner rectangle
    const QRect oldGeometry( geometry()  );
    const QRect innerRect(   innerRect() );
    if( oldGeometry != innerRect )
        setGeometry( innerRect );
    painter.translate( innerRect.left(), innerRect.top() );
    paint( &painter );
    painter.translate( -innerRect.left(), -innerRect.top() );
    if( oldGeometry != innerRect )
        setGeometry( oldGeometry );
}

const QRect& AbstractArea::areaGeometry() const
{
    return geometry();
}

void AbstractArea::positionHasChanged()
{
    emit positionChanged( this );
}

