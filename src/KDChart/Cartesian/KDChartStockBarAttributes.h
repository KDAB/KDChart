/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTSTOCKBARATTRIBUTES_H
#define KDCHARTSTOCKBARATTRIBUTES_H

#include "KDChartGlobal.h"
#include <QMetaType>

namespace KDChart {

/**
 * @brief Attributes to customize the appearance of a column in a stock chart
 */
class KDCHART_EXPORT StockBarAttributes
{
public:
    StockBarAttributes();
    StockBarAttributes(const StockBarAttributes &);
    StockBarAttributes &operator=(const StockBarAttributes &);

    ~StockBarAttributes();

    void setCandlestickWidth(qreal width);
    qreal candlestickWidth() const;

    void setTickLength(qreal length);
    qreal tickLength() const;

    bool operator==(const StockBarAttributes &) const;
    inline bool operator!=(const StockBarAttributes &other) const
    {
        return !operator==(other);
    }

private:
    class Private;
    Private *_d;
    Private *d_func()
    {
        return _d;
    }
    const Private *d_func() const
    {
        return _d;
    }
};
}

Q_DECLARE_METATYPE(KDChart::StockBarAttributes)

#endif // KDCHARTSTOCKBARATTRIBUTES_H
