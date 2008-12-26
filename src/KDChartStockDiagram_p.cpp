#include "KDChartStockDiagram_p.h"

using namespace KDChart;

void StockDiagram::Private::drawLowHighLine( const CartesianDiagramDataCompressor::DataPoint &low,
                                             const CartesianDiagramDataCompressor::DataPoint &high,
                                             PaintContext *context )
{
    PainterSaver painterSaver( context->painter() );

    context->painter()->setPen( diagram->lowHighLinePen( low.key ) );

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

void StockDiagram::Private::drawCandlestick( const CartesianDiagramDataCompressor::DataPoint &low,
                                             const CartesianDiagramDataCompressor::DataPoint &high,
                                             PaintContext *context )
{
    PainterSaver painterSaver( context->painter() );

    if ( low.value <= high.value ) {
        context->painter()->setPen( diagram->upTrendCandlestickPen( low.key ) );
        context->painter()->setBrush( diagram->upTrendCandlestickBrush( low.key ) );
    } else {
        context->painter()->setPen( diagram->downTrendCandlestickPen( low.key ) );
        context->painter()->setBrush( diagram->downTrendCandlestickBrush( low.key ) );
    }

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

void StockDiagram::Private::drawOpenMarker( const CartesianDiagramDataCompressor::DataPoint &open,
                                            PaintContext *context )
{
    PainterSaver painterSaver( context->painter() );

    context->painter()->setPen( diagram->pen( diagram->attributesModel()->mapToSource( open.index ) ) );

    QPointF leftPoint = context->coordinatePlane()->translate( QPointF( open.key + 0.5 - openMarkerLength, open.value ) );
    QPointF rightPoint = context->coordinatePlane()->translate( QPointF( open.key + 0.5, open.value ) );
    context->painter()->drawLine( leftPoint, rightPoint );

    DataValueTextInfoList list;
    appendDataValueTextInfoToList( diagram, list, diagram->attributesModel()->mapToSource( open.index ), 0,
                                   PositionPoints( leftPoint ), Position::South, Position::South, open.value );
    paintDataValueTextsAndMarkers( diagram, context, list, false );
}

void StockDiagram::Private::drawCloseMarker( const CartesianDiagramDataCompressor::DataPoint &close,
                                             PaintContext *context )
{
    PainterSaver painterSaver( context->painter() );

    context->painter()->setPen( diagram->pen( diagram->attributesModel()->mapToSource( close.index ) ) );

    QPointF leftPoint = context->coordinatePlane()->translate( QPointF( close.key + 0.5, close.value ) );
    QPointF rightPoint = context->coordinatePlane()->translate( QPointF( close.key + 0.5 + closeMarkerLength, close.value ) );
    context->painter()->drawLine( leftPoint, rightPoint );

    DataValueTextInfoList list;
    appendDataValueTextInfoToList( diagram, list, diagram->attributesModel()->mapToSource( close.index ), 0,
                                   PositionPoints( rightPoint ), Position::South, Position::South, close.value );
    paintDataValueTextsAndMarkers( diagram, context, list, false );
}

