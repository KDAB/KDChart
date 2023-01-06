/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTSTACKEDLYINGBARDIAGRAM_P_H
#define KDCHARTSTACKEDLYINGBARDIAGRAM_P_H

#include "KDChartBarDiagram_p.h"

namespace KDChart {

class StackedLyingBarDiagram : public BarDiagram::BarDiagramType
{
public:
    explicit StackedLyingBarDiagram(BarDiagram *);
    ~StackedLyingBarDiagram() override
    {
    }
    BarDiagram::BarType type() const override;
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paint(PaintContext *ctx) override;
};
}

#endif
