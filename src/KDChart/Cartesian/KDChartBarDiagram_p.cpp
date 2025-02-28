/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartBarDiagram_p.h"
#include "KDChartBarDiagram.h"

#include "KDChartDataValueAttributes.h"
#include "KDChartPainterSaver_p.h"

using namespace KDChart;

BarDiagram::Private::Private(const Private &rhs)
    : AbstractCartesianDiagram::Private(rhs)
{
}

void BarDiagram::BarDiagramType::paintBars(PaintContext *ctx, const QModelIndex &index, const QRectF &bar, qreal maxDepth)
{
    PainterSaver painterSaver(ctx->painter());

    // Pending Michel: configure threeDBrush settings - shadowColor etc...
    QBrush indexBrush(diagram()->brush(index));
    QPen indexPen(diagram()->pen(index));

    ctx->painter()->setRenderHint(QPainter::Antialiasing, diagram()->antiAliasing());
    ThreeDBarAttributes threeDAttrs = diagram()->threeDBarAttributes(index);
    if (threeDAttrs.isEnabled()) {
        indexBrush = threeDAttrs.threeDBrush(indexBrush, bar);
    }
    ctx->painter()->setBrush(indexBrush);
    ctx->painter()->setPen(PrintingParameters::scalePen(indexPen));

    if (threeDAttrs.isEnabled()) {
        if (maxDepth) {
            threeDAttrs.setDepth(-maxDepth);
        }
        // fixme adjust the painting to reasonable depth value
        const qreal usedDepth = threeDAttrs.depth() * (type() == BarDiagram::Normal ? 0.25 : 1.0);

        const QRectF isoRect = bar.translated(usedDepth, -usedDepth);
        // we need to find out if the height is negative
        // and in this case paint it up and down
        QPolygonF topPoints;
        if (isoRect.height() < 0) {
            if (!(type() == BarDiagram::Stacked && index.column() != 0)) {
                // fix it when several negative stacked values
                topPoints << isoRect.bottomLeft() << isoRect.bottomRight()
                          << bar.bottomRight() << bar.bottomLeft();
            }
        } else {
            reverseMapper().addRect(index.row(), index.column(), isoRect);
            ctx->painter()->drawRect(isoRect);
            if (!(type() == BarDiagram::Percent && isoRect.height() == 0)) {
                topPoints << bar.topLeft() << bar.topRight() << isoRect.topRight() << isoRect.topLeft();
            }
        }

        bool noClippingForTop = false;
        if (!topPoints.isEmpty()) {
            // Draw the top, if at least one of the top's points is
            // either inside or near at the edge of the coordinate plane:
            bool drawIt = false;
            bool hasPointOutside = false;
            const QRectF r(ctx->rectangle().adjusted(0, -1, 1, 0));
            for (QPointF pt : std::as_const(topPoints)) {
                if (r.contains(pt)) {
                    drawIt = true;
                } else {
                    hasPointOutside = true;
                }
            }
            if (drawIt) {
                const PainterSaver p(ctx->painter());
                noClippingForTop = hasPointOutside && ctx->painter()->hasClipping();
                if (noClippingForTop) {
                    ctx->painter()->setClipping(false);
                }
                reverseMapper().addPolygon(index.row(), index.column(), topPoints);
                ctx->painter()->drawPolygon(topPoints);
            }
        }

        if (bar.height() != 0) {
            const PainterSaver p(ctx->painter());
            if (noClippingForTop) {
                ctx->painter()->setClipping(false);
            }
            QPolygonF sidePoints;
            sidePoints << bar.topRight() << isoRect.topRight()
                       << isoRect.bottomRight() << bar.bottomRight();
            reverseMapper().addPolygon(index.row(), index.column(), sidePoints);
            ctx->painter()->drawPolygon(sidePoints);
        }
    }

    if (bar.height() != 0) {
        reverseMapper().addRect(index.row(), index.column(), bar);
        ctx->painter()->drawRect(bar);
    }
}

AttributesModel *BarDiagram::BarDiagramType::attributesModel() const
{
    return m_private->attributesModel;
}

QModelIndex BarDiagram::BarDiagramType::attributesModelRootIndex() const
{
    return diagram()->attributesModelRootIndex();
}

BarDiagram *BarDiagram::BarDiagramType::diagram() const
{
    return static_cast<BarDiagram *>(m_private->diagram);
}

void BarDiagram::BarDiagramType::calculateValueAndGapWidths(int rowCount, int colCount,
                                                            qreal groupWidth,
                                                            qreal &outBarWidth,
                                                            qreal &outSpaceBetweenBars,
                                                            qreal &outSpaceBetweenGroups)
{
    Q_UNUSED(rowCount);
    BarAttributes ba = diagram()->barAttributes();

    // Pending Michel Fixme
    /* We are colCount groups to paint. Each group is centered around the
     * horizontal point position on the grid. The full area covers the
     * values -1 to colCount + 1. A bar has a relative width of one unit,
     * the gaps between bars are 0.5 wide, and the gap between groups is
     * also one unit, by default. */

    qreal units;
    if (type() == Normal) {
        units = colCount // number of bars in group * 1.0
            + (colCount - 1) * ba.barGapFactor() // number of bar gaps
            + 1 * ba.groupGapFactor(); // number of group gaps
    } else {
        units = 1 + 1 * ba.groupGapFactor();
    }

    qreal unitWidth = groupWidth / units;

    if (!ba.useFixedBarWidth()) {
        outBarWidth = unitWidth;
    }

    outSpaceBetweenBars += unitWidth * ba.barGapFactor();

    // Pending Michel - minLimit: allow space between bars to be reduced until the bars are displayed next to each other.
    // is that what we want?
    // sebsauer; in the case e.g. CartesianCoordinatePlane::setHorizontalRangeReversed(true) was
    // used to reverse the values, we deal with negative outSpaceBetweenBars and unitWidth here
    // and since that's correct we don't like to lose e.g. the spacing here.
    // if ( outSpaceBetweenBars < 0 )
    //    outSpaceBetweenBars = 0;

    outSpaceBetweenGroups += unitWidth * ba.groupGapFactor();
}

ReverseMapper &BarDiagram::BarDiagramType::reverseMapper()
{
    return m_private->reverseMapper;
}

CartesianDiagramDataCompressor &BarDiagram::BarDiagramType::compressor() const
{
    return m_private->compressor;
}
