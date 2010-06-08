#ifndef KDGANTTPROXYMODEL_P_H
#define KDGANTTPROXYMODEL_P_H

namespace KDGantt {
    class ProxyModel::Private {
    public:
        explicit Private( ProxyModel* _q );

        QHash<int, int> columnMap;
        QHash<int, int> roleMap;
        bool calendarMode;
    };
}

#endif /* KDGANTTPROXYMODEL_P_H */

