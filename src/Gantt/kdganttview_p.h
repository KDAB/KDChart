#ifndef KDGANTTVIEW_P_H
#define KDGANTTVIEW_P_H

#include "kdganttview.h"
#include "kdganttgraphicsscene.h"
#include "kdgantttreeviewrowcontroller.h"
#include "kdganttconstraintmodel.h"
#include "kdganttconstraintproxy.h"

#include "kdganttgraphicsview.h"
#include "kdganttdatetimegrid.h"

#include "kdganttproxymodel.h"

#include <QSplitter>
#include <QTreeView>
#include <QGraphicsView>
#include <QModelIndex>
#include <QPointer>

class QAbstractProxyModel;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsRectItem;

namespace KDGantt {
    class GraphicsItem;
    class KDGanttTreeView;

    /*! \internal */
    class KDGanttTreeView : public QTreeView {
        Q_OBJECT
    public:
        explicit KDGanttTreeView( QAbstractProxyModel* proxy, QWidget* parent=0 );
        virtual ~KDGanttTreeView();

        AbstractRowController* rowController() { return &m_controller; }

    public Q_SLOTS:
        void expandAll(QModelIndex index = QModelIndex());
        void collapseAll(QModelIndex index = QModelIndex());

    private:
        TreeViewRowController m_controller;
    };

    /*! \internal */
    class View::Private {
    public:
        explicit Private(View*);
        virtual ~Private();

        void init();

        GraphicsItem* createItem( ItemType type ) const;

        void updateScene();

        void setupGraphicsView();

        // slots
        void slotCollapsed(const QModelIndex&);
        void slotExpanded(const QModelIndex&);
        void slotVerticalScrollValueChanged( int );
        void slotLeftWidgetVerticalRangeChanged( int, int );
        void slotGfxViewVerticalRangeChanged( int, int );

        View* q;

        QSplitter splitter;

        /* TODO: Refine/subclass */
        //KDGanttTreeView treeview;
        QPointer<QAbstractItemView> leftWidget;
        AbstractRowController* rowController;
        QPointer<GraphicsView> gfxview;
        //KDGanttHeaderWidget headerwidget;

        QPointer<QAbstractItemModel> model;
        ProxyModel ganttProxyModel;
        //KDGanttTreeViewRowController rowController;
        ConstraintModel mappedConstraintModel;
        ConstraintProxy constraintProxy;
    };

}
#endif /* KDGANTTVIEW_P_H */

