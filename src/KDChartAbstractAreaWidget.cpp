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

#include "KDChartAbstractAreaWidget.h"

//FIXME(khz): use an internal libfakes library instead of this internal header file
#include "kdchart_platformdef.h"


using namespace KDChart;


AbstractAreaWidget::Private::Private()
{
    // this bloc left empty intentionally
}

AbstractAreaWidget::Private::~Private()
{
    // this bloc left empty intentionally
}


AbstractAreaWidget::AbstractAreaWidget( QWidget* parent = 0 )
    : AbstractAreaBase()
    , QWidget( parent )
{
    // this bloc left empty intentionally
}

AbstractAreaWidget::~AbstractAreaWidget()
{
    // this bloc left empty intentionally
}

#define d d_func()

AbstractAreaWidget::AbstractAreaWidget( QWidget* parent ) :
    AbstractAreaBase( new Private() )
{
    d->parent = parent;
    init();
}

void AbstractAreaWidget::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );)
    QPainter painter( this );
    paintAll( painter );
}

void AbstractAreaWidget::paintIntoRect( QPainter& painter, const QRect& rect )
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

void AbstractAreaWidget::paintAll( QPainter& painter )
{
    // Paint the background and frame
    paintBackground( painter, rect() );
    paintFrame(      painter, rect() );

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

const QRect& AbstractAreaWidget::areaGeometry() const
{
    return geometry();
}

void AbstractAreaWidget::positionHasChanged()
{
    emit positionChanged( this );
}

