/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef TIMECHARTMODEL_H
#define TIMECHARTMODEL_H

#include <QDateTime>
#include <QPair>
#include <QSortFilterProxyModel>

class TimeChartModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TimeChartModel(QObject *parent = nullptr);

    QPair<QDateTime, QDateTime> visibleRange() const;

public Q_SLOTS:
    void setVisibleRange(const QDateTime &start, const QDateTime &end);
    void setVisibleStart(const QDateTime &start);
    void setVisibleEnd(const QDateTime &end);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QPair<QDateTime, QDateTime> range;
};

#endif
