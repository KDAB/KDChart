#ifndef KDGANTTLISTVIEWROWCONTROLLER_P_H
#define KDGANTTLISTVIEWROWCONTROLLER_P_H

#include "kdganttlistviewrowcontroller.h"

#include <QListView>

namespace KDGantt {
    class ListViewRowController::Private {
    public:
        class HackListView : public QListView {
        public:
            using QListView::verticalOffset;
            using QListView::setViewportMargins;
        };

        Private(QListView* lv, QAbstractProxyModel* pm )
            : listview(lv), proxy(pm) {}
        QListView* listview;
        QAbstractProxyModel* proxy;
    };
}

#endif /* KDGANTTLISTVIEWROWCONTROLLER_P_H */

