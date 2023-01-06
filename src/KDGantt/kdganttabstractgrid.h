/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTABSTRACTGRID_H
#define KDGANTTABSTRACTGRID_H

#include "kdganttconstraint.h"
#include "kdganttglobal.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QRectF;
class QAbstractItemModel;
class QModelIndex;
QT_END_NAMESPACE

namespace KDGantt {
class AbstractRowController;
class Span;

class KDGANTT_EXPORT AbstractGrid : public QObject
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(AbstractGrid)
    friend class GraphicsScene;

public:
    AbstractGrid(QObject *parent = nullptr);
    ~AbstractGrid() override;

    QAbstractItemModel *model() const;
    QModelIndex rootIndex() const;

    virtual Span mapToChart(const QModelIndex &idx) const = 0;
    virtual bool mapFromChart(const Span &span, const QModelIndex &idx,
                              const QList<Constraint> &constraints = QList<Constraint>()) const = 0;
    virtual qreal mapToChart(const QVariant &value) const;
    virtual QVariant mapFromChart(qreal x) const;
    bool isSatisfiedConstraint(const Constraint &c) const;

    virtual void paintGrid(QPainter *painter, const QRectF &sceneRect, const QRectF &exposedRect,
                           AbstractRowController *rowController = nullptr, QWidget *widget = nullptr) = 0;
    virtual void paintHeader(QPainter *painter, const QRectF &headerRect, const QRectF &exposedRect,
                             qreal offset, QWidget *widget = nullptr) = 0;

public Q_SLOTS:
    /*internal*/ virtual void setModel(QAbstractItemModel *model);
    /*internal*/ virtual void setRootIndex(const QModelIndex &idx);
Q_SIGNALS:
    void gridChanged();

protected:
    virtual void drawBackground(QPainter *paint, const QRectF &rect);
    virtual void drawForeground(QPainter *paint, const QRectF &rect);
};
}

#endif /* KDGANTTABSTRACTGRID_H */
