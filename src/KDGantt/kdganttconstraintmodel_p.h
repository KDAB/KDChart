/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef KDGANTTCONSTRAINTMODEL_P_H
#define KDGANTTCONSTRAINTMODEL_P_H

#include "kdganttconstraintmodel.h"

#include <QList>
#include <QMultiHash>
#include <QPersistentModelIndex>

namespace KDGantt {
class ConstraintModel::Private
{
public:
    Private();

    void addConstraintToIndex(const QModelIndex &idx, const Constraint &c);
    void removeConstraintFromIndex(const QModelIndex &idx, const Constraint &c);

    typedef QMultiHash<QPersistentModelIndex, Constraint> IndexType;

    QList<Constraint> constraints;
    IndexType indexMap;
};
}

#endif /* KDGANTTCONSTRAINTMODEL_P_H */
