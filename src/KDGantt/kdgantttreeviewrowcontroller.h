/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef KDGANTTTREEVIEWROWCONTROLLER_H
#define KDGANTTTREEVIEWROWCONTROLLER_H

#include "kdganttabstractrowcontroller.h"

QT_BEGIN_NAMESPACE
class QAbstractProxyModel;
class QTreeView;
QT_END_NAMESPACE

namespace KDGantt {
class KDGANTT_EXPORT TreeViewRowController : public AbstractRowController
{
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(TreeViewRowController)
public:
    TreeViewRowController(QTreeView *tv, QAbstractProxyModel *proxy);
    ~TreeViewRowController() override;

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

#endif /* KDGANTTTREEVIEWROWCONTROLLER_H */
