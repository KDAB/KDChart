/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartStackedBarDiagram_p.h"

using namespace KDChart;

StackedBarDiagram::StackedBarDiagram( BarDiagram* d )
    : BarDiagramType( d )
{
}

BarDiagram::BarType StackedBarDiagram::type() const
{
    return BarDiagram::Stacked;
}

const QPair<QPointF, QPointF> StackedBarDiagram::calculateDataBoundaries() const
{
    const int rowCount = compressor().modelDataRows();
    const int colCount = compressor().modelDataColumns();

    qreal xMin = 0;
    qreal xMax = diagram()->model() ? diagram()->model()->rowCount( diagram()->rootIndex() ) : 0;
    qreal yMin = 0, yMax = 0;

    bool bStarting = true;
    for( int row = 0; row < rowCount; ++row )
    {
        // calculate sum of values per column - Find out stacked Min/Max
        qreal stackedValues = 0.0;
        qreal negativeStackedValues = 0.0;
        for ( int col = 0; col < colCount ; ++col )
        {
            const CartesianDiagramDataCompressor::CachePosition position( row, col );
            const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            const double value = ISNAN( point.value ) ? 0.0 : point.value;

            if( value > 0.0 )
                stackedValues += value;
            else
                negativeStackedValues += value;

            // this is always true yMin can be 0 in case all values
            // are the same
            // same for yMax it can be zero if all values are negative
            if( bStarting ){
                yMin = negativeStackedValues < 0.0 ? negativeStackedValues : stackedValues;
                yMax = stackedValues > 0.0 ? stackedValues : negativeStackedValues;
                bStarting = false;
            }else{
                yMin = qMin( qMin( yMin, stackedValues ), negativeStackedValues );
                yMax = qMax( qMax( yMax, stackedValues ), negativeStackedValues );
            }
        }
    }
    // special cases
    if ( yMax == yMin ) {
        if ( yMin == 0.0 )
            yMax = 0.1; //we need at least a range
        else if( yMax < 0.0 )
            yMax = 0.0; // they are the same and negative
        else if( yMin > 0.0 )
            yMin = 0.0; // they are the same but positive
    }
    const QPointF bottomLeft ( QPointF( xMin, yMin ) );
    const QPointF topRight ( QPointF( xMax, yMax ) );

    return QPair< QPointF, QPointF >( bottomLeft,  topRight );
}

void StackedBarDiagram::paint( PaintContext* ctx )
{
    reverseMapper().clear();

    const QPair<QPointF,QPointF> boundaries = diagram()->dataBoundaries(); // cached

    const QPointF boundLeft = ctx->coordinatePlane()->translate( boundaries.first ) ;
    const QPointF boundRight = ctx->coordinatePlane()->translate( boundaries.second );

    const int rowCount = compressor().modelDataRows();
    const int colCount = compressor().modelDataColumns();

    BarAttributes ba = diagram()->barAttributes();
    qreal barWidth = 0;
    qreal maxDepth = 0;
    qreal width = boundRight.x() - boundLeft.x();
    qreal groupWidth = width/ (rowCount + 2);
    qreal spaceBetweenBars = 0;
    qreal spaceBetweenGroups = 0;

    if ( ba.useFixedBarWidth() ) {
        barWidth = ba.fixedBarWidth();
        groupWidth += barWidth;

        // Pending Michel set a min and max value for the groupWidth
        // related to the area.width
        if ( groupWidth < 0 )
            groupWidth = 0;

        if ( groupWidth  * rowCount > width )
            groupWidth = width / rowCount;
    }

    // maxLimit: allow the space between bars to be larger until area.width()
    // is covered by the groups.
    qreal maxLimit = rowCount * (groupWidth + ((colCount-1) * ba.fixedDataValueGap()) );


    //Pending Michel: FixMe
    if ( ba.useFixedDataValueGap() ) {
        if ( width > maxLimit )
            spaceBetweenBars += ba.fixedDataValueGap();
        else
            spaceBetweenBars = ((width/rowCount) - groupWidth)/(colCount-1);
    }

    if ( ba.useFixedValueBlockGap() )
        spaceBetweenGroups += ba.fixedValueBlockGap();

    calculateValueAndGapWidths( rowCount, colCount,groupWidth,
                                barWidth, spaceBetweenBars, spaceBetweenGroups );

    LabelPaintCache lpc;
    for( int col = 0; col < colCount; ++col )
    {
        qreal offset = spaceBetweenGroups;
        if( ba.useFixedBarWidth() )
            offset -= ba.fixedBarWidth();
        
        if( offset < 0 )
            offset = 0;

        for( int row = 0; row < rowCount; ++row )
        {
            const CartesianDiagramDataCompressor::CachePosition position( row, col );
            const CartesianDiagramDataCompressor::DataPoint p = compressor().data( position );
 
            const QModelIndex index = attributesModel()->mapToSource( p.index );
            ThreeDBarAttributes threeDAttrs = diagram()->threeDBarAttributes( index );
            const qreal value = p.value;
            qreal stackedValues = 0.0;
            qreal key = 0.0;

            if ( threeDAttrs.isEnabled() ) {
                if ( barWidth > 0 )
                    barWidth =  (width - ((offset+(threeDAttrs.depth()))*rowCount))/ rowCount;
                if ( barWidth <= 0 ) {
                    barWidth = 0;
                    maxDepth = offset - (width/rowCount);
                }
            } else {
                barWidth =  (width - (offset*rowCount))/ rowCount ;
            }

            for ( int k = col; k >= 0; --k )
            {
                const CartesianDiagramDataCompressor::CachePosition position( row, k );
                const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
                if( !ISNAN( point.value ) && (( p.value >= 0.0 && point.value >= 0.0 ) || ( p.value < 0.0 && point.value < 0.0 )) )
                    stackedValues += point.value;
                key = point.key;
            }

            if (!ISNAN( value ))
            {
                const qreal usedDepth = threeDAttrs.depth();

                QPointF point = ctx->coordinatePlane()->translate( QPointF( key, stackedValues ) );

                const qreal dy = point.y() - usedDepth;
                if ( dy < 0 ) {
                    threeDAttrs.setDepth( point.y() - 1 );
                    diagram()->setThreeDBarAttributes( threeDAttrs );
                }

                point.rx() += offset / 2;
                const QPointF previousPoint = ctx->coordinatePlane()->translate( QPointF( key, stackedValues - value ) );
                const qreal barHeight = previousPoint.y() - point.y();

                const QRectF rect( point, QSizeF( barWidth , barHeight ) );
                addLabel( &lpc, index, PositionPoints( rect ), Position::North,
                          Position::South, value );
                paintBars( ctx, index, rect, maxDepth );
            }
        }
    }
    paintDataValueTextsAndMarkers( ctx, lpc, false );
}