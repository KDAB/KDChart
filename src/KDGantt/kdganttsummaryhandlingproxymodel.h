/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTSUMMARYHANDLINGPROXYMODEL_H
#define KDGANTTSUMMARYHANDLINGPROXYMODEL_H

#include "kdganttforwardingproxymodel.h"

namespace KDGantt {
class KDGANTT_EXPORT SummaryHandlingProxyModel : public ForwardingProxyModel
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(SummaryHandlingProxyModel)
public:
    explicit SummaryHandlingProxyModel(QObject *parent = nullptr);
    ~SummaryHandlingProxyModel() override;

    /*reimp*/ void setSourceModel(QAbstractItemModel *model) override;

    /*reimp*/ QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const override;
    /*reimp*/ bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /*reimp*/ Qt::ItemFlags flags(const QModelIndex &idx) const override;

protected:
    /*reimp*/ void sourceModelReset() override;
    /*reimp*/ void sourceLayoutChanged() override;
    /*reimp*/ void sourceDataChanged(const QModelIndex &from, const QModelIndex &to) override;
    /*reimp*/ void sourceColumnsAboutToBeInserted(const QModelIndex &idx, int start, int end) override;
    /*reimp*/ void sourceColumnsAboutToBeRemoved(const QModelIndex &idx, int start, int end) override;
    /*reimp*/ void sourceRowsAboutToBeInserted(const QModelIndex &idx, int start, int end) override;
    /*reimp*/ void sourceRowsAboutToBeRemoved(const QModelIndex &, int start, int end) override;
};
}

#endif /* KDGANTTSUMMARYHANDLINGPROXYMODEL_H */
