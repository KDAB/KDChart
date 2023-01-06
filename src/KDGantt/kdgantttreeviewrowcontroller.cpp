/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "kdgantttreeviewrowcontroller.h"
#include "kdgantttreeviewrowcontroller_p.h"

#include <QAbstractProxyModel>
#include <QHeaderView>
#include <QScrollBar>

#include <cassert>

using namespace KDGantt;

/*!\class TreeViewRowController
 * This is an implementation of AbstractRowController that
 * aligns a gantt view with a QTreeView.
 */

TreeViewRowController::TreeViewRowController(QTreeView *tv,
                                             QAbstractProxyModel *proxy)
    : _d(new Private)
{
    _d->treeview = static_cast<Private::HackTreeView *>(tv);
    _d->proxy = proxy;
}

TreeViewRowController::~TreeViewRowController()
{
    delete _d;
    _d = nullptr;
}

#define d d_func()

int TreeViewRowController::headerHeight() const
{
    // return d->treeview->header()->sizeHint().height();
    return d->treeview->viewport()->y() - d->treeview->frameWidth();
}

int TreeViewRowController::maximumItemHeight() const
{
    return d->treeview->fontMetrics().height();
}

int TreeViewRowController::totalHeight() const
{
    return d->treeview->verticalScrollBar()->maximum() + d->treeview->viewport()->height();
}

bool TreeViewRowController::isRowVisible(const QModelIndex &_idx) const
{
    // qDebug() << _idx.model()<<d->proxy << d->treeview->model();
    const QModelIndex idx = d->proxy->mapToSource(_idx);
    assert(idx.isValid() ? (idx.model() == d->treeview->model()) : (true));
    return d->treeview->visualRect(idx).isValid();
}

bool TreeViewRowController::isRowExpanded(const QModelIndex &_idx) const
{
    const QModelIndex idx = d->proxy->mapToSource(_idx);
    assert(idx.isValid() ? (idx.model() == d->treeview->model()) : (true));
    return d->treeview->isExpanded(idx);
}

Span TreeViewRowController::rowGeometry(const QModelIndex &_idx) const
{
    const QModelIndex idx = d->proxy->mapToSource(_idx);
    assert(idx.isValid() ? (idx.model() == d->treeview->model()) : (true));
    QRect r = d->treeview->visualRect(idx).translated(QPoint(0, d->treeview->verticalOffset()));
    return Span(r.y(), r.height());
}

QModelIndex TreeViewRowController::indexAt(int height) const
{
    /* using indexAt( QPoint ) won't work here, since it does hit detection
     *   against the actual item text/icon, so we would return wrong values
     *   for items with no text etc.
     *
     *   The code below could cache for performance, but currently it doesn't
     *   seem to be the performance bottleneck at all.
     */
    if (!d->treeview->model())
        return QModelIndex();
    int y = d->treeview->verticalOffset();
    QModelIndex idx = d->treeview->model()->index(0, 0, d->treeview->rootIndex());
    do {
        if (y >= height)
            break;
        y += d->treeview->rowHeight(idx);
        idx = d->treeview->indexBelow(idx);
    } while (idx.isValid());
    return d->proxy->mapFromSource(idx);
}

QModelIndex TreeViewRowController::indexAbove(const QModelIndex &_idx) const
{
    const QModelIndex idx = d->proxy->mapToSource(_idx);
    return d->proxy->mapFromSource(d->treeview->indexAbove(idx));
}

QModelIndex TreeViewRowController::indexBelow(const QModelIndex &_idx) const
{
    const QModelIndex idx = d->proxy->mapToSource(_idx);
    return d->proxy->mapFromSource(d->treeview->indexBelow(idx));
}
