#ifndef KDGANTTLISTVIEWROWCONTROLLER_H
#define KDGANTTLISTVIEWROWCONTROLLER_H

#include "kdganttabstractrowcontroller.h"

class QAbstractProxyModel;
class QListView;

namespace KDGantt {
    class KDCHART_EXPORT ListViewRowController : public AbstractRowController {
        KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(ListViewRowController)
    public:
	ListViewRowController( QListView* lv, QAbstractProxyModel* proxy );
        ~ListViewRowController();

        /*reimp*/ int headerHeight() const;
        /*reimp*/ int maximumItemHeight() const;
        /*reimp*/ int totalHeight() const;
        /*reimp*/ bool isRowVisible( const QModelIndex& idx ) const;
        /*reimp*/ bool isRowExpanded( const QModelIndex& idx ) const;
        /*reimp*/ Span rowGeometry( const QModelIndex& idx ) const;
        /*reimp*/ QModelIndex indexAt( int height ) const;
        /*reimp*/ QModelIndex indexAbove( const QModelIndex& idx ) const;
        /*reimp*/ QModelIndex indexBelow( const QModelIndex& idx ) const;
    };
}

#endif /* KDGANTTLISTVIEWROWCONTROLLER_H */

