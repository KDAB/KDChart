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

#include "KDChartLineDiagram.h"
#include "KDChartDataValueAttributes.h"

#include "KDChartLineDiagram_p.h"
#include "KDChartPainterSaver_p.h"
#include "PaintingHelpers_p.h"

using namespace KDChart;
using namespace std;

LineDiagram::Private::Private( const Private& rhs )
    : AbstractCartesianDiagram::Private( rhs )
{
}

AttributesModel* LineDiagram::LineDiagramType::attributesModel() const
{
    return m_private->attributesModel;
}

QModelIndex LineDiagram::LineDiagramType::attributesModelRootIndex() const
{
    return m_private->diagram->attributesModelRootIndex();
}

int LineDiagram::LineDiagramType::datasetDimension() const
{
    return m_private->datasetDimension;
}

ReverseMapper& LineDiagram::LineDiagramType::reverseMapper()
{
    return m_private->reverseMapper;
}

LineDiagram* LineDiagram::LineDiagramType::diagram() const
{
    return static_cast< LineDiagram* >( m_private->diagram );
}

void LineDiagram::LineDiagramType::paintAreas(
    PaintContext* ctx,
    const QModelIndex& index, const QList< QPolygonF >& areas,
    uint opacity )
{
    QPainterPath path;
    for( int i = 0; i < areas.count(); ++i )
    {
        const QPolygonF& p = areas[ i ];
        path.addPolygon( p );
        reverseMapper().addPolygon( index.row(), index.column(), p );
        path.closeSubpath();
    }

    ThreeDLineAttributes threeDAttrs = m_private->diagram->threeDLineAttributes( index );
    QBrush trans = diagram()->brush( index );
    if( threeDAttrs.isEnabled() ) {
        trans = threeDAttrs.threeDBrush( trans, path.boundingRect() );
    }
    QColor transColor = trans.color();
    transColor.setAlpha( opacity );
    trans.setColor(transColor);
    QPen indexPen = diagram()->pen(index);
    indexPen.setBrush( trans );
    const PainterSaver painterSaver( ctx->painter() );

    ctx->painter()->setRenderHint( QPainter::Antialiasing, diagram()->antiAliasing() );
    ctx->painter()->setPen( PrintingParameters::scalePen( indexPen ) );
    ctx->painter()->setBrush( trans );

    ctx->painter()->drawPath( path );
}

qreal LineDiagram::LineDiagramType::valueForCell( int row, int column ) const
{
    return diagram()->valueForCell( row, column );
}

void LineDiagram::LineDiagramType::addLabel( LabelPaintCache* lpc,
                                             const QModelIndex & index,
                                             const CartesianDiagramDataCompressor::CachePosition* position,
                                             const PositionPoints& points,
                                             const Position& autoPositionPositive,
                                             const Position& autoPositionNegative, qreal value )
{
    m_private->addLabel( lpc, index, position, points,
                         autoPositionPositive, autoPositionNegative, value );
}

CartesianDiagramDataCompressor& LineDiagram::LineDiagramType::compressor() const
{
    return m_private->compressor;
}

qreal LineDiagram::LineDiagramType::interpolateMissingValue( const CartesianDiagramDataCompressor::CachePosition& pos ) const
{
    qreal leftValue = std::numeric_limits< qreal >::quiet_NaN();
    qreal rightValue = std::numeric_limits< qreal >::quiet_NaN();
    int missingCount = 1;

    const int column = pos.second;
    const int row = pos.first;
    const int rowCount = compressor().modelDataRows();

    // iterate back and forth to find valid values
    for( int r1 = row - 1; r1 > 0; --r1 )
    {
        const CartesianDiagramDataCompressor::CachePosition position( r1, column );
        const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
        leftValue = point.value;
        if( !ISNAN( point.value ) )
            break;
        ++missingCount;
    }
    for( int r2 = row + 1; r2 < rowCount; ++r2 )
    {
        const CartesianDiagramDataCompressor::CachePosition position( r2, column );
        const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
        rightValue = point.value;
        if( !ISNAN( point.value ) )
            break;
        ++missingCount;
    }
    if( !ISNAN( leftValue ) && !ISNAN( rightValue ) )
        return leftValue + ( rightValue - leftValue ) / ( missingCount + 1 );
    else
        return std::numeric_limits< qreal >::quiet_NaN();
}