/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>

class ProjectModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ProjectModel(QObject *parent = nullptr);
    ~ProjectModel() override;

    /*reimp*/ int rowCount(const QModelIndex &idx) const override;
    /*reimp*/ int columnCount(const QModelIndex &idx) const override;

    /*reimp*/ QModelIndex index(int row, int col, const QModelIndex &parent = QModelIndex()) const override;
    /*reimp*/ QModelIndex parent(const QModelIndex &idx) const override;

    /*reimp*/ QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    /*reimp*/ QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
    /*reimp*/ bool setData(const QModelIndex &idx, const QVariant &value,
                           int role = Qt::DisplayRole) override;

    /*reimp*/ bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    /*reimp*/ Qt::ItemFlags flags(const QModelIndex &) const override;

    bool load(const QString &filename);
    bool save(const QString &filename);

private:
    class Node;

    Node *m_root;
};

#endif /* PROJECTMODEL_H */
