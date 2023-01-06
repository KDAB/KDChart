/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "timechartmodel.h"

TimeChartModel::TimeChartModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

QPair<QDateTime, QDateTime> TimeChartModel::visibleRange() const
{
    return range;
}

void TimeChartModel::setVisibleRange(const QDateTime &start, const QDateTime &end)
{
    const QPair<QDateTime, QDateTime> r = qMakePair(start, end);
    if (r == range)
        return;

    range = r;
    invalidateFilter();
}

void TimeChartModel::setVisibleStart(const QDateTime &start)
{
    setVisibleRange(start, range.second);
}

void TimeChartModel::setVisibleEnd(const QDateTime &end)
{
    setVisibleRange(range.first, end);
}

/*!
 \reimp
 */
QVariant TimeChartModel::data(const QModelIndex &index, int role) const
{
    const QVariant v = QSortFilterProxyModel::data(index, role);
    if (index.column() % 2 != 0 || role != Qt::DisplayRole)
        return v;
    else
        return QDateTime::fromSecsSinceEpoch(0).secsTo(v.toDateTime()) / 3600.0;
}

/*!
 \reimp
 */
bool TimeChartModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    const QDateTime date = index.data().toDateTime();
    return (date >= range.first || range.first.isNull()) && (date <= range.second || range.second.isNull());
}
