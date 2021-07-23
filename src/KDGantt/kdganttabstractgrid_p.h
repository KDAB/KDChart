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

#ifndef KDGANTTABSTRACTGRID_P_H
#define KDGANTTABSTRACTGRID_P_H

#include <QPointer>
#include <QAbstractItemModel>
#include <QPersistentModelIndex>

#include "kdganttabstractgrid.h"

namespace KDGantt {
class AbstractGrid::Private
{
public:
    QPointer<QAbstractItemModel> model;
    QPersistentModelIndex root;
};

inline AbstractGrid::AbstractGrid(Private *d)
    : _d(d)
{
}
}

#endif /* KDGANTTABSTRACTGRID_P_H */
