/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTPERCENTPLOTTER_P_H
#define KDCHARTPERCENTPLOTTER_P_H

#include "KDChartPlotter_p.h"

namespace KDChart {

class PercentPlotter : public Plotter::PlotterType
{
public:
    explicit PercentPlotter(Plotter *);
    ~PercentPlotter() override
    {
    }
    Plotter::PlotType type() const override;
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paint(PaintContext *ctx) override;
};
}

#endif
