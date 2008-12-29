#ifndef KDCHART_STOCK_DIAGRAM_P_H
#define KDCHART_STOCK_DIAGRAM_P_H

#include "KDChartStockDiagram.h"
#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartCartesianDiagramDataCompressor_p.h"
#include "KDChartPaintContext.h"

namespace KDChart {

class StockDiagram::Private : public AbstractCartesianDiagram::Private
{
public:
    Type type;
    StockDiagram *diagram;

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

    void drawLowHighLine( const CartesianDiagramDataCompressor::DataPoint &low,
                          const CartesianDiagramDataCompressor::DataPoint &high,
                          PaintContext *context );
    void drawCandlestick( const CartesianDiagramDataCompressor::DataPoint &low,
                          const CartesianDiagramDataCompressor::DataPoint &high,
                          PaintContext *context );
    void drawOpenMarker( const CartesianDiagramDataCompressor::DataPoint &open,
                         PaintContext *context );
    void drawCloseMarker( const CartesianDiagramDataCompressor::DataPoint &close,
                          PaintContext *context );
};

KDCHART_IMPL_DERIVED_DIAGRAM( StockDiagram, AbstractCartesianDiagram, CartesianCoordinatePlane )

}

#endif // KDCHART_STOCK_DIAGRAM_P_H

