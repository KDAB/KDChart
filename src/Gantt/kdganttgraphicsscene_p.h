#ifndef KDGANTTGRAPHICSSCENE_P_H
#define KDGANTTGRAPHICSSCENE_P_H

#include <QPersistentModelIndex>
#include <QHash>
#include <QPointer>
#include <QItemSelectionModel>
#include <QAbstractProxyModel>

#include "kdganttgraphicsscene.h"
#include "kdganttconstraintmodel.h"
#include "kdganttdatetimegrid.h"

namespace KDGantt {
    class GraphicsScene::Private {
    public:
        explicit Private(GraphicsScene*);

        void resetConstraintItems();
        void createConstraintItem( const Constraint& c );
        void deleteConstraintItem( ConstraintGraphicsItem* citem );
        void deleteConstraintItem( const Constraint& c );
        ConstraintGraphicsItem* findConstraintItem( const Constraint& c ) const;

	void recursiveUpdateMultiItem( const Span& span, const QModelIndex& idx );


        GraphicsScene* q;

        QHash<QPersistentModelIndex,GraphicsItem*> items;
        GraphicsItem* dragSource;

        QPointer<ItemDelegate> itemDelegate;
        AbstractRowController* rowController;
        DateTimeGrid           default_grid;
        QPointer<AbstractGrid> grid;
        bool readOnly;

	/* printing related members */
	bool isPrinting;
	qreal labelsWidth;

        QPointer<QAbstractProxyModel> summaryHandlingModel;

        QPointer<ConstraintModel> constraintModel;

        QPointer<QItemSelectionModel> selectionModel;
    };

    GraphicsScene::GraphicsScene( GraphicsScene::Private* d ) : _d( d )
    {
        init();
    }
}

#endif /* KDGANTTGRAPHICSSCENE_P_H */

