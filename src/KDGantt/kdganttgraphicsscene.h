/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTGRAPHICSSCENE_H
#define KDGANTTGRAPHICSSCENE_H

#include <QDateTime>
#include <QGraphicsScene>
#include <QList>
#include <QModelIndex>

#include "kdganttglobal.h"

QT_BEGIN_NAMESPACE
class QAbstractProxyModel;
class QItemSelectionModel;
class QPrinter;
QT_END_NAMESPACE

namespace KDGantt {
class AbstractGrid;
class AbstractRowController;
class GraphicsItem;
class Constraint;
class ConstraintModel;
class ConstraintGraphicsItem;
class ItemDelegate;

class KDGANTT_EXPORT GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(GraphicsScene)
public:
    explicit GraphicsScene(QObject *parent = nullptr);
    ~GraphicsScene() override;

    // qreal dateTimeToSceneX( const QDateTime& dt ) const;
    // QDateTime sceneXtoDateTime( qreal x ) const;

    static QModelIndex mainIndex(const QModelIndex &idx);
    static QModelIndex dataIndex(const QModelIndex &idx);

    QAbstractItemModel *model() const;
    QAbstractProxyModel *summaryHandlingModel() const;
    QModelIndex rootIndex() const;
    ConstraintModel *constraintModel() const;
    QItemSelectionModel *selectionModel() const;

    void insertItem(const QPersistentModelIndex &, GraphicsItem *);
    void removeItem(const QModelIndex &);
    using QGraphicsScene::removeItem;
    GraphicsItem *findItem(const QModelIndex &) const;
    GraphicsItem *findItem(const QPersistentModelIndex &) const;

    void updateItems();
    void clearItems();
    void deleteSubtree(const QModelIndex &);

    ConstraintGraphicsItem *findConstraintItem(const Constraint &) const;
    void clearConstraintItems();

    void setItemDelegate(ItemDelegate *);
    ItemDelegate *itemDelegate() const;

    void setRowController(AbstractRowController *rc);
    AbstractRowController *rowController() const;

    void setGrid(AbstractGrid *grid);
    AbstractGrid *grid() const;

    bool isReadOnly() const;

    void updateRow(const QModelIndex &idx);
    GraphicsItem *createItem(ItemType type) const;

    /* used by GraphicsItem */
    void itemEntered(const QModelIndex &);
    void itemPressed(const QModelIndex &);
    void itemClicked(const QModelIndex &);
    void itemDoubleClicked(const QModelIndex &);
    void setDragSource(GraphicsItem *item);
    GraphicsItem *dragSource() const;

    /* Printing */
    void print(QPrinter *printer, bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPrinter *printer, qreal start, qreal end, bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPainter *painter, const QRectF &target = QRectF(), bool drawRowLabels = true, bool drawColumnLabels = true);
    void print(QPainter *painter, qreal start, qreal end, const QRectF &target = QRectF(), bool drawRowLabels = true, bool drawColumnLabels = true);

Q_SIGNALS:
    void gridChanged();

    void clicked(const QModelIndex &index);
    void qrealClicked(const QModelIndex &index);
    void entered(const QModelIndex &index);
    void pressed(const QModelIndex &index);

protected:
    /*reimp*/ void helpEvent(QGraphicsSceneHelpEvent *helpEvent) override;
    /*reimp*/ void drawBackground(QPainter *painter, const QRectF &rect) override;
    /*reimp*/ void drawForeground(QPainter *painter, const QRectF &rect) override;

public Q_SLOTS:
    void setModel(QAbstractItemModel *);
    void setSummaryHandlingModel(QAbstractProxyModel *);
    void setConstraintModel(ConstraintModel *);
    void setRootIndex(const QModelIndex &idx);
    void setSelectionModel(QItemSelectionModel *selectionmodel);
    void setReadOnly(bool);

private Q_SLOTS:
    /* slots for ConstraintModel */
    void slotConstraintAdded(const KDGantt::Constraint &);
    void slotConstraintRemoved(const KDGantt::Constraint &);
    void slotGridChanged();

private:
    void doPrint(QPainter *painter, const QRectF &targetRect,
                 qreal start, qreal end,
                 QPrinter *printer, bool drawRowLabels, bool drawColumnLabels);
};
}

#endif /* KDGANTTGRAPHICSSCENE_H */
