/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTBARATTRIBUTES_H
#define KDCHARTBARATTRIBUTES_H

#include "KDChartGlobal.h"
#include <QMetaType>

namespace KDChart {

/**
 * @brief Set of attributes for changing the appearance of bar charts
 */
class KDCHART_EXPORT BarAttributes
{
public:
    BarAttributes();
    BarAttributes(const BarAttributes &);
    BarAttributes &operator=(const BarAttributes &);

    ~BarAttributes();

    void setFixedDataValueGap(qreal gap);
    qreal fixedDataValueGap() const;

    void setUseFixedDataValueGap(bool gapIsFixed);
    bool useFixedDataValueGap() const;

    void setFixedValueBlockGap(qreal gap);
    qreal fixedValueBlockGap() const;

    void setUseFixedValueBlockGap(bool gapIsFixed);
    bool useFixedValueBlockGap() const;

    void setFixedBarWidth(qreal width);
    qreal fixedBarWidth() const;

    void setUseFixedBarWidth(bool useFixedBarWidth);
    bool useFixedBarWidth() const;

    void setGroupGapFactor(qreal gapFactor);
    qreal groupGapFactor() const;

    void setBarGapFactor(qreal gapFactor);
    qreal barGapFactor() const;

    void setDrawSolidExcessArrows(bool solidArrows);
    bool drawSolidExcessArrows() const;

    bool operator==(const BarAttributes &) const;
    inline bool operator!=(const BarAttributes &other) const
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
}; // End of class BarAttributes
}

Q_DECLARE_METATYPE(KDChart::BarAttributes)

#endif // KDCHARTBARATTRIBUTES_H
