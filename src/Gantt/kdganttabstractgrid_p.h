#ifndef KDGANTTABSTRACTGRID_P_H
#define KDGANTTABSTRACTGRID_P_H

#include <QPointer>
#include <QAbstractItemModel>
#include <QPersistentModelIndex>

#include "kdganttabstractgrid.h"

namespace KDGantt {
    class AbstractGrid::Private {
    public:
        QPointer<QAbstractItemModel> model;
        QPersistentModelIndex root;
    };

    inline AbstractGrid::AbstractGrid( Private* d ) : _d( d ) {}
}

#endif /* KDGANTTABSTRACTGRID_P_H */

