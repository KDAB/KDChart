#include <QModelIndex>

#include "KDChartBarDiagram.h"
#include "KDChartLineDiagram.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartLineDiagramType_p.h"

using namespace KDChart;

NormalLineDiagram::NormalLineDiagram()
{
}

LineDiagram::LineType NormalLineDiagram::type() const
{
    return LineDiagram::Normal;
}

const QPair<QPointF, QPointF> NormalLineDiagram::calculateDataBoundaries() const
{
    const int rowCount = attributesModel->rowCount( attributesModelRootIndex() );
    const int colCount = attributesModel->columnCount( attributesModelRootIndex() );
    double xMin = 0;
    double xMax = rowCount -1;
    double yMin = 0, yMax = 0;
    bool bOK;

    bool bStarting = true;
    for( int i = datasetDimension - 1; i < colCount; i += datasetDimension ) {
        for ( int j=0; j< rowCount; ++j ) {
            const double value = valueForCellTesting( j, i, bOK );
            double xvalue;
            if( datasetDimension > 1 && bOK )
                xvalue = valueForCellTesting( j, i-1, bOK );
            if( bOK ){
                if( bStarting ){
                    yMin = value;
                    yMax = value;
                }else{
                    yMin = qMin( yMin, value );
                    yMax = qMax( yMax, value );
                }
                if ( datasetDimension > 1 ) {
                    if( bStarting ){
                        xMin = xvalue;
                        xMax = xvalue;
                    }else{
                        xMin = qMin( xMin, xvalue );
                        xMax = qMax( xMax, xvalue );
                    }
                }
                bStarting = false;
            }
        }
    }

    QPointF bottomLeft( QPointF( xMin, yMin ) );
    QPointF topRight(   QPointF( xMax, yMax ) );
    return QPair<QPointF, QPointF> ( bottomLeft, topRight );
}

void NormalLineDiagram::paint(  PaintContext* ctx )
{
    const bool shiftCountedXValuesByHalfSection =
        (dynamic_cast< BarDiagram* >( diagram->referenceDiagram() ) != 0);

    //QTime t = QTime::currentTime();

    const QPair<QPointF, QPointF> boundaries = diagram->dataBoundaries();
    const QPointF bottomLeft = boundaries.first;
    const QPointF topRight = boundaries.second;

    int maxFound = 0;
    {   // find the last column number that is not hidden
        const int columnCount = attributesModel->columnCount( attributesModelRootIndex() );
        for( int iColumn =  datasetDimension-1;
             iColumn <  columnCount;
             iColumn += datasetDimension )
            if( ! diagram->isHidden( iColumn ) )
                maxFound = iColumn;
    }
    const int lastVisibleColumn = maxFound;
    const int rowCount = attributesModel->rowCount( attributesModelRootIndex() );

    DataValueTextInfoList list;
    LineAttributesInfoList lineList;
    LineAttributes::MissingValuesPolicy policy;


    for( int iColumn  = datasetDimension-1;
         iColumn <= lastVisibleColumn;
         iColumn += datasetDimension ) {
        //display area can be set by dataset ( == column) and/or by cell
        LineAttributes laPreviousCell; // by default no area is drawn
        QModelIndex indexPreviousCell;
        QList<QPolygonF> areas;

        bool bValuesFound = false;
        double lastValueX, lastValueY;
        double valueX, valueY;
        for ( int iRow = 0; iRow < rowCount; ++iRow ) {
            bool skipThisCell = false;
            // trying to find a fromPoint
            policy = getCellValues( iRow, iColumn,
                                    shiftCountedXValuesByHalfSection,
                                    valueX, valueY );
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
                    const QModelIndex index = diagram->model()->index( iRow, iColumn, diagram->rootIndex() );
                    const LineAttributes laCell = diagram->lineAttributes( index );
                    const bool bDisplayCellArea = laCell.displayArea();

                    QPointF fromPoint = plane->translate( QPointF( valueX, valueY ) );

                    const QPointF ptNorthWest(
                        (bDisplayCellArea && ! isPositive)
                        ? plane->translate( QPointF( valueX, 0.0 ) )
                        : fromPoint );
                    const QPointF ptSouthWest(
                        (bDisplayCellArea && isPositive)
                        ? plane->translate( QPointF( valueX, 0.0 ) )
                        : fromPoint );
                    //qDebug() << "--> ptNorthWest:" << ptNorthWest;
                    //qDebug() << "--> ptSouthWest:" << ptSouthWest;
                    QPointF ptNorthEast;
                    QPointF ptSouthEast;

                    if( foundToPoint ){
                        QPointF toPoint = plane->translate( QPointF( nextValueX, nextValueY ) );
                        lineList.append( LineAttributesInfo( index, fromPoint, toPoint ) );
                        ptNorthEast =
                            (bDisplayCellArea && ! isPositive)
                            ? plane->translate( QPointF( nextValueX, 0.0 ) )
                            : toPoint;
                        ptSouthEast =
                            (bDisplayCellArea && isPositive)
                            ? plane->translate( QPointF( nextValueX, 0.0 ) )
                            : toPoint;
                        // we can't take as a condition the line attributes
                        // to be different from a cell to another.
                        // e.g the user should be able to have different brush
                        // which is not reflected in the line attributes
                        // see examples/Area which show such an option
                        if( areas.count() /*&& laCell != laPreviousCell*/ ){
                            paintAreas( diagram, ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
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

                    const PositionPoints pts( ptNorthWest, ptNorthEast, ptSouthEast, ptSouthWest );
                    appendDataValueTextInfoToList( diagram, list, index, pts,
                                                   Position::NorthWest, Position::SouthWest,
                                                   valueY );
                }
            }
        }
        if( areas.count() ){
            paintAreas( diagram, ctx, indexPreviousCell, areas, laPreviousCell.transparency() );
            areas.clear();
        }
    }
}



