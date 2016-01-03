/****************************************************************************
** Copyright (C) 2001-2016 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDCHART_STOCK_DIAGRAM_H
#define KDCHART_STOCK_DIAGRAM_H

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartStockBarAttributes.h"
#include "KDChartThreeDBarAttributes.h"

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

    explicit StockDiagram( QWidget *parent = 0, CartesianCoordinatePlane *plane = 0 );
    ~StockDiagram();

    void setType( Type type );
    Type type() const;

    void setStockBarAttributes( const StockBarAttributes &attr );
    StockBarAttributes stockBarAttributes() const;

    void setStockBarAttributes( int column, const StockBarAttributes &attr );
    StockBarAttributes stockBarAttributes( int column ) const;

    void setThreeDBarAttributes( const ThreeDBarAttributes &attr );
    ThreeDBarAttributes threeDBarAttributes() const;

    void setThreeDBarAttributes( int column, const ThreeDBarAttributes &attr );
    ThreeDBarAttributes threeDBarAttributes( int column ) const;

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

#if QT_VERSION < 0x040400 || defined(Q_COMPILER_MANGLES_RETURN_TYPE)
    virtual const int numberOfAbscissaSegments() const;
    virtual const int numberOfOrdinateSegments() const;
#else
    virtual int numberOfAbscissaSegments() const;
    virtual int numberOfOrdinateSegments() const;
#endif

    virtual void paint( PaintContext *paintContext );

    virtual void resize( const QSizeF &size );

    virtual qreal threeDItemDepth( int column ) const;
    virtual qreal threeDItemDepth( const QModelIndex &index ) const;

protected:
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const;
};

} // Namespace KDChart

#endif // KDCHART_STOCK_DIAGRAM_H

