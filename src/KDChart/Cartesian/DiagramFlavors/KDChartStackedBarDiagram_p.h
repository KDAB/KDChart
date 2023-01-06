/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTSTACKEDBARDIAGRAM_P_H
#define KDCHARTSTACKEDBARDIAGRAM_P_H

#include "KDChartBarDiagram_p.h"

namespace KDChart {

class StackedBarDiagram : public BarDiagram::BarDiagramType
{
public:
    explicit StackedBarDiagram(BarDiagram *);
    ~StackedBarDiagram() override
    {
    }
    BarDiagram::BarType type() const override;
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paint(PaintContext *ctx) override;
};
}

#endif
