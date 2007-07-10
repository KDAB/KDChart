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

    DataValueTextInfoList textInfoList;
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
                const PositionPoints pts = point.value > 0 ? PositionPoints( b, a, d, c ) : PositionPoints( d, c, b, a );
                appendDataValueTextInfoToList( diagram(), textInfoList, point.index, pts,
                                               Position::NorthWest, Position::SouthWest,
                                               point.value );
                // if necessary, add the area to the area list:
                QList<QPolygonF> areas;
                if ( laCell.displayArea() ) {
                    QPolygonF polygon;
                    polygon << a << b << d << c;
                    areas << polygon;
                }
                paintAreas( ctx, lastPoint.index, areas, laCell.transparency() );
            }
            // wrap it up:
            previousCellPosition = position;
            laPreviousCell = laCell;
        }
    }

    paintElements( ctx, textInfoList, lineList, policy );
}



