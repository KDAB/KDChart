/****************************************************************************
** Copyright (C) 2001-2021 Klaralvdalens Datakonsult AB.  All rights reserved.
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
    if ( qFuzzyIsNull( m_private->tension ) ) {
        paintWithLines( ctx );

    } else {
        paintWithSplines( ctx, m_private->tension );
    }
}

void PercentLineDiagram::paintWithLines( PaintContext* ctx )
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

void PercentLineDiagram::paintWithSplines( PaintContext* ctx, qreal tension )
{
    reverseMapper().clear();

    const int columnCount = compressor().modelDataColumns();
    const int rowCount = compressor().modelDataRows();

    Q_ASSERT(dynamic_cast<CartesianCoordinatePlane*>(ctx->coordinatePlane()));
    const auto plane = static_cast<CartesianCoordinatePlane*>(ctx->coordinatePlane());
    const auto mainSplineDirection = plane->isHorizontalRangeReversed() ? ReverseSplineDirection : NormalSplineDirection;
    const auto reverseSplineDirection = plane->isHorizontalRangeReversed() ? NormalSplineDirection : ReverseSplineDirection;

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

    for ( int column = 0; column < columnCount; ++column )
    {
        //display area can be set by dataset ( == column) and/or by cell
        LineAttributes laPreviousCell; // by default no area is drawn
        QModelIndex indexPreviousCell;
        QList<QPainterPath> areas;

        for ( int row = 0; row < rowCount; ++row )
        {
            const CartesianDiagramDataCompressor::CachePosition position( row, column );
            CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            const QModelIndex sourceIndex = attributesModel()->mapToSource( point.index );
            const LineAttributes laCell = diagram()->lineAttributes( sourceIndex );
            const bool bDisplayCellArea = laCell.displayArea();

            const LineAttributes::MissingValuesPolicy policy = laCell.missingValuesPolicy();

            // TODO: revert back to lambdas when we stop caring about pre-C++11
            // auto valueAt = [&] ( int row, int col ) -> qreal {
            //     if ( row < 0 || row >= rowCount ) {
            //         return NAN;
            //     }
            //
            //     const CartesianDiagramDataCompressor::CachePosition position( row, col );
            //     const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            //
            //     return !ISNAN( point.value ) ? point.value
            //          : policy == LineAttributes::MissingValuesAreBridged ? interpolateMissingValue( position )
            //          : NAN;
            // };
            //
            // auto safeAddPositive = [] ( qreal accumulator, qreal newValue ) {
            //     return ISNAN( newValue ) || newValue <= 0 ? accumulator : accumulator + newValue;
            // };

            struct valueAtLambda {
                valueAtLambda( int rowCount, PercentLineDiagram* _this, LineAttributes::MissingValuesPolicy policy )
                    : rowCount( rowCount )
                    , _this( _this )
                    , policy( policy )
                {
                }

                int rowCount;
                PercentLineDiagram* _this;
                LineAttributes::MissingValuesPolicy policy;

                qreal operator() ( int row, int col ) const
                {
                    if ( row < 0 || row >= rowCount ) {
                        return NAN;
                    }

                    const CartesianDiagramDataCompressor::CachePosition position( row, col );
                    const CartesianDiagramDataCompressor::DataPoint point = _this->compressor().data( position );

                    return !ISNAN( point.value ) ? point.value
                         : policy == LineAttributes::MissingValuesAreBridged ? _this->interpolateMissingValue( position )
                         : NAN;
                }

            };
            valueAtLambda valueAt( rowCount, this, policy );


            struct safeAddPositiveLambda {
                qreal operator() (qreal accumulator, qreal newValue) const
                {
                    return ISNAN( newValue ) || newValue <= 0 ? accumulator : accumulator + newValue;
                }
            };
            safeAddPositiveLambda safeAddPositive;

            qreal nextKey = 0;
            QVector<qreal> stackedValuesTop( 4, 0.0 );
            QVector<qreal> stackedValuesBottom( 4, 0.0 );

            if ( ! qFuzzyIsNull( percentSumValues.at( row ) ) ) {
                for ( int currentRow = 0; currentRow < 4; ++currentRow ) {
                    stackedValuesTop[currentRow] = safeAddPositive( stackedValuesTop[currentRow], valueAt( row - 1 + currentRow, column ) );
                }

                for ( int column2 = column - 1; column2 >= 0; --column2 )
                {
                    for ( int currentRow = 0; currentRow < 4; ++currentRow ) {
                        stackedValuesTop[currentRow] = safeAddPositive( stackedValuesTop[currentRow], valueAt( row - 1 + currentRow, column2 ) );
                        stackedValuesBottom[currentRow] = safeAddPositive( stackedValuesBottom[currentRow], valueAt( row - 1 + currentRow, column2 ) );
                    }
                }

                for ( int currentRow = 0; currentRow < 4; ++currentRow ) {
                    stackedValuesTop[currentRow] /= percentSumValues.at( row ) / maxValue;
                    stackedValuesBottom[currentRow] /= percentSumValues.at( row ) / maxValue;
                }
            }

            nextKey = row + 1;

            // TODO: revert back to lambdas when we stop caring about pre-C++11
            // auto dataAt = [&] ( const QVector<qreal>& source, qreal key, int index ) {
            //     return ctx->coordinatePlane()->translate( QPointF( diagram()->centerDataPoints() ? key + 0.5 : key, source[index] ) );
            // };
            struct dataAtLambda {
                dataAtLambda( PaintContext* ctx, PercentLineDiagram* _this )
                    : ctx( ctx )
                    , _this( _this )
                {
                }

                PaintContext* ctx;
                PercentLineDiagram* _this;

                QPointF operator() ( const QVector<qreal>& source, qreal key, int index ) const
                {
                    return ctx->coordinatePlane()->translate( QPointF( _this->diagram()->centerDataPoints() ? key + 0.5 : key, source[index] ) );
                }
            };
            dataAtLambda dataAt( ctx, this );

            const QPointF ptNorthWest = dataAt( stackedValuesTop, point.key, 1 );
            const QPointF ptSouthWest =
                    bDisplayCellArea ? dataAt( stackedValuesBottom, point.key, 1 )
                                     : ptNorthWest;

            QPointF ptNorthEast;
            QPointF ptSouthEast;

            if ( row + 1 < rowCount ) {
                ptNorthEast = dataAt( stackedValuesTop, nextKey, 2 );
                lineList.append( LineAttributesInfo( sourceIndex, ptNorthWest, ptNorthEast ) );
                ptSouthEast =
                    bDisplayCellArea ? dataAt( stackedValuesBottom, nextKey, 2 )
                                     : ptNorthEast;

                if ( areas.count() && laCell != laPreviousCell ) {
                    PaintingHelpers::paintAreas( m_private, ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
                    areas.clear();
                }

                if ( bDisplayCellArea ) {
                    QPainterPath path;
                    path.moveTo( ptNorthWest );

                    const QPointF ptBeforeNorthWest =
                        row > 0 ? dataAt( stackedValuesTop, point.key - 1, 0 )
                                : ptNorthWest;
                    const QPointF ptAfterNorthEast =
                        row < rowCount - 1 ? dataAt( stackedValuesTop, point.key + 2, 3 )
                                           : ptNorthEast;
                    addSplineChunkTo( path, tension, ptBeforeNorthWest, ptNorthWest, ptNorthEast, ptAfterNorthEast, mainSplineDirection );

                    path.lineTo( ptNorthEast );
                    path.lineTo( ptSouthEast );

                    const QPointF ptBeforeSouthWest =
                        row > 0 ? dataAt( stackedValuesBottom, point.key - 1, 0 )
                                : ptSouthWest;
                    const QPointF ptAfterSouthEast =
                        row < rowCount - 1 ? dataAt( stackedValuesBottom, point.key + 2, 3 )
                                           : ptSouthEast;
                    addSplineChunkTo( path, tension, ptAfterSouthEast, ptSouthEast, ptSouthWest, ptBeforeSouthWest, reverseSplineDirection );

                    areas << path;
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
    }
    PaintingHelpers::paintElements( m_private, ctx, lpc, lineList );
}
