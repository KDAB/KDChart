/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTCONSTRAINTMODEL_H
#define KDGANTTCONSTRAINTMODEL_H

#include <QDebug>
#include <QModelIndex>

#include "kdganttconstraint.h"
#include "kdganttglobal.h"

namespace KDGantt {
class KDGANTT_EXPORT ConstraintModel : public QObject
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_DERIVED_PARENT(ConstraintModel, QObject *)
public:
    explicit ConstraintModel(QObject *parent = nullptr);
    ~ConstraintModel() override;

    /**
     * Subclassing ConstraintModel and overriding addConstraint() and removeConstraint() can provide
     * re-entrancy issues in the ConstraintModel<->ConstraintProxy interaction. Therefore it is recommended
     * to better subclass GraphicsView and override addConstraint() there.
     */
    virtual void addConstraint(const Constraint &c);
    virtual bool removeConstraint(const Constraint &c);

    void clear();
    void cleanup();

    QList<Constraint> constraints() const;

    bool hasConstraint(const Constraint &c) const;
    inline bool hasConstraint(const QModelIndex &s,
                              const QModelIndex &e) const;

    QList<Constraint> constraintsForIndex(const QModelIndex &) const;

Q_SIGNALS:
    void constraintAdded(const KDGantt::Constraint &);
    void constraintRemoved(const KDGantt::Constraint &);

private:
    Private *_d;
};

inline const ConstraintModel::Private *ConstraintModel::d_func() const
{
    return _d;
}
inline ConstraintModel::Private *ConstraintModel::d_func()
{
    return _d;
}
inline bool ConstraintModel::hasConstraint(const QModelIndex &s, const QModelIndex &e) const
{
    return hasConstraint(Constraint(s, e));
}
}

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>

QDebug KDGANTT_EXPORT operator<<(QDebug dbg, const KDGantt::ConstraintModel &model);
inline QDebug operator<<(QDebug dbg, KDGantt::ConstraintModel *model)
{
    return operator<<(dbg, *model);
}

#endif /* QT_NO_DEBUG_STREAM */

#endif /* KDGANTTCONSTRAINTMODEL_H */
