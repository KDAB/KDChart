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
