/****************************************************************************
** Copyright (C) 2001-2021 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

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

namespace KDGantt
{
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

    [[nodiscard]] QAbstractItemModel *model() const;
    [[nodiscard]] QItemSelectionModel *selectionModel() const;
    [[nodiscard]] ItemDelegate *itemDelegate() const;
    [[nodiscard]] ConstraintModel *constraintModel() const;
    [[nodiscard]] AbstractGrid *grid() const;
    [[nodiscard]] QModelIndex rootIndex() const;

    [[nodiscard]] QModelIndex indexAt(const QPoint &pos) const;

    void setLeftView(QAbstractItemView *);
    [[nodiscard]] const QAbstractItemView *leftView() const;
    QAbstractItemView *leftView();

    [[nodiscard]] const QSplitter *splitter() const;
    QSplitter *splitter();

    void setRowController(AbstractRowController *);
    AbstractRowController *rowController();
    [[nodiscard]] const AbstractRowController *rowController() const;

    void setGraphicsView(GraphicsView *);
    [[nodiscard]] const GraphicsView *graphicsView() const;
    GraphicsView *graphicsView();
    [[nodiscard]] const QAbstractProxyModel *ganttProxyModel() const;
    QAbstractProxyModel *ganttProxyModel();

    void ensureVisible(const QModelIndex &index);

    void print(QPrinter *printer, bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPrinter *printer, qreal start, qreal end, bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPainter *painter, const QRectF &target = QRectF(), bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPainter *painter, qreal start, qreal end, const QRectF &target = QRectF(), bool drawRowLabels = true, bool drawColumnLabels = true);

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
