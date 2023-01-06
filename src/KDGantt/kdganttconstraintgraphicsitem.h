/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTCONSTRAINTGRAPHICSITEM_H
#define KDGANTTCONSTRAINTGRAPHICSITEM_H

#include <QGraphicsItem>

#include "kdganttconstraint.h"

namespace KDGantt {
class GraphicsScene;

class KDGANTT_EXPORT ConstraintGraphicsItem : public QGraphicsItem
{
public:
    enum
    {
        Type = UserType + 43
    };

    explicit ConstraintGraphicsItem(const Constraint &c,
                                    QGraphicsItem *parent = nullptr, GraphicsScene *scene = nullptr);
    ~ConstraintGraphicsItem() override;

    /*reimp*/ int type() const override;
    /*reimp (non virtual)*/ GraphicsScene *scene() const;

    /*reimp*/ QString ganttToolTip() const;

    /*reimp*/ QRectF boundingRect() const override;
    /*reimp*/ void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget = nullptr) override;

    inline const Constraint &constraint() const
    {
        return m_constraint;
    }
    Constraint proxyConstraint() const;

    void setStart(const QPointF &start);
    inline QPointF start() const
    {
        return m_start;
    }
    void setEnd(const QPointF &end);
    inline QPointF end() const
    {
        return m_end;
    }

    void updateItem(const QPointF &start, const QPointF &end);

private:
    Constraint m_constraint;
    QPointF m_start;
    QPointF m_end;
};
}

#endif /* KDGANTTCONSTRAINTGRAPHICSITEM_H */
