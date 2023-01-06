/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTFORWARDINGPROXYMODEL_H
#define KDGANTTFORWARDINGPROXYMODEL_H

#include <QAbstractProxyModel>

#include "kdganttglobal.h"

namespace KDGantt {
class KDGANTT_EXPORT ForwardingProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ForwardingProxyModel)
public:
    explicit ForwardingProxyModel(QObject *parent = nullptr);
    ~ForwardingProxyModel() override;

    /*reimp*/ QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    /*reimp*/ QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    /*reimp*/ void setSourceModel(QAbstractItemModel *model) override;

    /*reimp*/ QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    /*reimp*/ QModelIndex parent(const QModelIndex &idx) const override;

    /*reimp*/ int rowCount(const QModelIndex &idx = QModelIndex()) const override;
    /*reimp*/ int columnCount(const QModelIndex &idx = QModelIndex()) const override;

    /*reimp*/ bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /*reimp*/ QMimeData *mimeData(const QModelIndexList &indexes) const override;
    /*reimp*/ bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    /*reimp*/ QStringList mimeTypes() const override;
    /*reimp*/ Qt::DropActions supportedDropActions() const override;

protected Q_SLOTS:
    virtual void sourceModelAboutToBeReset();
    virtual void sourceModelReset();
    virtual void sourceLayoutAboutToBeChanged();
    virtual void sourceLayoutChanged();
    virtual void sourceDataChanged(const QModelIndex &from, const QModelIndex &to);
    virtual void sourceColumnsAboutToBeInserted(const QModelIndex &idx, int start, int end);
    virtual void sourceColumnsInserted(const QModelIndex &idx, int start, int end);
    virtual void sourceColumnsAboutToBeRemoved(const QModelIndex &idx, int start, int end);
    virtual void sourceColumnsRemoved(const QModelIndex &idx, int start, int end);
    virtual void sourceRowsAboutToBeInserted(const QModelIndex &idx, int start, int end);
    virtual void sourceRowsInserted(const QModelIndex &idx, int start, int end);
    virtual void sourceRowsAboutToBeRemoved(const QModelIndex &, int start, int end);
    virtual void sourceRowsRemoved(const QModelIndex &, int start, int end);
};
}

#endif /* KDGANTTFORWARDINGPROXYMODEL_H */
