/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartStockBarAttributes.h"

#define d d_func()

using namespace KDChart;

class StockBarAttributes::Private
{
public:
    Private();

    qreal candlestickWidth = 0.3;
    qreal tickLength = 0.15;
};

StockBarAttributes::Private::Private()
{
}

StockBarAttributes::StockBarAttributes()
    : _d(new Private)
{
}

StockBarAttributes::StockBarAttributes(const StockBarAttributes &r)
    : _d(new Private(*r.d))
{
}

StockBarAttributes &StockBarAttributes::operator=(const StockBarAttributes &r)
{
    if (this == &r)
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
void StockBarAttributes::setCandlestickWidth(qreal width)
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
void StockBarAttributes::setTickLength(qreal length)
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

bool StockBarAttributes::operator==(const StockBarAttributes &r) const
{
    return candlestickWidth() == r.candlestickWidth() && tickLength() == r.tickLength();
}
