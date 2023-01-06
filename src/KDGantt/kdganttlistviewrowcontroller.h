/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTLISTVIEWROWCONTROLLER_H
#define KDGANTTLISTVIEWROWCONTROLLER_H

#include "kdganttabstractrowcontroller.h"

QT_BEGIN_NAMESPACE
class QAbstractProxyModel;
class QListView;
QT_END_NAMESPACE

namespace KDGantt {
class KDGANTT_EXPORT ListViewRowController : public AbstractRowController
{
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(ListViewRowController)
public:
    ListViewRowController(QListView *lv, QAbstractProxyModel *proxy);
    ~ListViewRowController() override;

    /*reimp*/ int headerHeight() const override;
    /*reimp*/ int maximumItemHeight() const override;
    /*reimp*/ int totalHeight() const override;
    /*reimp*/ bool isRowVisible(const QModelIndex &idx) const override;
    /*reimp*/ bool isRowExpanded(const QModelIndex &idx) const override;
    /*reimp*/ Span rowGeometry(const QModelIndex &idx) const override;
    /*reimp*/ QModelIndex indexAt(int height) const override;
    /*reimp*/ QModelIndex indexAbove(const QModelIndex &idx) const override;
    /*reimp*/ QModelIndex indexBelow(const QModelIndex &idx) const override;
};
}

#endif /* KDGANTTLISTVIEWROWCONTROLLER_H */
