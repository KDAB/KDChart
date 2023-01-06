/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <limits>

#include <QAbstractItemModel>

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAttributesModel.h"
#include "KDChartBarDiagram.h"
#include "KDChartLineDiagram.h"
#include "KDChartNormalLineDiagram_p.h"
#include "KDChartTextAttributes.h"
#include "PaintingHelpers_p.h"

using namespace KDChart;
using namespace std;

NormalLineDiagram::NormalLineDiagram(LineDiagram *d)
    : LineDiagramType(d)
{
}

LineDiagram::LineType NormalLineDiagram::type() const
{
    return LineDiagram::Normal;
}

const QPair<QPointF, QPointF> NormalLineDiagram::calculateDataBoundaries() const
{
    return compressor().dataBoundaries();
}

void NormalLineDiagram::paint(PaintContext *ctx)
{
    if (qFuzzyIsNull(m_private->tension)) {
        paintWithLines(ctx);

    } else {
        paintWithSplines(ctx, m_private->tension);
    }
}

void NormalLineDiagram::paintWithLines(PaintContext *ctx)
{
    reverseMapper().clear();
    Q_ASSERT(dynamic_cast<CartesianCoordinatePlane *>(ctx->coordinatePlane()));
    auto *plane = static_cast<CartesianCoordinatePlane *>(ctx->coordinatePlane());
    const int columnCount = compressor().modelDataColumns();
    const int rowCount = compressor().modelDataRows();
    if (columnCount == 0 || rowCount == 0)
        return; // maybe blank out the area?

    // Reverse order of data sets?
    bool rev = diagram()->reverseDatasetOrder();
    LabelPaintCache lpc;
    LineAttributesInfoList lineList;

    const int step = rev ? -1 : 1;
    const int end = rev ? -1 : columnCount;
    for (int column = rev ? columnCount - 1 : 0; column != end; column += step) {
        LineAttributes laPreviousCell;
        CartesianDiagramDataCompressor::DataPoint lastPoint;
        qreal lastAreaBoundingValue = 0;

        // Get min. y value, used as lower or upper bounding for area highlighting
        const qreal minYValue = qMin(plane->visibleDataRange().bottom(), plane->visibleDataRange().top());

        CartesianDiagramDataCompressor::CachePosition previousCellPosition;
        for (int row = 0; row < rowCount; ++row) {
            const CartesianDiagramDataCompressor::CachePosition position(row, column);
            // get where to draw the line from:
            CartesianDiagramDataCompressor::DataPoint point = compressor().data(position);
            if (point.hidden) {
                continue;
            }

            const QModelIndex sourceIndex = attributesModel()->mapToSource(point.index);

            const LineAttributes laCell = diagram()->lineAttributes(sourceIndex);
            const LineAttributes::MissingValuesPolicy policy = laCell.missingValuesPolicy();

            // lower or upper bounding for the highlighted area
            qreal areaBoundingValue;
            if (laCell.areaBoundingDataset() != -1) {
                const CartesianDiagramDataCompressor::CachePosition areaBoundingCachePosition(row, laCell.areaBoundingDataset());
                areaBoundingValue = compressor().data(areaBoundingCachePosition).value;
            } else {
                // Use min. y value (i.e. zero line in most cases) if no bounding dataset is set
                areaBoundingValue = minYValue;
            }

            if (ISNAN(point.value)) {
                switch (policy) {
                case LineAttributes::MissingValuesAreBridged:
                    // we just bridge both values
                    continue;
                case LineAttributes::MissingValuesShownAsZero:
                    // set it to zero
                    point.value = 0.0;
                    break;
                case LineAttributes::MissingValuesHideSegments:
                    // they're just hidden
                    break;
                default:
                    break;
                    // hm....
                }
            }

            if (!ISNAN(point.value)) {
                // area corners, a + b are the line ends:
                const qreal offset = diagram()->centerDataPoints() ? 0.5 : 0;
                const QPointF a(plane->translate(QPointF(lastPoint.key + offset, lastPoint.value)));
                const QPointF b(plane->translate(QPointF(point.key + offset, point.value)));
                const QPointF c(plane->translate(QPointF(lastPoint.key + offset, lastAreaBoundingValue)));
                const QPointF d(plane->translate(QPointF(point.key + offset, areaBoundingValue)));
                const PositionPoints pts = PositionPoints(b, a, d, c);

                // add label
                m_private->addLabel(&lpc, sourceIndex, &position, pts, Position::NorthWest,
                                    Position::NorthWest, point.value);

                // add line and area, if switched on and we have a current and previous value
                if (!ISNAN(a.x()) && !ISNAN(a.y()) && !ISNAN(b.x()) && !ISNAN(b.y())) {
                    lineList.append(LineAttributesInfo(sourceIndex, a, b));

                    if (laCell.displayArea()) {
                        QList<QPolygonF> areas;
                        areas << (QPolygonF() << a << b << d << c);
                        PaintingHelpers::paintAreas(m_private, ctx, attributesModel()->mapToSource(lastPoint.index),
                                                    areas, laCell.transparency());
                    }
                }
            }

            previousCellPosition = position;
            laPreviousCell = laCell;
            lastAreaBoundingValue = areaBoundingValue;
            lastPoint = point;
        }
    }

    // paint the lines
    PaintingHelpers::paintElements(m_private, ctx, lpc, lineList);
}

void NormalLineDiagram::paintWithSplines(PaintContext *ctx, qreal tension)
{
    reverseMapper().clear();
    Q_ASSERT(dynamic_cast<CartesianCoordinatePlane *>(ctx->coordinatePlane()));
    auto *plane = static_cast<CartesianCoordinatePlane *>(ctx->coordinatePlane());
    const int columnCount = compressor().modelDataColumns();
    const int rowCount = compressor().modelDataRows();
    if (columnCount == 0 || rowCount == 0)
        return; // maybe blank out the area?

    // Reverse order of data sets?
    bool rev = diagram()->reverseDatasetOrder();
    LabelPaintCache lpc;
    LineAttributesInfoList lineList;

    const auto mainSplineDirection = plane->isHorizontalRangeReversed() ? ReverseSplineDirection : NormalSplineDirection;

    const int step = rev ? -1 : 1;
    const int end = rev ? -1 : columnCount;
    for (int column = rev ? columnCount - 1 : 0; column != end; column += step) {
        LineAttributes laPreviousCell;
        CartesianDiagramDataCompressor::DataPoint lastPoint;
        qreal lastAreaBoundingValue = 0;

        // Get min. y value, used as lower or upper bounding for area highlighting
        const qreal minYValue = qMin(plane->visibleDataRange().bottom(), plane->visibleDataRange().top());

        CartesianDiagramDataCompressor::CachePosition previousCellPosition;
        for (int row = 0; row < rowCount; ++row) {
            const CartesianDiagramDataCompressor::CachePosition position(row, column);
            // get where to draw the line from:
            CartesianDiagramDataCompressor::DataPoint point = compressor().data(position);
            if (point.hidden) {
                continue;
            }

            const QModelIndex sourceIndex = attributesModel()->mapToSource(point.index);

            const LineAttributes laCell = diagram()->lineAttributes(sourceIndex);
            const LineAttributes::MissingValuesPolicy policy = laCell.missingValuesPolicy();

            // lower or upper bounding for the highlighted area
            qreal areaBoundingValue;
            if (laCell.areaBoundingDataset() != -1) {
                const CartesianDiagramDataCompressor::CachePosition areaBoundingCachePosition(row, laCell.areaBoundingDataset());
                areaBoundingValue = compressor().data(areaBoundingCachePosition).value;
            } else {
                // Use min. y value (i.e. zero line in most cases) if no bounding dataset is set
                areaBoundingValue = minYValue;
            }

            if (ISNAN(point.value)) {
                switch (policy) {
                case LineAttributes::MissingValuesAreBridged:
                    // we just bridge both values
                    continue;
                case LineAttributes::MissingValuesShownAsZero:
                    // set it to zero
                    point.value = 0.0;
                    break;
                case LineAttributes::MissingValuesHideSegments:
                    // they're just hidden
                    break;
                default:
                    break;
                    // hm....
                }
            }

            if (!ISNAN(point.value)) {
                // area corners, a + b are the line ends:
                const qreal offset = diagram()->centerDataPoints() ? 0.5 : 0;
                const QPointF a(plane->translate(QPointF(lastPoint.key + offset, lastPoint.value)));
                const QPointF b(plane->translate(QPointF(point.key + offset, point.value)));
                const QPointF c(plane->translate(QPointF(lastPoint.key + offset, lastAreaBoundingValue)));
                const QPointF d(plane->translate(QPointF(point.key + offset, areaBoundingValue)));
                const PositionPoints pts = PositionPoints(b, a, d, c);

                // add label
                m_private->addLabel(&lpc, sourceIndex, &position, pts, Position::NorthWest,
                                    Position::NorthWest, point.value);

                // add line and area, if switched on and we have a current and previous value
                if (!ISNAN(lastPoint.value)) {
                    lineList.append(LineAttributesInfo(sourceIndex, a, b));

                    if (laCell.displayArea()) {
                        QPainterPath path;

                        // TODO: revert back to lambdas when we stop caring about pre-C++11
                        // auto dataAt = [&] ( int i ) {
                        //     if ( i < 0 || i >= rowCount ) {
                        //         return QPointF( NAN, NAN );
                        //     } else {
                        //         const auto data = compressor().data( { i, column } );
                        //         return QPointF( plane->translate( QPointF( data.key + offset, data.value ) ) );
                        //     }
                        // };
                        struct dataAtLambda
                        {
                            dataAtLambda(int rowCount, int column, qreal offset, CartesianCoordinatePlane *plane, NormalLineDiagram *_this)
                                : rowCount(rowCount)
                                , column(column)
                                , offset(offset)
                                , plane(plane)
                                , _this(_this)
                            {
                            }

                            int rowCount;
                            int column;
                            qreal offset;
                            CartesianCoordinatePlane *plane;
                            NormalLineDiagram *_this;

                            QPointF operator()(int i) const
                            {
                                if (i < 0 || i >= rowCount) {
                                    return QPointF(NAN, NAN);
                                } else {
                                    const KDChart::CartesianDiagramDataCompressor::DataPoint data = _this->compressor().data(CartesianDiagramDataCompressor::CachePosition(i, column));
                                    return QPointF(plane->translate(QPointF(data.key + offset, data.value)));
                                }
                            }
                        };
                        dataAtLambda dataAt(rowCount, column, offset, plane, this);

                        path.moveTo(a);

                        addSplineChunkTo(path, tension, dataAt(row - 2), a, b, dataAt(row + 1), mainSplineDirection);

                        path.lineTo(d);
                        path.lineTo(c);
                        path.lineTo(a);
                        PaintingHelpers::paintAreas(m_private, ctx, attributesModel()->mapToSource(lastPoint.index),
                                                    QList<QPainterPath>() << path, laCell.transparency()); // TODO: change to {path} in C++11
                    }
                }
            }

            previousCellPosition = position;
            laPreviousCell = laCell;
            lastAreaBoundingValue = areaBoundingValue;
            lastPoint = point;
        }
    }

    // paint the lines
    PaintingHelpers::paintElements(m_private, ctx, lpc, lineList);
}
