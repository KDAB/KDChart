#ifndef KDGANTTCONSTRAINTPROXY_H
#define KDGANTTCONSTRAINTPROXY_H

#include "kdganttglobal.h"

#include <QPointer>

class QAbstractProxyModel;

namespace KDGantt {
    class Constraint;
    class ConstraintModel;

    class ConstraintProxy : public QObject {
        Q_OBJECT
    public:
        explicit ConstraintProxy( QObject* parent = 0 );
        virtual ~ConstraintProxy();

        void setSourceModel( ConstraintModel* src );
        void setDestinationModel( ConstraintModel* dest );
        void setProxyModel( QAbstractProxyModel* proxy );

        ConstraintModel* sourceModel() const;
        ConstraintModel* destinationModel() const;
        QAbstractProxyModel* proxyModel() const;


    private Q_SLOTS:

        void slotSourceConstraintAdded( const KDGantt::Constraint& );
        void slotSourceConstraintRemoved( const KDGantt::Constraint& );

        void slotDestinationConstraintAdded( const KDGantt::Constraint& );
        void slotDestinationConstraintRemoved( const KDGantt::Constraint& );

        void slotLayoutChanged();

    private:
        void copyFromSource();

        QPointer<QAbstractProxyModel> m_proxy;
        QPointer<ConstraintModel> m_source;
        QPointer<ConstraintModel> m_destination;
    };
}

#endif /* KDGANTTCONSTRAINTPROXY_H */

