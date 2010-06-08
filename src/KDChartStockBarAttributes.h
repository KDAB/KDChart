#ifndef KDCHARTSTOCKBARATTRIBUTES_H
#define KDCHARTSTOCKBARATTRIBUTES_H

#include <QMetaType>
#include "KDChartGlobal.h"

namespace KDChart {

/**
  * @brief Attributes to customize the appearance of a column in a stock chart
  */
class KDCHART_EXPORT StockBarAttributes
{
public:
    StockBarAttributes();
    StockBarAttributes( const StockBarAttributes& );
    StockBarAttributes &operator= ( const StockBarAttributes& );

    ~StockBarAttributes();

    void setCandlestickWidth( qreal width );
    qreal candlestickWidth() const;

    void setTickLength( qreal length );
    qreal tickLength() const;

    bool operator==( const StockBarAttributes& ) const;
    inline bool operator!=( const StockBarAttributes& other ) const { return !operator==(other); }

private:
    class Private;
    Private * _d;
    Private * d_func() { return _d; }
    const Private * d_func() const { return _d; }
};

}

Q_DECLARE_METATYPE( KDChart::StockBarAttributes )

#endif // KDCHARTSTOCKBARATTRIBUTES_H
