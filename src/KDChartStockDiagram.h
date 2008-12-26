#ifndef KDCHART_STOCK_DIAGRAM_H
#define KDCHART_STOCK_DIAGRAM_H

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartCartesianCoordinatePlane.h"

namespace KDChart {
    class PaintContext;

class KDCHART_EXPORT StockDiagram : public AbstractCartesianDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( StockDiagram )

    KDCHART_DECLARE_DERIVED_DIAGRAM( StockDiagram, CartesianCoordinatePlane )

public:
    enum Type {
        HighLowClose,
        OpenHighLowClose,
        Candlestick
    };

    StockDiagram( QWidget *parent = 0, CartesianCoordinatePlane *plane = 0 );
    ~StockDiagram();

    void setType( Type type );
    Type type() const;

    void setLowHighLinePen( const QPen &pen );
    QPen lowHighLinePen() const;

    void setLowHighLinePen( int column, const QPen &pen );
    QPen lowHighLinePen( int column ) const;

    void setUpTrendCandlestickBrush( const QBrush &brush );
    QBrush upTrendCandlestickBrush() const;

    void setDownTrendCandlestickBrush( const QBrush &brush );
    QBrush downTrendCandlestickBrush() const;

    void setUpTrendCandlestickBrush( int column, const QBrush &brush );
    QBrush upTrendCandlestickBrush( int column ) const;

    void setDownTrendCandlestickBrush( int column, const QBrush &brush );
    QBrush downTrendCandlestickBrush( int column ) const;

    void setUpTrendCandlestickPen( const QPen &pen );
    QPen upTrendCandlestickPen() const;

    void setDownTrendCandlestickPen( const QPen &pen );
    QPen downTrendCandlestickPen() const;

    void setUpTrendCandlestickPen( int column, const QPen &pen );
    QPen upTrendCandlestickPen( int column ) const;

    void setDownTrendCandlestickPen( int column, const QPen &pen );
    QPen downTrendCandlestickPen( int column ) const;

    virtual const int numberOfAbscissaSegments() const;
    virtual const int numberOfOrdinateSegments() const;

    virtual void paint( PaintContext *paintContext );

    virtual void resize( const QSizeF &size );

    virtual double threeDItemDepth( int column ) const;
    virtual double threeDItemDepth( const QModelIndex &index ) const;

protected:
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const;

private:

};

} // Namespace KDChart

#endif // KDCHART_STOCK_DIAGRAM_H

