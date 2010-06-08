#ifndef KDGANTTGRAPHICSVIEW_P_H
#define KDGANTTGRAPHICSVIEW_P_H

#include "kdganttgraphicsview.h"
#include "kdganttgraphicsscene.h"
#include "kdganttdatetimegrid.h"

#include <QPointer>

namespace KDGantt {
    class HeaderWidget : public QWidget {
        Q_OBJECT
    public:
        explicit HeaderWidget( GraphicsView* parent );
        virtual ~HeaderWidget();

        GraphicsView* view() const { return qobject_cast<GraphicsView*>( parent() );}

    public Q_SLOTS:
        void scrollTo( int );
    protected:
        /*reimp*/ bool event( QEvent* ev );
        /*reimp*/ void paintEvent( QPaintEvent* ev );
        /*reimp*/ void contextMenuEvent( QContextMenuEvent* ev );
    private:
        qreal m_offset;
    };

    class GraphicsView::Private {
        Q_DISABLE_COPY( Private )
    public:
        explicit Private(GraphicsView* _q);

        void updateHeaderGeometry();

        void slotGridChanged();
        void slotHorizontalScrollValueChanged( int val );

        /* slots for QAbstractItemModel signals */
        void slotColumnsInserted( const QModelIndex& parent,  int start, int end );
        void slotColumnsRemoved( const QModelIndex& parent,  int start, int end );
        void slotDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
        void slotLayoutChanged();
        void slotModelReset();
        void slotRowsInserted( const QModelIndex& parent,  int start, int end );
        void slotRowsAboutToBeRemoved( const QModelIndex& parent,  int start, int end );
        void slotRowsRemoved( const QModelIndex& parent,  int start, int end );

        void slotItemClicked( const QModelIndex& idx );
        void slotItemDoubleClicked( const QModelIndex& idx );

        void slotHeaderContextMenuRequested( const QPoint& pt );

        GraphicsView* q;
        AbstractRowController* rowcontroller;
        HeaderWidget headerwidget;
        GraphicsScene scene;
    };
}

#endif /* KDGANTTGRAPHICSVIEW_P_H */

