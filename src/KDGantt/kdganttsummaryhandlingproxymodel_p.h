/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTSUMMARYHANDLINGPROXYMODEL_P_H
#define KDGANTTSUMMARYHANDLINGPROXYMODEL_P_H

#include "kdganttsummaryhandlingproxymodel.h"

#include <QDateTime>
#include <QHash>
#include <QPair>
#include <QPersistentModelIndex>

namespace KDGantt {
class SummaryHandlingProxyModel::Private
{
public:
    bool cacheLookup(const QModelIndex &idx,
                     QPair<QDateTime, QDateTime> *result) const;
    void insertInCache(const SummaryHandlingProxyModel *model, const QModelIndex &idx) const;
    void removeFromCache(const QModelIndex &idx) const;
    void clearCache() const;

    inline bool isSummary(const QModelIndex &idx) const
    {
        int typ = idx.data(ItemTypeRole).toInt();
        return (typ == TypeSummary) || (typ == TypeMulti);
    }

    mutable QHash<QModelIndex, QPair<QDateTime, QDateTime>> cached_summary_items;
};
}

#endif /* KDGANTTSUMMARYHANDLINGPROXYMODEL_P_H */
