#include "KDChartStockDiagram_p.h"

using namespace KDChart;

/**
  * Draws a line connecting the low and the high value of an OHLC chart
  *
  * @param low The low data point
  * @param high The high data point
  * @param context The context to draw the low-high line in
  */
void StockDiagram::Private::drawLowHighLine( const CartesianDiagramDataCompressor::DataPoint &low,
                                             const CartesianDiagramDataCompressor::DataPoint &high,
                                             PaintContext *context )
{
    PainterSaver painterSaver( context->painter() );

    context->painter()->setPen( diagram->lowHighLinePen( (int)low.key ) );

    QPointF lowPoint = context->coordinatePlane()->translate( QPointF( low.key + 0.5, low.value ) );
    QPointF highPoint = context->coordinatePlane()->translate( QPointF( high.key + 0.5, high.value ) );
    context->painter()->drawLine( lowPoint, highPoint );

    DataValueTextInfoList list;
    appendDataValueTextInfoToList( diagram, list, diagram->attributesModel()->mapToSource( low.index ), 0,
                                   PositionPoints( lowPoint ), Position::South, Position::South, low.value );
    appendDataValueTextInfoToList( diagram, list, diagram->attributesModel()->mapToSource( high.index ), 0,
                                   PositionPoints( highPoint ), Position::South, Position::South, high.value );
    paintDataValueTextsAndMarkers( diagram, context, list, false );
}

/**
  * Draws a line connecting the low and the high value of an OHLC chart
  *
  * @param low The low data point
  * @param high The high data point
  * @param context The context to draw the candlestick in
  */
void StockDiagram::Private::drawCandlestick( const CartesianDiagramDataCompressor::DataPoint &low,
                                             const CartesianDiagramDataCompressor::DataPoint &high,
                                             PaintContext *context )
{
    PainterSaver painterSaver( context->painter() );

    if ( low.value <= high.value ) {
        context->painter()->setPen( diagram->upTrendCandlestickPen( (int)low.key ) );
        context->painter()->setBrush( diagram->upTrendCandlestickBrush( (int)low.key ) );
    } else {
        context->painter()->setPen( diagram->downTrendCandlestickPen( (int)low.key ) );
        context->painter()->setBrush( diagram->downTrendCandlestickBrush( (int)low.key ) );
    }

    // If the keys are not equal, something must have gone wong
    Q_ASSERT( (int)low.key == (int)high.key );

    StockBarAttributes attr = diagram->stockBarAttributes( (int)low.key );
    const qreal candlestickWidth = attr.candlestickWidth();
    QPointF leftLowPoint = context->coordinatePlane()->translate( QPointF( low.key + 0.5 - candlestickWidth / 2.0, low.value ) );
    QPointF rightLowPoint = context->coordinatePlane()->translate( QPointF( low.key + 0.5 + candlestickWidth / 2.0, low.value ) );
    QPointF leftHighPoint = context->coordinatePlane()->translate( QPointF( high.key + 0.5 - candlestickWidth / 2.0, high.value ) );
    QPointF rightHighPoint = context->coordinatePlane()->translate( QPointF( high.key + 0.5 + candlestickWidth / 2.0, high.value ) );
    QPolygonF candlestick;
    candlestick.append( leftLowPoint );
    candlestick.append( rightLowPoint );
    candlestick.append( rightHighPoint );
    candlestick.append( leftHighPoint );
    candlestick.append( leftLowPoint );
    context->painter()->drawPolygon( candlestick );

    DataValueTextInfoList list;
    appendDataValueTextInfoToList( diagram, list, diagram->attributesModel()->mapToSource( low.index ), 0,
                                   PositionPoints( rightLowPoint ), Position::South, Position::South, low.value );
    appendDataValueTextInfoToList( diagram, list, diagram->attributesModel()->mapToSource( high.index ), 0,
                                   PositionPoints( rightHighPoint ), Position::South, Position::South, high.value );
    paintDataValueTextsAndMarkers( diagram, context, list, false );
}

/**
  * Draws a tick indicating the open value
  *
  * @param low The open data point
  * @param context The context to draw the open marker in
  */
void StockDiagram::Private::drawOpenMarker( const CartesianDiagramDataCompressor::DataPoint &open,
                                            PaintContext *context )
{
    PainterSaver painterSaver( context->painter() );

    context->painter()->setPen( diagram->pen( diagram->attributesModel()->mapToSource( open.index ) ) );

    StockBarAttributes attr = diagram->stockBarAttributes( (int)open.key );
    QPointF leftPoint = context->coordinatePlane()->translate( QPointF( open.key + 0.5 - attr.tickLength(), open.value ) );
    QPointF rightPoint = context->coordinatePlane()->translate( QPointF( open.key + 0.5, open.value ) );
    context->painter()->drawLine( leftPoint, rightPoint );

    DataValueTextInfoList list;
    appendDataValueTextInfoToList( diagram, list, diagram->attributesModel()->mapToSource( open.index ), 0,
                                   PositionPoints( leftPoint ), Position::South, Position::South, open.value );
    paintDataValueTextsAndMarkers( diagram, context, list, false );
}

/**
  * Draws a tick indicating the close value
  *
  * @param low The close data point
  * @param context The context to draw the close marker in
  */
void StockDiagram::Private::drawCloseMarker( const CartesianDiagramDataCompressor::DataPoint &close,
                                             PaintContext *context )
{
    PainterSaver painterSaver( context->painter() );

    context->painter()->setPen( diagram->pen( diagram->attributesModel()->mapToSource( close.index ) ) );

    StockBarAttributes attr = diagram->stockBarAttributes( (int)close.key );
    QPointF leftPoint = context->coordinatePlane()->translate( QPointF( close.key + 0.5, close.value ) );
    QPointF rightPoint = context->coordinatePlane()->translate( QPointF( close.key + 0.5 + attr.tickLength(), close.value ) );
    context->painter()->drawLine( leftPoint, rightPoint );

    DataValueTextInfoList list;
    appendDataValueTextInfoToList( diagram, list, diagram->attributesModel()->mapToSource( close.index ), 0,
                                   PositionPoints( rightPoint ), Position::South, Position::South, close.value );
    paintDataValueTextsAndMarkers( diagram, context, list, false );
}

