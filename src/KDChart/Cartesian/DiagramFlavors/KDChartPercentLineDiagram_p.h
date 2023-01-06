/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTPERCENTLINEDIAGRAM_P_H
#define KDCHARTPERCENTLINEDIAGRAM_P_H

#include "KDChartLineDiagram_p.h"

namespace KDChart {

class PercentLineDiagram : public LineDiagram::LineDiagramType
{
public:
    explicit PercentLineDiagram(LineDiagram *);
    ~PercentLineDiagram() override
    {
    }
    LineDiagram::LineType type() const override;
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paint(PaintContext *ctx) override;

private:
    void paintWithLines(PaintContext *ctx);
    void paintWithSplines(PaintContext *ctx, qreal tension);
};
}

#endif
