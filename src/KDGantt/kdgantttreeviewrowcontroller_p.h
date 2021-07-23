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
