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
