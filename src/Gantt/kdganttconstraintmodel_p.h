#ifndef KDGANTTCONSTRAINTMODEL_P_H
#define KDGANTTCONSTRAINTMODEL_P_H

#include "kdganttconstraintmodel.h"

#include <QList>
#include <QMultiHash>
#include <QPersistentModelIndex>

namespace KDGantt {
    class ConstraintModel::Private {
    public:
        Private();

        void addConstraintToIndex( const QModelIndex& idx, const Constraint& c );
        void removeConstraintFromIndex( const QModelIndex& idx,  const Constraint& c );

        typedef QMultiHash<QPersistentModelIndex,Constraint> IndexType;

        QList<Constraint> constraints;
        IndexType indexMap;
    };
}

#endif /* KDGANTTCONSTRAINTMODEL_P_H */

