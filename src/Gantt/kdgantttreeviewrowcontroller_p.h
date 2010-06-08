#ifndef KDGANTTTREEVIEWROWCONTROLLER_P_H
#define KDGANTTTREEVIEWROWCONTROLLER_P_H

#include "kdganttabstractrowcontroller.h"

#include <QTreeView>

class QAbstractProxyModel;

namespace KDGantt {
    class TreeViewRowController::Private {
    public:
        class HackTreeView : public QTreeView {
        public:
            using QTreeView::verticalOffset;
#if QT_VERSION >= 0x040300
            using QTreeView::rowHeight;
#else
            using QTreeView::indexRowSizeHint;
#endif
        };
        HackTreeView* treeview;
        QAbstractProxyModel* proxy;
    };
}

#endif /* KDGANTTTREEVIEWROWCONTROLLER_P_H */

