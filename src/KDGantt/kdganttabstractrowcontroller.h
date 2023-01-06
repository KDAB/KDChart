/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTABSTRACTROWCONTROLLER_H
#define KDGANTTABSTRACTROWCONTROLLER_H

#include "kdganttglobal.h"
#include <QPair>

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
