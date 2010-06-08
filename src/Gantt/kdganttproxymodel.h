#ifndef KDGANTTPROXYMODEL_H
#define KDGANTTPROXYMODEL_H

#include "kdganttforwardingproxymodel.h"

namespace KDGantt {
    class KDCHART_EXPORT ProxyModel : public ForwardingProxyModel {
        Q_OBJECT
        Q_DISABLE_COPY(ProxyModel)
        KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC( ProxyModel )
    public:
        explicit ProxyModel( QObject* parent=0 );
        virtual ~ProxyModel();

        void setColumn( int ganttrole, int col );
        void setRole( int ganttrole, int role );

        int column( int ganttrole ) const;
        int role( int ganttrole ) const;

#if 0
        void setCalendarMode( bool enable );
        bool calendarMode() const;
#endif

        /*reimp*/ QModelIndex mapFromSource( const QModelIndex& idx) const;
        /*reimp*/ QModelIndex mapToSource( const QModelIndex& proxyIdx ) const;

        /*reimp*/ int rowCount( const QModelIndex& idx ) const;
        /*reimp*/ int columnCount( const QModelIndex& idx ) const;

        /*reimp*/ QVariant data( const QModelIndex& idx, int role = Qt::DisplayRole ) const;
        /*reimp*/ bool setData( const QModelIndex& idx, const QVariant& value, int role=Qt::EditRole );
    };
}

#endif /* KDGANTTPROXYMODEL_H */

