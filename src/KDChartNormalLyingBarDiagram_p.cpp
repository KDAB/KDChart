/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005-2007 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartNormalLyingBarDiagram_p.h"

using namespace KDChart;

NormalLyingBarDiagram::NormalLyingBarDiagram( BarDiagram* d )
    : BarDiagramType( d )
{
}

BarDiagram::BarType NormalLyingBarDiagram::type() const
{
    return BarDiagram::Normal;
}

const QPair<QPointF, QPointF> NormalLyingBarDiagram::calculateDataBoundaries() const
{
    const int rowCount = compressor().modelDataRows();
    const int colCount = compressor().modelDataColumns();

    double xMin = 0.0;
    double xMax = diagram()->model() ? diagram()->model()->rowCount( diagram()->rootIndex() ) : 0;
    double yMin = 0.0, yMax = 0.0;

    bool bStarting = true;
    for ( int column = 0; column < colCount; ++column )
    {
        for ( int row = 0; row < rowCount; ++row )
        {
            const CartesianDiagramDataCompressor::CachePosition position( row, column );
            const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            const double value = point.value;
            // this is always true yMin can be 0 in case all values
            // are the same
            // same for yMax it can be zero if all values are negative
            if( bStarting ){
                yMin = value;
                yMax = value;
                bStarting = false;
            }else{
                yMin = qMin( yMin, value );
                yMax = qMax( yMax, value );
            }
        }
    }

    // special cases
    if (  yMax == yMin ) {
        if ( yMin == 0.0 )
            yMax = 0.1; //we need at least a range
        else if( yMax < 0.0 )
            yMax = 0.0; // they are the same and negative
        else if( yMin > 0.0 )
            yMin = 0.0; // they are the same but positive
    }
    //const QPointF bottomLeft ( QPointF( xMin, yMin ) );
    //const QPointF topRight ( QPointF( xMax, yMax ) );
    const QPointF bottomLeft ( QPointF( yMin, xMin ) );
    const QPointF topRight ( QPointF( yMax, xMax ) );

    return QPair< QPointF, QPointF >( bottomLeft,  topRight );
}

void NormalLyingBarDiagram::paint(  PaintContext* ctx )
{
    reverseMapper().clear();

    const QPair<QPointF,QPointF> boundaries = diagram()->dataBoundaries(); // cached

    const QPointF boundTop = ctx->coordinatePlane()->translate( boundaries.first ) ;
    const QPointF boundBottom = ctx->coordinatePlane()->translate( boundaries.second );

    const int rowCount = attributesModel()->rowCount(attributesModelRootIndex());
    const int colCount = attributesModel()->columnCount(attributesModelRootIndex());

    BarAttributes ba = diagram()->barAttributes( diagram()->model()->index( 0, 0, diagram()->rootIndex() ) );
    double barWidth = 0;
    double maxDepth = 0;
    double width = boundTop.y() - boundBottom.y();
    double groupWidth = width / (colCount + 2);
    double spaceBetweenBars = 0;
    double spaceBetweenGroups = 0;

    if ( ba.useFixedBarWidth() ) {
        barWidth = ba.fixedBarWidth();
        groupWidth += barWidth;

        // Pending Michel set a min and max value for the groupWidth
        // related to the area.width
        if ( groupWidth < 0 )
            groupWidth = 0;

        if ( groupWidth  * colCount > width )
            groupWidth = width / colCount;
    }

    // maxLimit: allow the space between bars to be larger until area.width()
    // is covered by the groups.
    double maxLimit = rowCount * (groupWidth + ((colCount-1) * ba.fixedDataValueGap()) );

    //Pending Michel: FixMe
    if ( ba.useFixedDataValueGap() ) {
        if ( width > maxLimit )
            spaceBetweenBars += ba.fixedDataValueGap();
        else
            spaceBetweenBars = ((width/rowCount) - groupWidth)/(colCount-1);
    }

    if ( ba.useFixedValueBlockGap() ) {
        spaceBetweenGroups += ba.fixedValueBlockGap();
    }

    calculateValueAndGapWidths( colCount, rowCount,groupWidth,
                                barWidth, spaceBetweenBars, spaceBetweenGroups );

    DataValueTextInfoList list;

    // Make sure the top bars are painted first, and thus iterate through
    // them starting from the top
    for( int row = rowCount - 1; row >= 0; --row )
    {
        double offset = -groupWidth/2 + spaceBetweenGroups/2;

        if ( ba.useFixedDataValueGap() )
        {
            if ( spaceBetweenBars > 0 )
            {
                if ( width > maxLimit )
                    offset -= ba.fixedDataValueGap();
                else
                    offset -= ((width/rowCount) - groupWidth)/(colCount-1);

            }
            else
            {
                offset += barWidth/2;
            }
        }

        offset += (double)colCount * (barWidth + spaceBetweenBars);
        for( int column = colCount - 1; column >= 0; --column )
        {
            // paint one group
            const CartesianDiagramDataCompressor::CachePosition position( row,  column );
            const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            const QModelIndex sourceIndex = attributesModel()->mapToSource( point.index );
            const qreal value = point.value;//attributesModel()->data( sourceIndex ).toDouble();
            // To reverse the y-values, we need to get the upper y-value bound
            const double topValue = calculateDataBoundaries().second.y() - 1;
            QPointF leftPoint =  ctx->coordinatePlane()->translate( QPointF( 0, topValue - point.key + 0.5 ) );
            QPointF rightPoint = ctx->coordinatePlane()->translate( QPointF( value, topValue - point.key ) );
            const double barHeight = rightPoint.x() - leftPoint.x();
            leftPoint.setY( leftPoint.y() + offset );
            const QRectF rect( leftPoint, QSizeF( barHeight, barWidth ) );
            appendDataValueTextInfoToList( diagram(), list, sourceIndex, PositionPoints( rect ),
                                           Position::NorthEast, Position::SouthWest,
                                           point.value );
            paintBars( ctx, sourceIndex, rect, maxDepth );
            
            offset -= barWidth + spaceBetweenBars;
        }
    }
    paintDataValueTextsAndMarkers(  diagram(),  ctx,  list,  false );
}
