#ifndef KDGANTTSUMMARYHANDLINGPROXYMODEL_H
#define KDGANTTSUMMARYHANDLINGPROXYMODEL_H

#include "kdganttforwardingproxymodel.h"

namespace KDGantt {
    class KDCHART_EXPORT SummaryHandlingProxyModel : public ForwardingProxyModel {
        Q_OBJECT
        KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC( SummaryHandlingProxyModel )
    public:
        explicit SummaryHandlingProxyModel( QObject* parent=0 );
        virtual ~SummaryHandlingProxyModel();

        /*reimp*/ void setSourceModel( QAbstractItemModel* model );

        /*reimp*/ QVariant data( const QModelIndex& proxyIndex, int role = Qt::DisplayRole) const;
        /*reimp*/ bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

        /*reimp*/ Qt::ItemFlags flags( const QModelIndex& idx ) const;

    protected:
        /*reimp*/ void sourceModelReset();
        /*reimp*/ void sourceLayoutChanged();
        /*reimp*/ void sourceDataChanged( const QModelIndex& from, const QModelIndex& to );
        /*reimp*/ void sourceColumnsAboutToBeInserted( const QModelIndex& idx, int start, int end );
        /*reimp*/ void sourceColumnsAboutToBeRemoved( const QModelIndex& idx, int start, int end );
        /*reimp*/ void sourceRowsAboutToBeInserted( const QModelIndex& idx, int start, int end );
        /*reimp*/ void sourceRowsAboutToBeRemoved( const QModelIndex&, int start, int end );
    };
}

#endif /* KDGANTTSUMMARYHANDLINGPROXYMODEL_H */

