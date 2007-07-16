#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartLineDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartStackedLineDiagram_p.h"

using namespace KDChart;

StackedLineDiagram::StackedLineDiagram( LineDiagram* d )
    : LineDiagramType( d )
{
}

LineDiagram::LineType StackedLineDiagram::type() const
{
    return LineDiagram::Stacked;
}

const QPair<QPointF, QPointF> StackedLineDiagram::calculateDataBoundaries() const
{
    const int rowCount = compressor().modelDataRows();
    const int colCount = compressor().modelDataColumns();
    double xMin = 0;
    double xMax = rowCount -1;
    double yMin = 0, yMax = 0;

    bool bStarting = true;
    for ( int j=0; j< rowCount; ++j ) {
        // calculate sum of values per column - Find out stacked Min/Max
        double stackedValues = 0;
        for( int i = datasetDimension() - 1; i < colCount; i += datasetDimension() ) {
            CartesianDiagramDataCompressor::CachePosition position( j, i );
            CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );

            stackedValues += point.value;
        }

        if( bStarting ){
            yMin = stackedValues;
            yMax = stackedValues;
            bStarting = false;
        }else{
            // Pending Michel:
            // I am taking in account all values negatives or positives
            // take in account all stacked values
            yMin = qMin( qMin( yMin,  0.0 ), stackedValues );
            yMax = qMax( yMax, stackedValues );
        }
    }

    QPointF bottomLeft( QPointF( xMin, yMin ) );
    QPointF topRight(   QPointF( xMax, yMax ) );
    return QPair<QPointF, QPointF> ( bottomLeft, topRight );
}

void StackedLineDiagram::paint(  PaintContext* ctx )
{
    reverseMapper().clear();

    const QPair<QPointF, QPointF> boundaries = diagram()->dataBoundaries();
    const QPointF bottomLeft = boundaries.first;
    const QPointF topRight = boundaries.second;
        
    const int columnCount = compressor().modelDataColumns();
    const int rowCount = compressor().modelDataRows();

// FIXME integrate column index retrieval to compressor:
    int maxFound = 0;
//    {   // find the last column number that is not hidden
//        for( int iColumn =  datasetDimension() - 1;
//             iColumn <  columnCount;
//             iColumn += datasetDimension() )
//            if( ! diagram()->isHidden( iColumn ) )
//                maxFound = iColumn;
//    }
    maxFound = columnCount;
    // ^^^ temp

    DataValueTextInfoList list;
    LineAttributesInfoList lineList;
    LineAttributes::MissingValuesPolicy policy;

    //FIXME(khz): add LineAttributes::MissingValuesPolicy support for LineDiagram::Stacked and ::Percent

    QVector <double > percentSumValues;

    QList<QPointF> bottomPoints;
    bool bFirstDataset = true;

    for( int column = 0; column < columnCount; ++column )
    {
        CartesianDiagramDataCompressor::CachePosition previousCellPosition;

        //display area can be set by dataset ( == column) and/or by cell
        LineAttributes laPreviousCell; // by default no area is drawn
        QModelIndex indexPreviousCell;
        QList<QPolygonF> areas;
        QList<QPointF> points;

        for ( int row = 0; row < rowCount; ++row ) {
            const CartesianDiagramDataCompressor::CachePosition position( row, column );
            const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            const QModelIndex sourceIndex = attributesModel()->mapToSource( point.index );

            const LineAttributes laCell = diagram()->lineAttributes( sourceIndex );
            const bool bDisplayCellArea = laCell.displayArea();

            double stackedValues = 0, nextValues = 0;
            for ( int column2 = column; column2 >= 0; --column2 )
/*                  column2 >= datasetDimension() - 1;
                  column2 -= datasetDimension() )*/
            {
                const CartesianDiagramDataCompressor::CachePosition position( row, column2 );
                const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
                const QModelIndex sourceIndex = attributesModel()->mapToSource( point.index );
                stackedValues += point.value;
                //qDebug() << valueForCell( iRow, iColumn2 );
                if ( row + 1 < rowCount ){
                    CartesianDiagramDataCompressor::CachePosition position( row + 1, column2 );
                    CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
                    nextValues += point.value;
                }
            }
            //qDebug() << stackedValues << endl;
            const QPointF nextPoint = ctx->coordinatePlane()->translate( QPointF( row, stackedValues ) );
            points << nextPoint;

            const QPointF ptNorthWest( nextPoint );
            const QPointF ptSouthWest(
                bDisplayCellArea
                ? ( bFirstDataset
                    ? ctx->coordinatePlane()->translate( QPointF( row, 0.0 ) )
                    : bottomPoints.at( row )
                    )
                : nextPoint );
            QPointF ptNorthEast;
            QPointF ptSouthEast;

            if ( row + 1 < rowCount ){
                QPointF toPoint = ctx->coordinatePlane()->translate( QPointF( row + 1, nextValues ) );
                lineList.append( LineAttributesInfo( sourceIndex, nextPoint, toPoint ) );
                ptNorthEast = toPoint;
                ptSouthEast =
                    bDisplayCellArea
                    ? ( bFirstDataset
                        ? ctx->coordinatePlane()->translate( QPointF( row + 1, 0.0 ) )
                        : bottomPoints.at( row + 1 )
                        )
                    : toPoint;
                if( areas.count() && laCell != laPreviousCell ){
                    paintAreas( ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
                    areas.clear();
                }
                if( bDisplayCellArea ){
                    QPolygonF poly;
                    poly << ptNorthWest << ptNorthEast << ptSouthEast << ptSouthWest;
                    areas << poly;
                    laPreviousCell = laCell;
                    indexPreviousCell = sourceIndex;
                }else{
                    //qDebug() << "no area shown for row"<<iRow<<"  column"<<iColumn;
                }
            }else{
                ptNorthEast = ptNorthWest;
                ptSouthEast = ptSouthWest;
            }

            const PositionPoints pts( ptNorthWest, ptNorthEast, ptSouthEast, ptSouthWest );
            appendDataValueTextInfoToList( diagram(), list, sourceIndex, pts,
                                           Position::NorthWest, Position::SouthWest,
                                           point.value );
        }
        if( areas.count() ){
            paintAreas( ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
            areas.clear();
        }
        bottomPoints = points;
        bFirstDataset = false;
    }
    paintElements( ctx, list, lineList, policy );
}
