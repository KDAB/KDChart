/****************************************************************************
 ** Copyright (C) 2008 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartStockBarAttributes.h"

#define d d_func()

using namespace KDChart;

class StockBarAttributes::Private {
public:
    Private();

    qreal candlestickWidth;
    qreal tickLength;
};

StockBarAttributes::Private::Private()
    : candlestickWidth( 0.3 )
    , tickLength( 0.15 )
{
}

StockBarAttributes::StockBarAttributes()
    : _d( new Private )
{
}

StockBarAttributes::StockBarAttributes( const StockBarAttributes& r )
    : _d( new Private( *r.d ) )
{
}

StockBarAttributes &StockBarAttributes::operator= ( const StockBarAttributes& r )
{
    if ( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

StockBarAttributes::~StockBarAttributes()
{
    delete _d;
}

/**
  * Sets the width of a candlestick
  *
  * @param width The width of a candlestick
  */
void StockBarAttributes::setCandlestickWidth( qreal width )
{
    d->candlestickWidth = width;
}
/**
  * @return the width of a candlestick
  */
qreal StockBarAttributes::candlestickWidth() const
{
    return d->candlestickWidth;
}

/**
  * Sets the tick length of both the open and close marker
  *
  * @param length the tick length
  */
void StockBarAttributes::setTickLength( qreal length )
{
    d->tickLength = length;
}

/**
  * @return the tick length used for both the open and close marker
  */
qreal StockBarAttributes::tickLength() const
{
    return d->tickLength;
}

bool StockBarAttributes::operator==( const StockBarAttributes& r ) const
{
    return candlestickWidth() == r.candlestickWidth() &&
           tickLength() == r.tickLength();
}
