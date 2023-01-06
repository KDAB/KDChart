/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTTREEVIEWROWCONTROLLER_P_H
#define KDGANTTTREEVIEWROWCONTROLLER_P_H

#include "kdganttabstractrowcontroller.h"

#include <QTreeView>

QT_BEGIN_NAMESPACE
class QAbstractProxyModel;
QT_END_NAMESPACE

namespace KDGantt {
class TreeViewRowController::Private
{
public:
    class HackTreeView : public QTreeView
    {
    public:
        using QTreeView::rowHeight;
        using QTreeView::verticalOffset;
    };
    HackTreeView *treeview;
    QAbstractProxyModel *proxy;
};
}

#endif /* KDGANTTTREEVIEWROWCONTROLLER_P_H */
