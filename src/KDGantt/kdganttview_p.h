/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

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

QT_BEGIN_NAMESPACE
class QAbstractProxyModel;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsRectItem;
QT_END_NAMESPACE

namespace KDGantt {
class GraphicsItem;
class KDGanttTreeView;

/*! \internal */
class KDGanttTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit KDGanttTreeView(QAbstractProxyModel *proxy, QWidget *parent = nullptr);
    ~KDGanttTreeView() override;

    AbstractRowController *rowController()
    {
        return &m_controller;
    }

public Q_SLOTS:
    void expandAll(QModelIndex index = QModelIndex());
    void collapseAll(QModelIndex index = QModelIndex());

private:
    TreeViewRowController m_controller;
};

/*! \internal */
class View::Private
{
public:
    explicit Private(View *);
    virtual ~Private();

    void init();

    GraphicsItem *createItem(ItemType type) const;

    void updateScene();

    void setupGraphicsView();

    // slots
    void slotCollapsed(const QModelIndex &);
    void slotExpanded(const QModelIndex &);
    void slotVerticalScrollValueChanged(int);
    void slotLeftWidgetVerticalRangeChanged(int, int);
    void slotGfxViewVerticalRangeChanged(int, int);

    View *q;

    QSplitter splitter;

    /* TODO: Refine/subclass */
    //KDGanttTreeView treeview;
    QPointer<QAbstractItemView> leftWidget;
    AbstractRowController *rowController;
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
