/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef KDCHARTNORMALLINEDIAGRAM_P_H
#define KDCHARTNORMALLINEDIAGRAM_P_H

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
#include "KDChartLineDiagram_p.h"

namespace KDChart {

class NormalLineDiagram : public LineDiagram::LineDiagramType
{
public:
    explicit NormalLineDiagram(LineDiagram *);
    ~NormalLineDiagram() override
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
