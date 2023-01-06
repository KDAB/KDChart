/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTGRAPHICSSCENE_P_H
#define KDGANTTGRAPHICSSCENE_P_H

#include <QAbstractProxyModel>
#include <QHash>
#include <QItemSelectionModel>
#include <QPersistentModelIndex>
#include <QPointer>

#include "kdganttconstraintmodel.h"
#include "kdganttdatetimegrid.h"
#include "kdganttgraphicsscene.h"

namespace KDGantt {
class GraphicsScene::Private
{
public:
    explicit Private(GraphicsScene *);

    void resetConstraintItems();
    void createConstraintItem(const Constraint &c);
    void deleteConstraintItem(ConstraintGraphicsItem *citem);
    void deleteConstraintItem(const Constraint &c);
    ConstraintGraphicsItem *findConstraintItem(const Constraint &c) const;

    void recursiveUpdateMultiItem(const Span &span, const QModelIndex &idx);

    GraphicsScene *q;

    QHash<QPersistentModelIndex, GraphicsItem *> items;
    GraphicsItem *dragSource;

    QPointer<ItemDelegate> itemDelegate;
    AbstractRowController *rowController;
    DateTimeGrid default_grid;
    QPointer<AbstractGrid> grid;
    bool readOnly;

    /* printing related members */
    bool isPrinting;
    bool drawColumnLabels;
    qreal labelsWidth;

    QPointer<QAbstractProxyModel> summaryHandlingModel;

    QPointer<ConstraintModel> constraintModel;

    QPointer<QItemSelectionModel> selectionModel;
};

GraphicsScene::GraphicsScene(GraphicsScene::Private *d)
    : _d(d)
{
    init();
}
}

#endif /* KDGANTTGRAPHICSSCENE_P_H */
