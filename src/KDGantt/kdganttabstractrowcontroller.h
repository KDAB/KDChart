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

#ifndef KDGANTTABSTRACTROWCONTROLLER_H
#define KDGANTTABSTRACTROWCONTROLLER_H

#include <QPair>
#include "kdganttglobal.h"

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

namespace KDGantt {
class KDGANTT_EXPORT AbstractRowController
{
public:
    AbstractRowController();
    virtual ~AbstractRowController();

    virtual int headerHeight() const = 0;
    virtual int maximumItemHeight() const = 0;
    virtual int totalHeight() const = 0;

    virtual bool isRowVisible(const QModelIndex &idx) const = 0;
    virtual bool isRowExpanded(const QModelIndex &idx) const = 0;
    virtual Span rowGeometry(const QModelIndex &idx) const = 0;

    virtual QModelIndex indexAt(int height) const = 0;
    virtual QModelIndex indexAbove(const QModelIndex &idx) const = 0;
    virtual QModelIndex indexBelow(const QModelIndex &idx) const = 0;
};
}

#endif /* KDGANTTABSTRACTROWCONTROLLER_H */
