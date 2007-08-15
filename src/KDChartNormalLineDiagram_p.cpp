#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartLineDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartNormalLineDiagram_p.h"

using namespace KDChart;

NormalLineDiagram::NormalLineDiagram( LineDiagram* d )
    : LineDiagramType( d )
{
}

LineDiagram::LineType NormalLineDiagram::type() const
{
    return LineDiagram::Normal;
}

const QPair< QPointF, QPointF > NormalLineDiagram::calculateDataBoundaries() const
{
    const int rowCount = compressor().modelDataRows();
    const int colCount = compressor().modelDataColumns();
    double xMin = 0;
    double xMax = rowCount -1;
    double yMin = 0;
    double yMax = 0;

    bool first = true;
    for( int column = 0; column < colCount; ++column )
    {
        for ( int row = 0; row < rowCount; ++row )
        {
            const CartesianDiagramDataCompressor::CachePosition position( row, column );
            const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );

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

    const QPointF bottomLeft( QPointF( xMin, qMin( 0.0, yMin ) ) );
    const QPointF topRight( QPointF( xMax, yMax ) );
    return QPair< QPointF, QPointF >( bottomLeft, topRight );
}

void NormalLineDiagram::paint( PaintContext* ctx )
{
    reverseMapper().clear();
    Q_ASSERT( dynamic_cast<CartesianCoordinatePlane*>( ctx->coordinatePlane() ) );
    CartesianCoordinatePlane* plane = static_cast<CartesianCoordinatePlane*>( ctx->coordinatePlane() );
    const int columnCount = compressor().modelDataColumns();
    const int rowCount = compressor().modelDataRows();
    if ( columnCount == 0 || rowCount == 0 ) return; // maybe blank out the area?

// FIXME integrate column index retrieval to compressor:
// the compressor should only pass through visiblel columns
    int maxFound = 0;
//     {   // find the last column number that is not hidden
//         for( int column =  datasetDimension() - 1;
//              column <  columnCount;
//              column += datasetDimension() )
//             if( ! diagram()->isHidden( column ) )
//                 maxFound = column;
//     }
    maxFound = columnCount;
    // ^^^ temp

    DataValueTextInfoList textInfoList;
    LineAttributesInfoList lineList;
    LineAttributes::MissingValuesPolicy policy; // ???

    for( int column  = 0; column < columnCount; ++column ) {
        LineAttributes laPreviousCell;
        CartesianDiagramDataCompressor::CachePosition previousCellPosition;

        for ( int row = 0; row < rowCount; ++row ) {
            const CartesianDiagramDataCompressor::CachePosition position( row, column );
            // get where to draw the line from:
            const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
            LineAttributes laCell;
            if ( row > 0 ) { // position 0 is not really painted, since it takes two points to make a line :-)
                const QModelIndex sourceIndex = attributesModel()->mapToSource( point.index );
                const CartesianDiagramDataCompressor::DataPoint lastPoint = compressor().data( previousCellPosition );
                // area corners, a + b are the line ends:
                const QPointF a( plane->translate( QPointF( row - 1, lastPoint.value ) ) );
                const QPointF b( plane->translate( QPointF( row, point.value ) ) );
                const QPointF c( plane->translate( QPointF( row - 1, 0.0 ) ) );
                const QPointF d( plane->translate( QPointF( row, 0.0 ) ) );
                // add the line to the list:
                laCell = diagram()->lineAttributes( sourceIndex );
                // add data point labels:
                const PositionPoints pts = PositionPoints( b, a, d, c );
                // if necessary, add the area to the area list:
                QList<QPolygonF> areas;
                if ( laCell.displayArea() ) {
                    QPolygonF polygon;
                    polygon << a << b << d << c;
                    areas << polygon;
                }
                // add the pieces to painting if this is not hidden:
                if ( ! point.hidden ) {
                    appendDataValueTextInfoToList( diagram(), textInfoList, sourceIndex, pts,
                                                   Position::NorthWest, Position::SouthWest,
                                                   point.value );
                    paintAreas( ctx, attributesModel()->mapToSource( lastPoint.index ), areas, laCell.transparency() );
                    lineList.append( LineAttributesInfo( sourceIndex, a, b ) );
                }
            }
            // wrap it up:
            previousCellPosition = position;
            laPreviousCell = laCell;
        }
    }

    paintElements( ctx, textInfoList, lineList, policy );
}
