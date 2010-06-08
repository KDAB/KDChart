#ifndef KDGANTTABSTRACTROWCONTROLLER_H
#define KDGANTTABSTRACTROWCONTROLLER_H

#include <QPair>
#include "kdganttglobal.h"

class QModelIndex;

namespace KDGantt {
    class KDCHART_EXPORT AbstractRowController {
    public:
        AbstractRowController();
        virtual ~AbstractRowController();

        virtual int headerHeight() const = 0;
        virtual int maximumItemHeight() const = 0;
	virtual int totalHeight() const = 0;

        virtual bool isRowVisible( const QModelIndex& idx ) const = 0;
	virtual bool isRowExpanded( const QModelIndex& idx ) const = 0;
        virtual Span rowGeometry( const QModelIndex& idx ) const = 0;


        virtual QModelIndex indexAt( int height ) const = 0;
        virtual QModelIndex indexAbove( const QModelIndex& idx ) const = 0;
        virtual QModelIndex indexBelow( const QModelIndex& idx ) const = 0;
    };
}

#endif /* KDGANTTABSTRACTROWCONTROLLER_H */

