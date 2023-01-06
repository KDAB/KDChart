/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTTERNARYDIAGRAM_P_H
#define KDCHARTABSTRACTTERNARYDIAGRAM_P_H

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

#include "KDChartAbstractTernaryDiagram.h"

#include "KDChartPainterSaver_p.h"
#include "KDChartTernaryCoordinatePlane.h"
#include <KDChartAbstractDiagram_p.h>
#include <KDChartAbstractThreeDAttributes.h>
#include <KDChartGridAttributes.h>

#include "ChartGraphicsItem.h"
#include "ReverseMapper.h"
#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class AbstractTernaryDiagram::Private : public AbstractDiagram::Private
{
    friend class AbstractTernaryDiagram;

public:
    Private();
    ~Private() override
    {
    }

    Private(const Private &rhs)
        : AbstractDiagram::Private(rhs)
        ,
        // Do not copy axes and reference diagrams.
        axesList()
        , referenceDiagram(nullptr)
        , referenceDiagramOffset()
    {
    }

    TernaryAxisList axesList;

    AbstractTernaryDiagram *referenceDiagram;
    QPointF referenceDiagramOffset;

    void drawPoint(QPainter *p, int row, int column,
                   const QPointF &widgetLocation)
    {
        // Q_ASSERT( false ); // unused, to be removed
        static const qreal Diameter = 5.0;
        static const qreal Radius = Diameter / 2.0;
        QRectF ellipseRect(widgetLocation - QPointF(Radius, Radius),
                           QSizeF(Diameter, Diameter));
        p->drawEllipse(ellipseRect);

        reverseMapper.addRect(row, column, ellipseRect);
    }

    virtual void paint(PaintContext *paintContext)
    {
        paintContext->painter()->setRenderHint(QPainter::Antialiasing,
                                               antiAliasing);
        if (!axesList.isEmpty()) {

            Q_FOREACH (TernaryAxis *axis, axesList) {
                PainterSaver s(paintContext->painter());
                axis->paintCtx(paintContext);
            }
        }
    }
};

KDCHART_IMPL_DERIVED_DIAGRAM(AbstractTernaryDiagram, AbstractDiagram, TernaryCoordinatePlane)
}

#endif /* KDCHARTABSTRACTTERNARYDIAGRAM_P_H */
