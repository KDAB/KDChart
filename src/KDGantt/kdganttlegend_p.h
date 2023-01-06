/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTLEGEND_P_H
#define KDGANTTLEGEND_P_H

#include "kdganttlegend.h"

#include "kdganttproxymodel.h"

namespace KDGantt {
class Legend::Private
{
public:
    ProxyModel proxyModel;
};
}

#endif /* KDGANTTLEGEND_P_H */
