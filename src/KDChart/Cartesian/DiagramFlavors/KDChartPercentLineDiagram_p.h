/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
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
