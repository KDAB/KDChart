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

#include "KDChartMarkerAttributes.h"
#include <QColor>
#include <QMap>
#include <QtXml/QDomDocumentFragment>
#include <qglobal.h>

#include <KDABLibFakes>

#define d d_func()

using namespace KDChart;

class MarkerAttributes::Private
{
    friend class MarkerAttributes;
public:
    Private();
private:
    bool visible;
    QMap<uint,MarkerStyle> markerStylesMap;
    MarkerStyle markerStyle;
    QSizeF markerSize;
    QColor markerColor;
    QPen markerPen;
};

MarkerAttributes::Private::Private()
{
}


MarkerAttributes::MarkerAttributes()
    : _d( new Private() )
{
    setVisible( false );
    setMarkerStyle( MarkerSquare );
    setMarkerSize( QSizeF(10,10) );
    setPen( QPen( Qt::black ) );
}

MarkerAttributes::MarkerAttributes( const MarkerAttributes& r )
    : _d( new Private( *r.d ) )
{

}

MarkerAttributes & MarkerAttributes::operator=( const MarkerAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

MarkerAttributes::~MarkerAttributes()
{
    delete _d; _d = 0;
}

bool MarkerAttributes::operator==( const MarkerAttributes& r ) const
{
    return ( isVisible() == r.isVisible() &&
	     markerStylesMap() == r.markerStylesMap() &&
	     markerStyle() == r.markerStyle() &&
	     markerSize() == r.markerSize() &&
	     markerColor() == r.markerColor() &&
             pen() == r.pen() );
}



void MarkerAttributes::setVisible( bool visible )
{
    d->visible = visible;
}

bool MarkerAttributes::isVisible() const
{
    return d->visible;
}

void MarkerAttributes::setMarkerStylesMap( MarkerStylesMap map )
{
    d->markerStylesMap = map;
}

MarkerAttributes::MarkerStylesMap MarkerAttributes::markerStylesMap() const
{
    return d->markerStylesMap;
}

void MarkerAttributes::setMarkerStyle( const MarkerStyle style )
{
    d->markerStyle = style;
}

MarkerAttributes::MarkerStyle MarkerAttributes::markerStyle() const
{
    return d->markerStyle;
}

void MarkerAttributes::setMarkerSize( const QSizeF& size )
{
    d->markerSize = size;
}

QSizeF MarkerAttributes::markerSize() const
{
    return d->markerSize;
}

void MarkerAttributes::setMarkerColor( const QColor& color )
{
    d->markerColor = color;
}

QColor MarkerAttributes::markerColor() const
{
    return d->markerColor;
}

void MarkerAttributes::setPen( const QPen& pen )
{
    d->markerPen = pen;
}

QPen MarkerAttributes::pen() const
{
    return d->markerPen;
}

QDomDocumentFragment MarkerAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment MarkerAttributes::toXML() const" );
    return QDomDocumentFragment();
}






































