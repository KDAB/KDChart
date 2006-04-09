#include <QtDebug>

#include "KDChartDatasetProxyModel.h"

using namespace KDChart;

DatasetProxyModel::DatasetProxyModel (QObject* parent)
    : QSortFilterProxyModel ( parent )
{
}

void DatasetProxyModel::setDatasetRowDescriptionVector (
    const DatasetDescriptionVector& configuration )
{
    Q_ASSERT_X ( sourceModel(), "DatasetProxyModel::setDatasetRowDescriptionVector",
                 "A source model must be set before the selection can be configured." );
    initializeDatasetDecriptors ( configuration, sourceModel()->rowCount(),
                                  mRowSrcToProxyMap,  mRowProxyToSrcMap );
    clear(); // clear emits layoutChanged()
}

void DatasetProxyModel::setDatasetColumnDescriptionVector (
    const DatasetDescriptionVector& configuration )
{
    Q_ASSERT_X ( sourceModel(), "DatasetProxyModel::setDatasetColumnDescriptionVector",
                 "A source model must be set before the selection can be configured." );
    initializeDatasetDecriptors ( configuration, sourceModel()->columnCount(),
                                  mColSrcToProxyMap, mColProxyToSrcMap );
    clear(); // clear emits layoutChanged()
}

void DatasetProxyModel::setDatasetDescriptionVectors (
    const DatasetDescriptionVector& rowConfig,
    const DatasetDescriptionVector& columnConfig )
{
    setDatasetRowDescriptionVector( rowConfig );
    setDatasetColumnDescriptionVector ( columnConfig );
}

QModelIndex DatasetProxyModel::mapFromSource ( const QModelIndex & sourceIndex ) const
{
    Q_ASSERT_X ( sourceModel(), "DatasetProxyModel::mapFromSource", "A source "
                 "model must be set before the selection can be configured." );

    if ( mRowSrcToProxyMap.size() == 0 && mColSrcToProxyMap.size() == 0 )
    {
        return createIndex ( sourceIndex.row(), sourceIndex.column(),
                             sourceIndex.internalPointer() );
    } else {
        int row = mapSourceRowToProxy ( sourceIndex.row() );
        int column = mapSourceColumnToProxy ( sourceIndex.column() );
        return createIndex ( row, column, sourceIndex.internalPointer() );
    }
}

QModelIndex DatasetProxyModel::mapToSource ( const QModelIndex& proxyIndex ) const
{
    Q_ASSERT_X ( sourceModel(), "DatasetProxyModel::mapToSource", "A source "
                 "model must be set before the selection can be configured." );

    if ( mRowSrcToProxyMap.size() == 0 && mColSrcToProxyMap.size() == 0 )
    {
        return createIndex ( proxyIndex.row(),  proxyIndex.column(),
                             proxyIndex.internalPointer() );
    } else {
        int row = mapProxyRowToSource ( proxyIndex.row() );
        int column = mapProxyColumnToSource ( proxyIndex.column() );
        return createIndex ( row, column, proxyIndex.internalPointer() );
    }
}

bool DatasetProxyModel::filterAcceptsRow ( int sourceRow,
                                           const QModelIndex & ) const
{
    if ( mRowSrcToProxyMap.size() == 0 )
    {   // no row mapping set, all rows are passed down:
        return true;
    } else {
        Q_ASSERT ( sourceModel() );
        Q_ASSERT ( mRowSrcToProxyMap.size() == sourceModel()->rowCount() );
        if ( mRowSrcToProxyMap[sourceRow] == -1 )
        {   // this row is explicitly not accepted:
            return false;
        } else {
            Q_ASSERT ( mRowSrcToProxyMap[sourceRow] >= 0
                       && mRowSrcToProxyMap[sourceRow] < mRowSrcToProxyMap.size() );
            return true;
        }
    }
}

bool DatasetProxyModel::filterAcceptsColumn ( int sourceColumn,
                                              const QModelIndex & ) const
{
    if ( mColSrcToProxyMap.size() == 0 )
    {   // no column mapping set up yet, all columns are passed down:
        return true;
    } else {
        Q_ASSERT ( sourceModel() );
        Q_ASSERT ( mColSrcToProxyMap.size() == sourceModel()->columnCount() );
        if ( mColSrcToProxyMap[sourceColumn] == -1 )
        {   // this column is explicitly not accepted:
            return false;
        } else {
            Q_ASSERT ( mColSrcToProxyMap[sourceColumn] >= 0
                       && mColSrcToProxyMap[sourceColumn] < mColSrcToProxyMap.size() );
            return true;
        }
    }
}

int DatasetProxyModel::mapProxyRowToSource ( const int& proxyRow ) const
{
    if ( mRowProxyToSrcMap.size() == 0 )
    {   // if no row mapping is set, we pass down the row:
        return proxyRow;
    } else {
        Q_ASSERT ( proxyRow >= 0 && proxyRow < mRowProxyToSrcMap.size() );
        return mRowProxyToSrcMap[ proxyRow ];
    }
}

int DatasetProxyModel::mapProxyColumnToSource ( const int& proxyColumn ) const
{
    if ( mColProxyToSrcMap.size() == 0 )
    {   // if no column mapping is set, we pass down the column:
        return proxyColumn;
    } else {
        Q_ASSERT ( proxyColumn >= 0 && proxyColumn < mColProxyToSrcMap.size() );
        return mColProxyToSrcMap[ proxyColumn ];
    }
}

int DatasetProxyModel::mapSourceRowToProxy ( const int& sourceRow ) const
{
    if ( mRowSrcToProxyMap.size() == 0 )
    {
        return sourceRow;
    } else {
        Q_ASSERT ( sourceRow >= 0 && sourceRow < mRowSrcToProxyMap.size() );
        return mRowSrcToProxyMap[sourceRow];
    }
}

int DatasetProxyModel::mapSourceColumnToProxy ( const int& sourceColumn ) const
{
    if ( mColSrcToProxyMap.size() == 0 )
    {
        return sourceColumn;
    } else {
        Q_ASSERT ( sourceColumn >= 0 && sourceColumn < mColSrcToProxyMap.size() );
        return mColSrcToProxyMap[sourceColumn];
    }
}

void DatasetProxyModel::resetDatasetDescriptions()
{
    mRowSrcToProxyMap.clear();
    mRowProxyToSrcMap.clear();
    mColSrcToProxyMap.clear();
    mColProxyToSrcMap.clear();
    clear();
}

QVariant DatasetProxyModel::data(const QModelIndex &index, int role) const
{
    return sourceModel()->data( mapToSource ( index ), role );
}

QVariant DatasetProxyModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal )
    {
        if ( mapProxyColumnToSource ( section ) == -1 )
        {
            return QVariant();
        } else {
            return sourceModel()->headerData ( mapProxyColumnToSource ( section ),
                                                       orientation,  role );
        }
    } else {
        if ( mapProxyRowToSource ( section ) == -1 )
        {
            return QVariant();
        } else {
            return sourceModel()->headerData ( mapProxyRowToSource ( section ),
                                                       orientation, role );
        }
    }
}

void DatasetProxyModel::initializeDatasetDecriptors (
    const DatasetDescriptionVector& inConfiguration,
    const int sourceCount,
    DatasetDescriptionVector& outSourceToProxyMap,
    DatasetDescriptionVector& outProxyToSourceMap )
{
    // in the current mapping implementation, the proxy-to-source map is
    // identical to the configuration vector:
    outProxyToSourceMap = inConfiguration;
    outSourceToProxyMap.fill ( -1,  sourceCount );

    for ( int index = 0; index < inConfiguration.size(); ++index )
    {   // make sure the values in inConfiguration point to columns in the
        // source model:
        Q_ASSERT_X ( inConfiguration[index] >= 0
                   && inConfiguration[index] < sourceCount,
                     "DatasetProxyModel::initializeDatasetDecriptors",
                     "column index outside of source model" );
        Q_ASSERT_X ( outSourceToProxyMap[inConfiguration[index]] == -1 ,
                     "DatasetProxyModel::initializeDatasetDecriptors",
                     "no duplicates allowed in mapping configuration, mapping has to be revertible" );
        outSourceToProxyMap[inConfiguration[index]] = index;
    }
}

void DatasetProxyModel::setSourceModel (QAbstractItemModel *sourceModel)
{
    QSortFilterProxyModel::setSourceModel ( sourceModel );
    connect ( sourceModel,  SIGNAL ( layoutChanged() ),
              SLOT( resetDatasetDescriptions() ) );

    connect (sourceModel, SIGNAL ( dataChanged( const QModelIndex&, const QModelIndex&) ), SLOT(testSlot() ));
    
    resetDatasetDescriptions();
}

void DatasetProxyModel::testSlot()
{
    qDebug() << "fooooooooooooooooooooooooobbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaaaaaarrrrrrrrrrrrrrrrrrrrr";
}
