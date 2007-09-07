#include "AlternatingColumnsProxyModel.h"

AlternatingColumnsProxyModel::AlternatingColumnsProxyModel ()
    : QAbstractProxyModel()
{
}

QModelIndex AlternatingColumnsProxyModel::index ( int row, int column, const QModelIndex& ) const
{
    return createIndex ( row, column );
}

QModelIndex AlternatingColumnsProxyModel::parent ( const QModelIndex& ) const
{
    return QModelIndex(); // this is a 2-dimensional model
}

int AlternatingColumnsProxyModel::rowCount( const QModelIndex &parent ) const
{
    return sourceModel()->rowCount( parent ) / 2;
}

int AlternatingColumnsProxyModel::columnCount( const QModelIndex &parent ) const
{
    return sourceModel()->columnCount( parent );
}

QVariant AlternatingColumnsProxyModel::data( const QModelIndex& index, int role ) const
{
    return sourceModel()->data( mapToSource ( index ), role );
}

QModelIndex AlternatingColumnsProxyModel::mapToSource ( const QModelIndex& proxyIndex ) const
{
    return createIndex ( proxyIndex.row() * 2 + proxyIndex.column() % 2, proxyIndex.column() );
}

QModelIndex AlternatingColumnsProxyModel::mapFromSource( const QModelIndex& sourceIndex ) const
{
    return createIndex( sourceIndex.row() / 2 - sourceIndex.column() % 2, sourceIndex.column() );
}



