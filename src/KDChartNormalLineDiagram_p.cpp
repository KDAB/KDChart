#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartLineDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartNormalLineDiagram_p.h"
#include "KDChartLineDiagramDataCompressor_p.h"

using namespace KDChart;

NormalLineDiagram::NormalLineDiagram( LineDiagram* d )
    : LineDiagramType( d )
{
}

LineDiagram::LineType NormalLineDiagram::type() const
{
    return LineDiagram::Normal;
}

const QPair<QPointF, QPointF> NormalLineDiagram::calculateDataBoundaries() const
{
    const int rowCount = compressor().modelDataRows();
    const int colCount = compressor().modelDataColumns();
    double xMin = 0;
    double xMax = rowCount -1;
    double yMin = 0;
    double yMax = 0;

    bool first = true;
    for( int i = 0; i < colCount; ++i ) {
        for ( int j = 0; j < rowCount; ++j ) {
            LineDiagramDataCompressor::CachePosition position( j, i );
            LineDiagramDataCompressor::DataPoint point = compressor().data( position );

            if ( first ) {
                    yMin = point.value;
                    yMax = point.value;
            } else {
                yMin = qMin( yMin, point.value );
                yMax = qMax( yMax, point.value );
            }

            first = false;
        }
    }

    QPointF bottomLeft( QPointF( xMin, qMin( 0.0, yMin ) ) );
    QPointF topRight( QPointF( xMax, yMax ) );
    return QPair<QPointF, QPointF> ( bottomLeft, topRight );
}

void NormalLineDiagram::paint(  PaintContext* ctx )
{
    reverseMapper().clear();
    // FIXME remove hungarian notation: iColumn, bOK, bValuesFound
    // FIXME do not use STL class names for variable names, like "list"
    // remove, we are not a bar diagram:
//     const bool shiftCountedXValuesByHalfSection =
//         (dynamic_cast< BarDiagram* >( diagram()->referenceDiagram() ) != 0);

//     const QPair<QPointF, QPointF> boundaries = diagram()->dataBoundaries(); // cached
//     const QPointF& bottomLeft = boundaries.first;
//     const QPointF& topRight = boundaries.second;
    Q_ASSERT( dynamic_cast<CartesianCoordinatePlane*>( ctx->coordinatePlane() ) );
    CartesianCoordinatePlane* plane = static_cast<CartesianCoordinatePlane*>( ctx->coordinatePlane() );
    const int columnCount = compressor().modelDataColumns();
    const int rowCount = compressor().modelDataRows();
    if ( columnCount == 0 || rowCount == 0 ) return; // maybe blank out the area?

    // FIXME integrate column index retrieval to compressor:
    int maxFound = 0;
//     {   // find the last column number that is not hidden
//         for( int iColumn =  datasetDimension() - 1;
//              iColumn <  columnCount;
//              iColumn += datasetDimension() )
//             if( ! diagram()->isHidden( iColumn ) )
//                 maxFound = iColumn;
//     }
    maxFound = columnCount;
    // ^^^ temp
    const int lastVisibleColumn = maxFound;
    // const int rowCount = attributesModel()->rowCount( attributesModelRootIndex() );

    DataValueTextInfoList list;
    LineAttributesInfoList lineList;
    LineAttributes::MissingValuesPolicy policy; // ???

    for( int iColumn  = 0; iColumn < columnCount; ++iColumn ) {
        LineAttributes laPreviousCell;
        LineDiagramDataCompressor::CachePosition previousCellPosition;

        for ( int row = 0; row < rowCount; ++row ) {
            LineDiagramDataCompressor::CachePosition position( row, iColumn );
            // get where to draw the line from:
            LineDiagramDataCompressor::DataPoint point = compressor().data( position );
            LineAttributes laCell;
            if ( row > 0 ) { // position 0 is not really painted, since it takes two points to make a line :-)
                LineDiagramDataCompressor::DataPoint lastPoint = compressor().data( previousCellPosition );
                // area corners, a + b are the line ends:
                QPointF a( plane->translate( QPointF( row - 1, lastPoint.value ) ) );
                QPointF b( plane->translate( QPointF( row, point.value ) ) );
                QPointF c( plane->translate( QPointF( row - 1, 0.0 ) ) );
                QPointF d( plane->translate( QPointF( row, 0.0 ) ) );
                // add the line to the list:
                laCell = diagram()->lineAttributes( point.index );
                lineList.append( LineAttributesInfo( point.index, a, b ) );
                // add data point labels:
                // ...
                // if necessary, add the area to the area list:
                QList<QPolygonF> areas;
                if ( laCell.displayArea() ) {
                    QPolygonF polygon;
                    polygon << a << b << d << c;
                    areas << polygon;
                }
                paintAreas( ctx, lastPoint.index, areas, laPreviousCell.transparency() );
            }
            // wrap it up:
            previousCellPosition = position;
            laPreviousCell = laCell;
        }
    }

    paintElements( ctx, list, lineList, policy );

/*
    bool bValuesFound = false;
    double lastValueX, lastValueY;
    double valueX, valueY;
    for ( int iRow = 0; iRow < rowCount; ++iRow ) {
        bool skipThisCell = false;
        // trying to find a fromPoint
        // FIXME this needs to use an index

        // all that policy stuff will be completely hidden in the compressor:
        // policy = getCellValues( iRow, iColumn,
//                                     shiftCountedXValuesByHalfSection,
//                                     valueX, valueY );
        switch( policy ){
        case LineAttributes::MissingValuesAreBridged:
            if( bValuesFound ){
                valueX = lastValueX;
                valueY = lastValueY;
            }else{
                skipThisCell = true;
            }
            break;
        case LineAttributes::MissingValuesHideSegments:
            skipThisCell = true;
            break;
        case LineAttributes::MissingValuesShownAsZero:
// fall through intended
        case LineAttributes::MissingValuesPolicyIgnored:
            lastValueX = valueX;
            lastValueY = valueY;
            bValuesFound = true;
            break;
        }
        if( ! skipThisCell ){
// trying to find a toPoint
            double nextValueX, nextValueY;
            bool foundToPoint = false;
            int iNextRow = iRow+1;
            while ( ! (foundToPoint || skipThisCell || iNextRow >= rowCount) ) {
                policy = getCellValues(
                    iNextRow, iColumn,
                    shiftCountedXValuesByHalfSection,
                    nextValueX, nextValueY );
                switch( policy ){
                case LineAttributes::MissingValuesAreBridged:
// The cell has no valid value, so we  make sure that
// this cell will not be processed by the next iteration
// of the iRow loop:
                    ++iRow;
                    break;
                case LineAttributes::MissingValuesHideSegments:
// The cell has no valid value, so we  make sure that
// this cell will not be processed by the next iteration
// of the iRow loop:
                    skipThisCell = true;
                    ++iRow;
                    break;
                case LineAttributes::MissingValuesShownAsZero:
// fall through intended
                case LineAttributes::MissingValuesPolicyIgnored:
                    foundToPoint = true;
                    break;
                }
                ++iNextRow;
            }
            if( ! skipThisCell ){
                const bool isPositive = (valueY >= 0.0);
                const QModelIndex index = diagram()->model()->index( iRow, iColumn, diagram()->rootIndex() );
                const LineAttributes laCell = diagram()->lineAttributes( index );
                const bool bDisplayCellArea = laCell.displayArea();

                QPointF fromPoint = ctx->coordinatePlane()->translate( QPointF( valueX, valueY ) );

                const QPointF ptNorthWest(
                    (bDisplayCellArea && ! isPositive)
                    ? ctx->coordinatePlane()->translate( QPointF( valueX, 0.0 ) )
                    : fromPoint );
                const QPointF ptSouthWest(
                    (bDisplayCellArea && isPositive)
                    ? ctx->coordinatePlane()->translate( QPointF( valueX, 0.0 ) )
                    : fromPoint );
//qDebug() << "--> ptNorthWest:" << ptNorthWest;
//qDebug() << "--> ptSouthWest:" << ptSouthWest;
                QPointF ptNorthEast;
                QPointF ptSouthEast;

                if( foundToPoint ){
                    QPointF toPoint = ctx->coordinatePlane()->translate( QPointF( nextValueX, nextValueY ) );
                    lineList.append( LineAttributesInfo( index, fromPoint, toPoint ) );
                    ptNorthEast =
                        (bDisplayCellArea && ! isPositive)
                        ? ctx->coordinatePlane()->translate( QPointF( nextValueX, 0.0 ) )
                        : toPoint;
                    ptSouthEast =
                        (bDisplayCellArea && isPositive)
                        ? ctx->coordinatePlane()->translate( QPointF( nextValueX, 0.0 ) )
                        : toPoint;
// we can't take as a condition the line attributes
// to be different from a cell to another.
// e.g the user should be able to have different brush
// which is not reflected in the line attributes
// see examples/Area which show such an option
                    if( areas.count() ) { // && laCell != laPreviousCell )
                        paintAreas( ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
                        areas.clear();
                    }
                    if( bDisplayCellArea ){
                        QPolygonF poly;
                        poly << ptNorthWest << ptNorthEast << ptSouthEast << ptSouthWest;
                        //qDebug() << "ptNorthWest:" << ptNorthWest;
                        //qDebug() << "ptNorthEast:" << ptNorthEast;
                        //qDebug() << "ptSouthEast:" << ptSouthEast;
                        //qDebug() << "ptSouthWest:" << ptSouthWest;
                        //qDebug() << "polygon:" << poly;
                        areas << poly;
                        laPreviousCell = laCell;
                        indexPreviousCell = index;
                    }
                }else{
                    ptNorthEast = ptNorthWest;
                    ptSouthEast = ptSouthWest;
                }

                const PositionPoints pts = isPositive ? PositionPoints( ptNorthWest, ptNorthEast, ptSouthEast, ptSouthWest )
                                           : PositionPoints( ptSouthWest, ptSouthEast, ptNorthEast, ptNorthWest );
                appendDataValueTextInfoToList( diagram(), list, index, pts,
                                               Position::NorthWest, Position::SouthWest,
                                               valueY );
            }
        }
    }
    if( areas.count() ){
        paintAreas( ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
        areas.clear();
    }
}
    paintElements( ctx, list, lineList, policy );
*/
}



