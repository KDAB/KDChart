#ifndef KDGANTTTREEVIEWROWCONTROLLER_H
#define KDGANTTTREEVIEWROWCONTROLLER_H

#include "kdganttabstractrowcontroller.h"

class QAbstractProxyModel;
class QTreeView;

namespace KDGantt {
    class KDCHART_EXPORT TreeViewRowController :  public AbstractRowController {
        KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(TreeViewRowController)
    public:
        TreeViewRowController( QTreeView* tv, QAbstractProxyModel* proxy );
        virtual ~TreeViewRowController();

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

#endif /* KDGANTTTREEVIEWROWCONTROLLER_H */

