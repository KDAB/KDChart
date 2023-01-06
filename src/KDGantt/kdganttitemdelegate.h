/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTITEMDELEGATE_H
#define KDGANTTITEMDELEGATE_H

#include <QBrush>
#include <QDebug>
#include <QItemDelegate>
#include <QPen>

#include "kdganttglobal.h"

namespace KDGantt {
class StyleOptionGanttItem;
class Constraint;

class KDGANTT_EXPORT ItemDelegate : public QItemDelegate
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(ItemDelegate)
public:
    enum InteractionState
    {
        State_None = 0,
        State_Move,
        State_ExtendLeft,
        State_ExtendRight,
        State_DragConstraint
    };

    explicit ItemDelegate(QObject *parent = nullptr);
    ~ItemDelegate() override;

    void setDefaultBrush(ItemType type, const QBrush &brush);
    QBrush defaultBrush(ItemType type) const;

    void setDefaultPen(ItemType type, const QPen &pen);
    QPen defaultPen(ItemType type) const;

    virtual Span itemBoundingSpan(const StyleOptionGanttItem &opt, const QModelIndex &idx) const;
    virtual QRectF constraintBoundingRect(const QPointF &start, const QPointF &end, const Constraint &constraint) const;
    virtual InteractionState interactionStateFor(const QPointF &pos,
                                                 const StyleOptionGanttItem &opt,
                                                 const QModelIndex &idx) const;

    virtual void paintGanttItem(QPainter *p, const StyleOptionGanttItem &opt, const QModelIndex &idx);
    virtual void paintConstraintItem(QPainter *p, const QStyleOptionGraphicsItem &opt,
                                     const QPointF &start, const QPointF &end, const Constraint &constraint);

    virtual QString toolTip(const QModelIndex &idx) const;

protected:
    void paintFinishStartConstraint(QPainter *p, const QStyleOptionGraphicsItem &opt,
                                    const QPointF &start, const QPointF &end, const Constraint &constraint);
    QPolygonF finishStartLine(const QPointF &start, const QPointF &end) const;
    QPolygonF finishStartArrow(const QPointF &start, const QPointF &end) const;

    void paintFinishFinishConstraint(QPainter *p, const QStyleOptionGraphicsItem &opt,
                                     const QPointF &start, const QPointF &end, const Constraint &constraint);
    QPolygonF finishFinishLine(const QPointF &start, const QPointF &end) const;
    QPolygonF finishFinishArrow(const QPointF &start, const QPointF &end) const;

    void paintStartStartConstraint(QPainter *p, const QStyleOptionGraphicsItem &opt,
                                   const QPointF &start, const QPointF &end, const Constraint &constraint);
    QPolygonF startStartLine(const QPointF &start, const QPointF &end) const;
    QPolygonF startStartArrow(const QPointF &start, const QPointF &end) const;

    void paintStartFinishConstraint(QPainter *p, const QStyleOptionGraphicsItem &opt,
                                    const QPointF &start, const QPointF &end, const Constraint &constraint);
    QPolygonF startFinishLine(const QPointF &start, const QPointF &end) const;
    QPolygonF startFinishArrow(const QPointF &start, const QPointF &end) const;
};
}

#ifndef QT_NO_DEBUG_STREAM
KDGANTT_EXPORT QDebug operator<<(QDebug dbg, KDGantt::ItemDelegate::InteractionState);
#endif

#endif /* KDGANTTITEMDELEGATE_H */
