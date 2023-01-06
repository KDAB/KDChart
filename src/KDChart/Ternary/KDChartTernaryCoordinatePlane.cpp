/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartTernaryCoordinatePlane_p.h"

#include <QPainter>
#include <QtDebug>

#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartTernaryAxis.h"

#include "TernaryConstants.h"

using namespace KDChart;

#define d d_func()

TernaryCoordinatePlane::Private::Private()
    : AbstractCoordinatePlane::Private()
{
}

TernaryCoordinatePlane::TernaryCoordinatePlane(Chart *parent)
    : AbstractCoordinatePlane(new Private(), parent)
{
}

TernaryCoordinatePlane::~TernaryCoordinatePlane()
{
}

void TernaryCoordinatePlane::init()
{
}

void TernaryCoordinatePlane::addDiagram(AbstractDiagram *diagram)
{
    Q_ASSERT_X(dynamic_cast<AbstractTernaryDiagram *>(diagram),
               "TernaryCoordinatePlane::addDiagram", "Only ternary "
                                                     "diagrams can be added to a ternary coordinate plane!");
    AbstractCoordinatePlane::addDiagram(diagram);
}

void TernaryCoordinatePlane::layoutDiagrams()
{ // this is our "resize event":
    // all diagrams always take the same space, nothing to be done here
    // the "inner" margin (adjustments to diagram coordinates)
    QRectF diagramNativeRectangle(QPointF(0.0, 0.0),
                                  QSizeF(TriangleWidth, TriangleHeight));
    QPair<QSizeF, QSizeF> margins = grid()->requiredMargins();
    d->diagramRect = areaGeometry();
    diagramNativeRectangle.adjust(-margins.first.width(), -margins.first.height(),
                                  margins.second.width(), margins.second.height());

    // the "outer" margin (distance between diagram contents and area,
    // determined by axis label overlap
    {
        QSizeF topleft(0.0, 0.0);
        QSizeF bottomRight(0.0, 0.0);
        Q_FOREACH (AbstractDiagram *abstractDiagram, diagrams()) {
            auto *diagram =
                qobject_cast<AbstractTernaryDiagram *>(abstractDiagram);
            Q_ASSERT(diagram);
            Q_FOREACH (TernaryAxis *axis, diagram->axes()) {
                QPair<QSizeF, QSizeF> margin = axis->requiredMargins();
                topleft = topleft.expandedTo(margin.first);
                bottomRight = bottomRight.expandedTo(margin.second);
            }
        }
        d->diagramRectContainer =
            d->diagramRect.adjusted(topleft.width(),
                                    topleft.height(),
                                    -bottomRight.width(),
                                    -bottomRight.height());
    }

    // now calculate isometric projection, x and y widget coordinate
    // units, and location of (0.0, 0.0) in diagram coordinates
    QPointF zeroZeroPoint = d->diagramRectContainer.bottomLeft();
    qreal w = d->diagramRectContainer.width();
    qreal h = d->diagramRectContainer.height();
    qreal usableWidth;
    qreal usableHeight;

    if (TriangleHeight * w > h) {
        // shorten width:
        usableWidth = h / diagramNativeRectangle.height();
        usableHeight = h;
        zeroZeroPoint.setX(zeroZeroPoint.x() + (w - usableWidth) / 2);
    } else {
        // reduce height:
        usableWidth = w;
        usableHeight = diagramNativeRectangle.height() * w;
        zeroZeroPoint.setY(zeroZeroPoint.y() - (h - usableHeight) / 2);
    }
    // the rectangle has 1 as it's width, and TriangleHeight as it's
    // height - so this is how we translate that to widget coordinates:
    d->xUnit = usableWidth / diagramNativeRectangle.width(); // only because we normalize the values to [0..1]
    d->yUnit = -usableHeight / diagramNativeRectangle.height();

    // now move zeroZeroPoint so that it does not include the tick marks
    {
        qreal descent = diagramNativeRectangle.height() - TriangleHeight;
        qreal rightShift = -diagramNativeRectangle.x();
        zeroZeroPoint += QPointF(rightShift * d->xUnit, descent * d->yUnit);
    }

    d->diagramRect.setBottomLeft(zeroZeroPoint);
    d->diagramRect.setTopRight(QPointF(usableWidth, -usableHeight) + zeroZeroPoint);
}

const QPointF TernaryCoordinatePlane::translate(const QPointF &point) const
{
    return QPointF(d->diagramRect.bottomLeft().x() + point.x() * d->xUnit,
                   d->diagramRect.bottomLeft().y() + point.y() * d->yUnit);
}

QSize TernaryCoordinatePlane::minimumSizeHint() const
{
    // FIXME temp
    return QSize();
}

QSizePolicy TernaryCoordinatePlane::sizePolicy() const
{
    return QSizePolicy(QSizePolicy::MinimumExpanding,
                       QSizePolicy::MinimumExpanding);
}

void TernaryCoordinatePlane::paint(QPainter *painter)
{
    PainterSaver s(painter);
    // FIXME: this is not a good location for that:
    painter->setRenderHint(QPainter::Antialiasing, true);

    AbstractDiagramList diags = diagrams();
    if (!diags.isEmpty()) {
        PaintContext ctx;
        ctx.setPainter(painter);
        ctx.setCoordinatePlane(this);
        const QRectF drawArea(areaGeometry());
        ctx.setRectangle(drawArea);

        // paint the coordinate system rulers:
        Q_ASSERT(d->grid != nullptr);
        d->grid->drawGrid(&ctx);

        // paint the diagrams:
        for (int i = 0; i < diags.size(); i++) {
            PainterSaver diagramPainterSaver(painter);
            diags[i]->paint(&ctx);
        }
    }
}

DataDimensionsList TernaryCoordinatePlane::getDataDimensionsList() const
{ // not needed
    return DataDimensionsList();
}

TernaryGrid *TernaryCoordinatePlane::grid() const
{
    auto *ternaryGrid = static_cast<TernaryGrid *>(d->grid);
    Q_ASSERT(dynamic_cast<TernaryGrid *>(d->grid));
    return ternaryGrid;
}

#undef d
