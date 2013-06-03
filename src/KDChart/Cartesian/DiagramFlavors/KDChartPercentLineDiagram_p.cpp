/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartPercentLineDiagram_p.h"

#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartLineDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "PaintingHelpers_p.h"

using namespace KDChart;
using namespace std;

PercentLineDiagram::PercentLineDiagram( LineDiagram* d )
    : LineDiagramType( d )
{
}

LineDiagram::LineType PercentLineDiagram::type() const
{
    return LineDiagram::Percent;
}

const QPair<QPointF, QPointF> PercentLineDiagram::calculateDataBoundaries() const
{
    const qreal xMin = 0.0;
    qreal xMax = diagram()->model() ? diagram()->model()->rowCount( diagram()->rootIndex() ) : 0;
    if ( !diagram()->centerDataPoints() && diagram()->model() )
        xMax -= 1;
    const qreal yMin = 0.0;
    const qreal yMax = 100.0;

    QPointF bottomLeft( QPointF( xMin, yMin ) );
    QPointF topRight( QPointF( xMax, yMax ) );
    return QPair<QPointF, QPointF> ( bottomLeft, topRight );
}

void PercentLineDiagram::paint( PaintContext* ctx )
{
    reverseMapper().clear();

    const int columnCount = compressor().modelDataColumns();
    const int rowCount = compressor().modelDataRows();

// FIXME integrade column index retrieval to compressor:
    int maxFound = 0;
//    {   // find the last column number that is not hidden
//        for ( int iColumn =  datasetDimension() - 1;
//             iColumn <  columnCount;
//             iColumn += datasetDimension() )
//            if ( ! diagram()->isHidden( iColumn ) )
//                maxFound = iColumn;
//    }
    maxFound = columnCount;
    // ^^^ temp
    const int lastVisibleColumn = maxFound - 1;

    LabelPaintCache lpc;
    LineAttributesInfoList lineList;

    //FIXME(khz): add LineAttributes::MissingValuesPolicy support for LineDiagram::Stacked and ::Percent

    qreal maxValue = 100; // always 100%
    qreal sumValues = 0;
    QVector <qreal > percentSumValues;

    //calculate sum of values for each column and store
    for ( int row = 0; row < rowCount; ++row )
    {
        for ( int col = 0; col < columnCount; ++col )
        {
            const CartesianDiagramDataCompressor::CachePosition position( row, col );
            CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            const QModelIndex sourceIndex = attributesModel()->mapToSource( point.index );
            const LineAttributes laCell = diagram()->lineAttributes( sourceIndex );
            const LineAttributes::MissingValuesPolicy policy = laCell.missingValuesPolicy();
            if ( ISNAN( point.value ) && policy == LineAttributes::MissingValuesAreBridged )
                point.value = interpolateMissingValue( position );
            if ( point.value > 0 )
                sumValues += point.value;
            if ( col == lastVisibleColumn )
            {
                percentSumValues << sumValues ;
                sumValues = 0;
            }
        }
    }

    QList<QPointF> bottomPoints;
    bool bFirstDataset = true;

    for ( int column = 0; column < columnCount; ++column )
    {
        //display area can be set by dataset ( == column) and/or by cell
        LineAttributes laPreviousCell; // by default no area is drawn
        QModelIndex indexPreviousCell;
        QList<QPolygonF> areas;
        QList<QPointF> points;

        for ( int row = 0; row < rowCount; ++row )
        {
            const CartesianDiagramDataCompressor::CachePosition position( row, column );
            CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            const QModelIndex sourceIndex = attributesModel()->mapToSource( point.index );
            const LineAttributes laCell = diagram()->lineAttributes( sourceIndex );
            const bool bDisplayCellArea = laCell.displayArea();

            qreal stackedValues = 0, nextValues = 0, nextKey = 0;
            for ( int column2 = column;
                  column2 >= 0;//datasetDimension() - 1;
                  column2 -= 1 )//datasetDimension() )
            {
                const CartesianDiagramDataCompressor::CachePosition position( row, column2 );
                CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );

                const LineAttributes::MissingValuesPolicy policy = laCell.missingValuesPolicy();
                if ( ISNAN( point.value ) && policy == LineAttributes::MissingValuesAreBridged )
                    point.value = interpolateMissingValue( position );

                const qreal val = point.value;
                if ( val > 0 )
                    stackedValues += val;
                //qDebug() << valueForCell( iRow, iColumn2 );
                if ( row + 1 < rowCount ) {
                    const CartesianDiagramDataCompressor::CachePosition position( row + 1, column2 );
                    CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );

                    const LineAttributes::MissingValuesPolicy policy = laCell.missingValuesPolicy();
                    if ( ISNAN( point.value ) && policy == LineAttributes::MissingValuesAreBridged )
                        point.value = interpolateMissingValue( position );

                    const qreal val = point.value;
                    if ( val > 0 )
                        nextValues += val;
                    nextKey = point.key;
                }
            }
            if ( percentSumValues.at( row ) != 0 )
                stackedValues = stackedValues / percentSumValues.at( row ) * maxValue;
            else
                stackedValues = 0.0;
            //qDebug() << stackedValues << endl;
            QPointF nextPoint = ctx->coordinatePlane()->translate( QPointF( diagram()->centerDataPoints() ? point.key + 0.5 : point.key, stackedValues ) );
            points << nextPoint;

            const QPointF ptNorthWest( nextPoint );
            const QPointF ptSouthWest(
                bDisplayCellArea
                ? ( bFirstDataset
                    ? ctx->coordinatePlane()->translate( QPointF( diagram()->centerDataPoints() ? point.key + 0.5 : point.key, 0.0 ) )
                    : bottomPoints.at( row )
                    )
                : nextPoint );
            QPointF ptNorthEast;
            QPointF ptSouthEast;

            if ( row + 1 < rowCount ) {
                 if ( percentSumValues.at( row + 1 ) != 0 )
                     nextValues = nextValues / percentSumValues.at( row + 1 ) * maxValue;
                 else
                     nextValues = 0.0;
                QPointF toPoint = ctx->coordinatePlane()->translate( QPointF( diagram()->centerDataPoints() ? nextKey + 0.5 : nextKey, nextValues ) );
                lineList.append( LineAttributesInfo( sourceIndex, nextPoint, toPoint ) );
                ptNorthEast = toPoint;
                ptSouthEast =
                    bDisplayCellArea
                    ? ( bFirstDataset
                        ? ctx->coordinatePlane()->translate( QPointF( diagram()->centerDataPoints() ? nextKey + 0.5 : nextKey, 0.0 ) )
                        : bottomPoints.at( row + 1 )
                        )
                    : toPoint;
                if ( areas.count() && laCell != laPreviousCell ) {
                    PaintingHelpers::paintAreas( m_private, ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
                    areas.clear();
                }
                if ( bDisplayCellArea ) {
                    QPolygonF poly;
                    poly << ptNorthWest << ptNorthEast << ptSouthEast << ptSouthWest;
                    areas << poly;
                    laPreviousCell = laCell;
                    indexPreviousCell = sourceIndex;
                } else {
                    //qDebug() << "no area shown for row"<<iRow<<"  column"<<iColumn;
                }
            } else {
                ptNorthEast = ptNorthWest;
                ptSouthEast = ptSouthWest;
            }

            if ( !ISNAN( point.value ) )
            {
                const PositionPoints pts( ptNorthWest, ptNorthEast, ptSouthEast, ptSouthWest );
                m_private->addLabel( &lpc, sourceIndex, &position, pts, Position::NorthWest,
                                     Position::NorthWest, point.value );
            }
        }
        if ( areas.count() ) {
            PaintingHelpers::paintAreas( m_private, ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
            areas.clear();
        }
        bottomPoints = points;
        bFirstDataset = false;
    }
    PaintingHelpers::paintElements( m_private, ctx, lpc, lineList );
}
