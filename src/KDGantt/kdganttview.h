/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTVIEW_H
#define KDGANTTVIEW_H

#include "kdganttglobal.h"
#include <QModelIndex>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QAbstractProxyModel;
class QAbstractItemView;
class QItemSelectionModel;
class QPrinter;
class QSplitter;
QT_END_NAMESPACE

namespace KDGantt {
class ItemDelegate;
class Constraint;
class ConstraintModel;
class AbstractGrid;
class GraphicsView;
class AbstractRowController;

class KDGANTT_EXPORT View : public QWidget
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET(View)
    Q_PRIVATE_SLOT(d, void slotCollapsed(const QModelIndex &))
    Q_PRIVATE_SLOT(d, void slotExpanded(const QModelIndex &))
    Q_PRIVATE_SLOT(d, void slotVerticalScrollValueChanged(int))
    Q_PRIVATE_SLOT(d, void slotLeftWidgetVerticalRangeChanged(int, int))
    Q_PRIVATE_SLOT(d, void slotGfxViewVerticalRangeChanged(int, int))

public:
    explicit View(QWidget *parent = nullptr);
    ~View() override;

    QAbstractItemModel *model() const;
    QItemSelectionModel *selectionModel() const;
    ItemDelegate *itemDelegate() const;
    ConstraintModel *constraintModel() const;
    AbstractGrid *grid() const;
    QModelIndex rootIndex() const;

    QModelIndex indexAt(const QPoint &pos) const;

    void setLeftView(QAbstractItemView *);
    const QAbstractItemView *leftView() const;
    QAbstractItemView *leftView();

    const QSplitter *splitter() const;
    QSplitter *splitter();

    void setRowController(AbstractRowController *);
    AbstractRowController *rowController();
    const AbstractRowController *rowController() const;

    void setGraphicsView(GraphicsView *);
    const GraphicsView *graphicsView() const;
    GraphicsView *graphicsView();
    const QAbstractProxyModel *ganttProxyModel() const;
    QAbstractProxyModel *ganttProxyModel();

    void ensureVisible(const QModelIndex &index);

    void print(QPrinter *printer, bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPrinter *printer, qreal start, qreal end, bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPainter *painter, const QRectF &target = QRectF(), bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPainter *painter, qreal start, qreal end,
               const QRectF &target = QRectF(), bool drawRowLabels = true, bool drawColumnLabels = true);

public Q_SLOTS:
    void setModel(QAbstractItemModel *model);
    void setRootIndex(const QModelIndex &idx);
    void setSelectionModel(QItemSelectionModel *smodel);
    void setItemDelegate(ItemDelegate *);
    void setConstraintModel(ConstraintModel *);
    void setGrid(AbstractGrid *);
    void expandAll(QModelIndex index = QModelIndex());
    void collapseAll(QModelIndex index = QModelIndex());

protected:
    /*reimp*/ void resizeEvent(QResizeEvent *) override;
};
}

#endif /* KDGANTTVIEW_H */
