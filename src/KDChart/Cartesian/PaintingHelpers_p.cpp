/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "PaintingHelpers_p.h"

#include "KDChartGlobal.h"

#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractDiagram_p.h"
#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartLineDiagram.h"
#include "KDChartLineDiagram_p.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartPlotter.h"
#include "KDChartPrintingParameters.h"
#include "KDChartThreeDLineAttributes.h"
#include "KDChartValueTrackerAttributes.h"
#include "ReverseMapper.h"

namespace KDChart {
namespace PaintingHelpers {

/*!
Projects a point in a space defined by its x, y, and z coordinates
into a point on a plane, given two rotation angles around the x
resp. y axis.
*/
const QPointF project(const QPointF &point, const ThreeDLineAttributes &tdAttributes)
{
    // Pending Michel FIXME - the rotation does not work as expected atm
    qreal xrad = DEGTORAD(tdAttributes.lineXRotation());
    qreal yrad = DEGTORAD(tdAttributes.lineYRotation());
    return QPointF(point.x() * cos(yrad) + tdAttributes.depth() * sin(yrad),
                   point.y() * cos(xrad) - tdAttributes.depth() * sin(xrad));
}

QPainterPath fitPoints(const QPolygonF &points, qreal tension, SplineDirection splineDirection)
{
    QPainterPath path;
    path.moveTo(points.at(0));
    const int count = points.size();

    // TODO: convert to lambda when we stop caring about pre-C++11
    // auto dataAt = [&] (int i) {
    //     return i < 0 || i >= count ? QPointF(NAN, NAN) : points.at( i );
    // };
    struct dataAtLambda
    {
        dataAtLambda(const QPolygonF &points, int count)
            : points(points)
            , count(count)
        {
        }

        const QPolygonF &points;
        int count;

        QPointF operator()(int i) const
        {
            return i < 0 || i >= count ? QPointF(NAN, NAN) : points.at(i);
        }
    };

    dataAtLambda dataAt(points, count);

    for (int i = 1; i < count; ++i) {
        addSplineChunkTo(path, tension, dataAt(i - 2), points.at(i - 1), points.at(i), dataAt(i + 1), splineDirection);
    }

    return path;
}

void paintPolyline(PaintContext *ctx, const QBrush &brush, const QPen &pen, const QPolygonF &points)
{
    ctx->painter()->setBrush(brush);
    ctx->painter()->setPen(PrintingParameters::scalePen(
        QPen(pen.color(), pen.width(), pen.style(), Qt::FlatCap, Qt::MiterJoin)));
    ctx->painter()->drawPolyline(points);
}

void paintSpline(PaintContext *ctx, const QBrush &brush, const QPen &pen, const QPolygonF &points, qreal tension, SplineDirection splineDirection)
{
    if (points.size() < 3) {
        paintPolyline(ctx, brush, pen, points);
        return;
    }

    ctx->painter()->setBrush(brush);
    ctx->painter()->setBrush(QBrush());
    ctx->painter()->setPen(PrintingParameters::scalePen(
        QPen(pen.color(), pen.width(), pen.style(), Qt::FlatCap, Qt::MiterJoin)));

    ctx->painter()->drawPath(fitPoints(points, tension, splineDirection));
}

void paintThreeDLines(PaintContext *ctx, AbstractDiagram *diagram, const QModelIndex &index,
                      const QPointF &from, const QPointF &to, const ThreeDLineAttributes &tdAttributes,
                      ReverseMapper *reverseMapper)
{
    const QPointF topLeft = project(from, tdAttributes);
    const QPointF topRight = project(to, tdAttributes);
    const QPolygonF segment = QPolygonF() << from << topLeft << topRight << to;

    QBrush indexBrush(diagram->brush(index));
    indexBrush = tdAttributes.threeDBrush(indexBrush, QRectF(topLeft, topRight));

    const PainterSaver painterSaver(ctx->painter());

    ctx->painter()->setRenderHint(QPainter::Antialiasing, diagram->antiAliasing());
    ctx->painter()->setBrush(indexBrush);
    ctx->painter()->setPen(PrintingParameters::scalePen(diagram->pen(index)));

    reverseMapper->addPolygon(index.row(), index.column(), segment);
    ctx->painter()->drawPolygon(segment);
}

void paintValueTracker(PaintContext *ctx, const ValueTrackerAttributes &vt, const QPointF &at)
{
    auto *plane = qobject_cast<CartesianCoordinatePlane *>(ctx->coordinatePlane());
    if (!plane)
        return;

    DataDimensionsList gridDimensions = ctx->coordinatePlane()->gridDimensionsList();
    const QPointF bottomLeft(ctx->coordinatePlane()->translate(
        QPointF(plane->isHorizontalRangeReversed() ? gridDimensions.at(0).end : gridDimensions.at(0).start,
                plane->isVerticalRangeReversed() ? gridDimensions.at(1).end : gridDimensions.at(1).start)));
    const QPointF topRight(ctx->coordinatePlane()->translate(
        QPointF(plane->isHorizontalRangeReversed() ? gridDimensions.at(0).start : gridDimensions.at(0).end,
                plane->isVerticalRangeReversed() ? gridDimensions.at(1).start : gridDimensions.at(1).end)));
    const QPointF markerPoint = at;

    QPointF startPoint;
    if (vt.orientations() & Qt::Horizontal) {
        startPoint = QPointF(bottomLeft.x(), at.y());
    } else {
        startPoint = QPointF(at.x(), topRight.y());
    }

    QPointF endPoint;
    if (vt.orientations() & Qt::Vertical) {
        endPoint = QPointF(at.x(), bottomLeft.y());
    } else {
        endPoint = QPointF(topRight.x(), at.y());
    }

    const QSizeF markerSize = vt.markerSize();
    const QRectF ellipseMarker = QRectF(at.x() - markerSize.width() / 2,
                                        at.y() - markerSize.height() / 2,
                                        markerSize.width(), markerSize.height());

    QPointF startMarker[3];
    if (vt.orientations() & Qt::Horizontal) {
        startMarker[0] = startPoint + QPointF(0, markerSize.height() / 2);
        startMarker[1] = startPoint + QPointF(markerSize.width() / 2, 0);
        startMarker[2] = startPoint - QPointF(0, markerSize.height() / 2);
    } else {
        startMarker[0] = startPoint + QPointF(0, markerSize.height() / 2);
        startMarker[1] = startPoint + QPointF(markerSize.width() / 2, 0);
        startMarker[2] = startPoint - QPointF(markerSize.width() / 2, 0);
    }

    QPointF endMarker[3];

    if (vt.orientations() & Qt::Vertical) {
        endMarker[0] = endPoint + QPointF(markerSize.width() / 2, 0);
        endMarker[1] = endPoint - QPointF(0, markerSize.height() / 2);
        endMarker[2] = endPoint - QPointF(markerSize.width() / 2, 0);
    } else {
        endMarker[0] = endPoint + QPointF(0, markerSize.width() / 2);
        endMarker[1] = endPoint - QPointF(0, markerSize.height() / 2);
        endMarker[2] = endPoint - QPointF(markerSize.width() / 2, 0);
    }

    QPointF topLeft = startPoint;
    QPointF bottomRightOffset = endPoint - topLeft;
    QSizeF size(bottomRightOffset.x(), bottomRightOffset.y());
    QRectF area(topLeft, size);

    PainterSaver painterSaver(ctx->painter());
    ctx->painter()->setPen(PrintingParameters::scalePen(vt.linePen()));
    ctx->painter()->setBrush(QBrush());
    ctx->painter()->drawLine(markerPoint, startPoint);
    ctx->painter()->drawLine(markerPoint, endPoint);

    ctx->painter()->fillRect(area, vt.areaBrush());

    ctx->painter()->setPen(PrintingParameters::scalePen(vt.markerPen()));
    ctx->painter()->setBrush(vt.markerBrush());
    ctx->painter()->drawEllipse(ellipseMarker);

    ctx->painter()->setPen(PrintingParameters::scalePen(vt.arrowBrush().color()));
    ctx->painter()->setBrush(vt.arrowBrush());
    ctx->painter()->drawPolygon(startMarker, 3);
    ctx->painter()->drawPolygon(endMarker, 3);
}

// ### for BC reasons we cannot insert a common interface for LineDiagram and Plotter into the class
//     hierarchy, so we have to use hacks to use their common methods
static ThreeDLineAttributes threeDLineAttributes(AbstractDiagram *diagram, const QModelIndex &index)
{
    if (auto *plotter = qobject_cast<Plotter *>(diagram)) {
        return plotter->threeDLineAttributes(index);
    } else if (auto *lineDiagram = qobject_cast<LineDiagram *>(diagram)) {
        return lineDiagram->threeDLineAttributes(index);
    }
    Q_ASSERT(false);
    return ThreeDLineAttributes();
}

static ValueTrackerAttributes valueTrackerAttributes(AbstractDiagram *diagram, const QModelIndex &index)
{
    if (auto *plotter = qobject_cast<Plotter *>(diagram)) {
        return plotter->valueTrackerAttributes(index);
    } else if (auto *lineDiagram = qobject_cast<LineDiagram *>(diagram)) {
        return lineDiagram->valueTrackerAttributes(index);
    }
    Q_ASSERT(false);
    return ValueTrackerAttributes();
}

void paintObject(AbstractDiagram::Private *diagramPrivate, PaintContext *ctx, const QBrush &brush, const QPen &pen, const QPolygonF &points)
{
    qreal tension = 0;
    SplineDirection splineDirection = NormalSplineDirection;

    if (auto *lineDiagram = dynamic_cast<LineDiagram::Private *>(diagramPrivate)) {
        tension = lineDiagram->tension;
        Q_ASSERT(dynamic_cast<CartesianCoordinatePlane *>(ctx->coordinatePlane()));
        const auto plane = static_cast<CartesianCoordinatePlane *>(ctx->coordinatePlane());
        splineDirection = plane->isHorizontalRangeReversed() ? ReverseSplineDirection : NormalSplineDirection;
    }

    if (qFuzzyIsNull(tension)) {
        paintPolyline(ctx, brush, pen, points);
    } else {
        paintSpline(ctx, brush, pen, points, tension, splineDirection);
    }
}

void paintElements(AbstractDiagram::Private *diagramPrivate, PaintContext *ctx,
                   const LabelPaintCache &lpc, const LineAttributesInfoList &lineList)
{
    AbstractDiagram *diagram = diagramPrivate->diagram;
    // paint all lines and their attributes
    const PainterSaver painterSaver(ctx->painter());
    ctx->painter()->setRenderHint(QPainter::Antialiasing, diagram->antiAliasing());

    QBrush curBrush;
    QPen curPen;
    QPolygonF points;
    Q_FOREACH (const LineAttributesInfo &lineInfo, lineList) {
        const QModelIndex &index = lineInfo.index;
        const ThreeDLineAttributes td = threeDLineAttributes(diagram, index);

        if (td.isEnabled()) {
            PaintingHelpers::paintThreeDLines(ctx, diagram, index, lineInfo.value,
                                              lineInfo.nextValue, td, &diagramPrivate->reverseMapper);
        } else {
            const QBrush brush(diagram->brush(index));
            const QPen pen(diagram->pen(index));

            // line goes from lineInfo.value to lineInfo.nextValue
            // We don't want it added if we're not drawing it, since the reverse mapper is used
            // for lookup when trying to find e.g. tooltips. Having the line added when invisible gives
            // us tooltips in empty areas.
            if (pen.style() != Qt::NoPen)
                diagramPrivate->reverseMapper.addLine(lineInfo.index.row(), lineInfo.index.column(),
                                                      lineInfo.value, lineInfo.nextValue);

            if (points.count() && points.last() == lineInfo.value && curBrush == brush && curPen == pen) {
                // continue the current run of lines
            } else {
                // different painter settings or discontinuous line: start a new run of lines
                if (points.count()) {
                    paintObject(diagramPrivate, ctx, curBrush, curPen, points);
                }
                curBrush = brush;
                curPen = pen;
                points.clear();
                points << lineInfo.value;
            }
            points << lineInfo.nextValue;
        }
    }
    if (points.count()) {
        // the last run of lines is yet to be painted - do it now
        paintObject(diagramPrivate, ctx, curBrush, curPen, points);
    }

    Q_FOREACH (const LineAttributesInfo &lineInfo, lineList) {
        const ValueTrackerAttributes vt = valueTrackerAttributes(diagram, lineInfo.index);
        if (vt.isEnabled()) {
            PaintingHelpers::paintValueTracker(ctx, vt, lineInfo.nextValue);
        }
    }

    // paint all data value texts and the point markers
    diagramPrivate->paintDataValueTextsAndMarkers(ctx, lpc, true);
}

void paintAreas(AbstractDiagram::Private *diagramPrivate, PaintContext *ctx, const QModelIndex &index,
                const QList<QPolygonF> &areas, uint opacity)
{
    AbstractDiagram *diagram = diagramPrivate->diagram;
    QPainterPath path;
    for (int i = 0; i < areas.count(); ++i) {
        const QPolygonF &p = areas[i];
        path.addPolygon(p);
        diagramPrivate->reverseMapper.addPolygon(index.row(), index.column(), p);
        path.closeSubpath();
    }

    ThreeDLineAttributes threeDAttrs = threeDLineAttributes(diagram, index);
    QBrush trans = diagram->brush(index);
    if (threeDAttrs.isEnabled()) {
        trans = threeDAttrs.threeDBrush(trans, path.boundingRect());
    }
    QColor transColor = trans.color();
    transColor.setAlpha(opacity);
    trans.setColor(transColor);
    QPen indexPen = diagram->pen(index);
    indexPen.setBrush(trans);
    const PainterSaver painterSaver(ctx->painter());

    ctx->painter()->setRenderHint(QPainter::Antialiasing, diagram->antiAliasing());
    ctx->painter()->setPen(PrintingParameters::scalePen(indexPen));
    ctx->painter()->setBrush(trans);

    ctx->painter()->drawPath(path);
}

void paintAreas(AbstractDiagram::Private *diagramPrivate, PaintContext *ctx, const QModelIndex &index,
                const QList<QPainterPath> &areas, uint opacity)
{
    AbstractDiagram *diagram = diagramPrivate->diagram;
    QPainterPath path;
    for (int i = 0; i < areas.count(); ++i) {
        path += areas[i];
        // diagramPrivate->reverseMapper.addPolygon( index.row(), index.column(), p );
    }

    QBrush trans = diagram->brush(index);
    QColor transColor = trans.color();
    transColor.setAlpha(opacity);
    trans.setColor(transColor);
    QPen indexPen = diagram->pen(index);
    indexPen.setBrush(trans);
    const PainterSaver painterSaver(ctx->painter());

    ctx->painter()->setRenderHint(QPainter::Antialiasing, diagram->antiAliasing());
    ctx->painter()->setPen(PrintingParameters::scalePen(indexPen));
    ctx->painter()->setBrush(trans);

    ctx->painter()->drawPath(path);
}

} // namespace PaintingHelpers
} // namespace KDChart
