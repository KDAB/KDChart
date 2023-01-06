/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTLISTVIEWROWCONTROLLER_P_H
#define KDGANTTLISTVIEWROWCONTROLLER_P_H

#include "kdganttlistviewrowcontroller.h"

#include <QListView>

namespace KDGantt {
class ListViewRowController::Private
{
public:
    class HackListView : public QListView
    {
    public:
        using QListView::setViewportMargins;
        using QListView::verticalOffset;
    };

    Private(QListView *lv, QAbstractProxyModel *pm)
        : listview(lv)
        , proxy(pm)
    {
    }
    QListView *listview;
    QAbstractProxyModel *proxy;
};
}

#endif /* KDGANTTLISTVIEWROWCONTROLLER_P_H */
