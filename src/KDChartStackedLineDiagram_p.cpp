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
    const int rowCount = attributesModel->rowCount( attributesModelRootIndex() );
    const int colCount = attributesModel->columnCount( attributesModelRootIndex() );
    double xMin = 0;
    double xMax = rowCount -1;
    double yMin = 0, yMax = 0;
    bool bOK;

    bool bStarting = true;
    for ( int j=0; j< rowCount; ++j ) {
        // calculate sum of values per column - Find out stacked Min/Max
        double stackedValues = 0;
        for( int i = datasetDimension-1; i < colCount; i += datasetDimension ) {
            const double value = valueForCellTesting( j, i, bOK );
            if( bOK )
                stackedValues += value;
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
    const QPair<QPointF, QPointF> boundaries = diagram->dataBoundaries();
    const QPointF bottomLeft = boundaries.first;
    const QPointF topRight = boundaries.second;

    int maxFound = 0;
    {   // find the last column number that is not hidden
        const int columnCount = attributesModel->columnCount(attributesModelRootIndex());
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

    //FIXME(khz): add LineAttributes::MissingValuesPolicy support for LineDiagram::Stacked and ::Percent

    const bool isPercentMode = type() == LineDiagram::Percent;
    double maxValue = 100; // always 100%
    double sumValues = 0;
    QVector <double > percentSumValues;

    //calculate sum of values for each column and store
    if( isPercentMode ){
        for ( int j=0; j<rowCount ; ++j ) {
            for( int i =  datasetDimension-1;
                 i <= lastVisibleColumn;
                 i += datasetDimension ) {
                double tmpValue = valueForCell( j, i );
                if ( tmpValue > 0 )
                    sumValues += tmpValue;
                if ( i == lastVisibleColumn ) {
                    percentSumValues <<  sumValues ;
                    sumValues = 0;
                }
            }
        }
    }

    QList<QPointF> bottomPoints;
    bool bFirstDataset = true;

    for( int iColumn =  datasetDimension-1;
         iColumn <= lastVisibleColumn;
         iColumn += datasetDimension ) {

        //display area can be set by dataset ( == column) and/or by cell
        LineAttributes laPreviousCell; // by default no area is drawn
        QModelIndex indexPreviousCell;
        QList<QPolygonF> areas;
        QList<QPointF> points;

        for ( int iRow = 0; iRow< rowCount; ++iRow ) {
            const QModelIndex index = diagram->model()->index( iRow, iColumn, diagram->rootIndex() );
            const LineAttributes laCell = diagram->lineAttributes( index );
            const bool bDisplayCellArea = laCell.displayArea();

            double stackedValues = 0, nextValues = 0;
            for ( int iColumn2 = iColumn;
                  iColumn2 >= datasetDimension-1;
                  iColumn2 -= datasetDimension )
            {
                const double val = valueForCell( iRow, iColumn2 );
                if( val > 0 || ! isPercentMode )
                    stackedValues += val;
                //qDebug() << valueForCell( iRow, iColumn2 );
                if ( iRow+1 < rowCount ){
                    const double val = valueForCell( iRow+1, iColumn2 );
                    if( val > 0 || ! isPercentMode )
                        nextValues += val;
                }
            }
            if( isPercentMode ){
                if ( percentSumValues.at( iRow ) != 0  )
                    stackedValues = stackedValues / percentSumValues.at( iRow ) * maxValue;
                else
                    stackedValues = 0.0;
            }
            //qDebug() << stackedValues << endl;
            QPointF nextPoint = plane->translate( QPointF( iRow, stackedValues ) );
            points << nextPoint;

            const QPointF ptNorthWest( nextPoint );
            const QPointF ptSouthWest(
                bDisplayCellArea
                ? ( bFirstDataset
                    ? plane->translate( QPointF( iRow, 0.0 ) )
                    : bottomPoints.at( iRow )
                    )
                : nextPoint );
            QPointF ptNorthEast;
            QPointF ptSouthEast;

            if ( iRow+1 < rowCount ){
                if( isPercentMode ){
                    if ( percentSumValues.at( iRow+1 ) != 0  )
                        nextValues = nextValues / percentSumValues.at( iRow+1 ) * maxValue;
                    else
                        nextValues = 0.0;
                }
                QPointF toPoint = plane->translate( QPointF( iRow+1, nextValues ) );
                lineList.append( LineAttributesInfo( index, nextPoint, toPoint ) );
                ptNorthEast = toPoint;
                ptSouthEast =
                    bDisplayCellArea
                    ? ( bFirstDataset
                        ? plane->translate( QPointF( iRow+1, 0.0 ) )
                        : bottomPoints.at( iRow+1 )
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
                    indexPreviousCell = index;
                }else{
                    //qDebug() << "no area shown for row"<<iRow<<"  column"<<iColumn;
                }
            }else{
                ptNorthEast = ptNorthWest;
                ptSouthEast = ptSouthWest;
            }

            const PositionPoints pts( ptNorthWest, ptNorthEast, ptSouthEast, ptSouthWest );
            appendDataValueTextInfoToList( diagram, list, index, pts,
                                           Position::NorthWest, Position::SouthWest,
                                           valueForCell( iRow, iColumn ) );
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



