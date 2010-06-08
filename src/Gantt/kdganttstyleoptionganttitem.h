#ifndef KDGANTTSTYLEOPTIONGANTTITEM_H
#define KDGANTTSTYLEOPTIONGANTTITEM_H

#include "../kdchart_export.h"

#include <QStyleOptionViewItem>
#include <QRectF>
#include <QDebug>

namespace KDGantt {
    class AbstractGrid;
    class KDCHART_EXPORT StyleOptionGanttItem : public QStyleOptionViewItem {
    public:
        enum Position { Left, Right, Center, Hidden };

        StyleOptionGanttItem();
        StyleOptionGanttItem( const StyleOptionGanttItem& other );
        StyleOptionGanttItem& operator=( const StyleOptionGanttItem& other );

        QRectF boundingRect;
        QRectF itemRect;
        Position displayPosition;
        AbstractGrid* grid;
        QString text;
    };
}

#ifndef QT_NO_DEBUG_STREAM

QDebug operator<<( QDebug dbg, KDGantt::StyleOptionGanttItem::Position p);
QDebug operator<<( QDebug dbg, const KDGantt::StyleOptionGanttItem& s );

#endif /* QT_NO_DEBUG_STREAM */


#endif /* KDGANTTSTYLEOPTIONGANTTITEM_H */

