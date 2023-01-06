/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTSTACKEDLINEDIAGRAM_P_H
#define KDCHARTSTACKEDLINEDIAGRAM_P_H

#include "KDChartLineDiagram_p.h"

namespace KDChart {

class StackedLineDiagram : public LineDiagram::LineDiagramType
{
public:
    explicit StackedLineDiagram(LineDiagram *);
    ~StackedLineDiagram() override
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
