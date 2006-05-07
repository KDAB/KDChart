#include "AlternatingColumnsProxyModel.h"

AlternatingColumnsProxyModel::AlternatingColumnsProxyModel ()
    : QAbstractProxyModel()
{
}

QModelIndex AlternatingColumnsProxyModel::index ( int row, int column, const QModelIndex &parent ) const
{   // this proxy model does not maintain own indexes, it just returns indexes
    // to the source model and passes all indexes through  in the other
    // methods:
    return sourceModel()->index ( row * 2 + column % 2, column, parent );
}

QModelIndex AlternatingColumnsProxyModel::parent ( const QModelIndex& child ) const
{
    return child.parent();
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
    return sourceModel()->data( index, role );
}

QModelIndex AlternatingColumnsProxyModel::mapToSource ( const QModelIndex& proxyIndex ) const
{
    return proxyIndex;
}

QModelIndex AlternatingColumnsProxyModel::mapFromSource( const QModelIndex& sourceIndex ) const
{
    return sourceIndex;
}



