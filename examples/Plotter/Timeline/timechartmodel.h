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

#ifndef TIMECHARTMODEL_H
#define TIMECHARTMODEL_H

#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QPair>

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
