/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTABSTRACTGRID_P_H
#define KDGANTTABSTRACTGRID_P_H

#include <QAbstractItemModel>
#include <QPersistentModelIndex>
#include <QPointer>

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
