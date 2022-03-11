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

#ifndef KDCHART_STOCK_DIAGRAM_P_H
#define KDCHART_STOCK_DIAGRAM_P_H

#include "KDChartStockDiagram.h"
#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartCartesianDiagramDataCompressor_p.h"
#include "KDChartPaintContext.h"

namespace KDChart {

class StockDiagram::Private : public AbstractCartesianDiagram::Private
{
    friend class StockDiagram;

public:
    Private();
    Private(const Private &r);
    ~Private() override;

    Type type;
    StockDiagram *stockDiagram()
    {
        return static_cast<StockDiagram *>(diagram);
    }

    QBrush upTrendCandlestickBrush;
    QBrush downTrendCandlestickBrush;
    QPen upTrendCandlestickPen;
    QPen downTrendCandlestickPen;

    QMap<int, QBrush> upTrendCandlestickBrushes;
    QMap<int, QBrush> downTrendCandlestickBrushes;
    QMap<int, QPen> upTrendCandlestickPens;
    QMap<int, QPen> downTrendCandlestickPens;

    QPen lowHighLinePen;
    QMap<int, QPen> lowHighLinePens;

    void drawOHLCBar(int dataset, const CartesianDiagramDataCompressor::DataPoint &open,
                     const CartesianDiagramDataCompressor::DataPoint &high,
                     const CartesianDiagramDataCompressor::DataPoint &low,
                     const CartesianDiagramDataCompressor::DataPoint &close,
                     PaintContext *context);
    void drawHLCBar(int dataset, const CartesianDiagramDataCompressor::DataPoint &high,
                    const CartesianDiagramDataCompressor::DataPoint &low,
                    const CartesianDiagramDataCompressor::DataPoint &close,
                    PaintContext *context);
    void drawCandlestick(int dataset, const CartesianDiagramDataCompressor::DataPoint &open,
                         const CartesianDiagramDataCompressor::DataPoint &high,
                         const CartesianDiagramDataCompressor::DataPoint &low,
                         const CartesianDiagramDataCompressor::DataPoint &close,
                         PaintContext *context);

private:
    void drawLine(int dataset, int col, const QPointF &point1, const QPointF &p2, PaintContext *context);
    QPointF projectPoint(PaintContext *context, const QPointF &point) const;
    QRectF projectCandlestick(PaintContext *context, const QPointF &open, const QPointF &close, qreal width) const;
    int openValueColumn() const;
    int highValueColumn() const;
    int lowValueColumn() const;
    int closeValueColumn() const;

    class ThreeDPainter;
};

KDCHART_IMPL_DERIVED_DIAGRAM(StockDiagram, AbstractCartesianDiagram, CartesianCoordinatePlane)
}

#endif // KDCHART_STOCK_DIAGRAM_P_H
