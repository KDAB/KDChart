/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Gantt library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Gantt licenses may use this file in
 ** accordance with the KD Gantt Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdgantt for
 **   information about KD Gantt Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
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

