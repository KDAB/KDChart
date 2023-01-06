/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTNORMALLYINGBARDIAGRAM_P_H
#define KDCHARTNORMALLYINGBARDIAGRAM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
#include "KDChartBarDiagram_p.h"

namespace KDChart {

class NormalLyingBarDiagram : public BarDiagram::BarDiagramType
{
public:
    explicit NormalLyingBarDiagram(BarDiagram *);
    ~NormalLyingBarDiagram() override
    {
    }
    BarDiagram::BarType type() const override;
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paint(PaintContext *ctx) override;
};
}

#endif
