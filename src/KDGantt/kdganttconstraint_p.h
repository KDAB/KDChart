/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTCONSTRAINT_P_H
#define KDGANTTCONSTRAINT_P_H

#include <QMap>
#include <QPersistentModelIndex>
#include <QSharedData>

#include "kdganttconstraint.h"

namespace KDGantt {
class Constraint::Private : public QSharedData
{
public:
    Private();
    Private(const Private &other);

    inline bool equals(const Private &other) const
    {
        /* Due to a Qt bug we have to check separately for invalid indexes */
        return (start == other.start || (!start.isValid() && !other.start.isValid()))
            && (end == other.end || (!end.isValid() && !other.end.isValid()))
            && type == other.type
            && relationType == other.relationType
            && data == other.data;
    }

    QPersistentModelIndex start;
    QPersistentModelIndex end;
    Type type = TypeSoft;
    RelationType relationType = FinishStart;
    QMap<int, QVariant> data;
};
}

#endif /* KDGANTTCONSTRAINT_P_H */
