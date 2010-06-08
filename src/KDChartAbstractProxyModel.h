#ifndef KDCHARTABSTRACTPROXYMODEL_H
#define KDCHARTABSTRACTPROXYMODEL_H

#include <QAbstractProxyModel>

#include "KDChartGlobal.h"

namespace KDChart
{
    /**
      * @brief Base class for all proxy models used inside KD Chart
      * \internal
      */
    class KDCHART_EXPORT AbstractProxyModel : public QAbstractProxyModel
    {
        Q_OBJECT
    public:
        explicit AbstractProxyModel( QObject* parent = 0 );

        /*! \reimpl */ 
        QModelIndex mapFromSource( const QModelIndex & sourceIndex ) const;
        /*! \reimpl */ 
        QModelIndex mapToSource( const QModelIndex &proxyIndex ) const;

        /*! \reimpl */
        QModelIndex index( int row, int col, const QModelIndex& index ) const;
        /*! \reimpl */ 
        QModelIndex parent( const QModelIndex& index ) const;
    };
}

#endif /* KDCHARTABSTRACTPROXYMODEL_H */
