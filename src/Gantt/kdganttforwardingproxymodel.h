#ifndef KDGANTTFORWARDINGPROXYMODEL_H
#define KDGANTTFORWARDINGPROXYMODEL_H

#include <QAbstractProxyModel>

#include "kdganttglobal.h"

namespace KDGantt {
    class KDCHART_EXPORT ForwardingProxyModel : public QAbstractProxyModel {
        Q_OBJECT
        Q_DISABLE_COPY(ForwardingProxyModel)
    public:
        explicit ForwardingProxyModel( QObject* parent=0 );
        virtual ~ForwardingProxyModel();

        /*reimp*/ QModelIndex mapFromSource ( const QModelIndex & sourceIndex ) const;
        /*reimp*/ QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;

        /*reimp*/ void setSourceModel( QAbstractItemModel* model );

        /*reimp*/ QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
        /*reimp*/ QModelIndex parent( const QModelIndex& idx ) const;

        /*reimp*/ int rowCount( const QModelIndex& idx = QModelIndex() ) const;
        /*reimp*/ int columnCount( const QModelIndex& idx = QModelIndex() ) const;

        /*reimp*/ bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

        /*reimp*/ QMimeData *mimeData(const QModelIndexList &indexes) const;
        /*reimp*/ bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
        /*reimp*/ QStringList mimeTypes() const;
        /*reimp*/ Qt::DropActions supportedDropActions() const;
    
    protected Q_SLOTS:
        virtual void sourceModelAboutToBeReset();
        virtual void sourceModelReset();
        virtual void sourceLayoutAboutToBeChanged();
        virtual void sourceLayoutChanged();
        virtual void sourceDataChanged( const QModelIndex& from, const QModelIndex& to );
        virtual void sourceColumnsAboutToBeInserted( const QModelIndex& idx, int start, int end );
        virtual void sourceColumnsInserted( const QModelIndex& idx, int start, int end );
        virtual void sourceColumnsAboutToBeRemoved( const QModelIndex& idx, int start, int end );
        virtual void sourceColumnsRemoved( const QModelIndex& idx, int start, int end );
        virtual void sourceRowsAboutToBeInserted( const QModelIndex& idx, int start, int end );
        virtual void sourceRowsInserted( const QModelIndex& idx, int start, int end );
        virtual void sourceRowsAboutToBeRemoved( const QModelIndex&, int start, int end );
        virtual void sourceRowsRemoved( const QModelIndex&, int start, int end );
    };
}

#endif /* KDGANTTFORWARDINGPROXYMODEL_H */

