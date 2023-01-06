/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTSTYLEOPTIONGANTTITEM_H
#define KDGANTTSTYLEOPTIONGANTTITEM_H

#include "kdganttglobal.h"

#include <QDebug>
#include <QRectF>
#include <QStyleOptionViewItem>

namespace KDGantt {
class AbstractGrid;
class KDGANTT_EXPORT StyleOptionGanttItem : public QStyleOptionViewItem
{
public:
    enum Position
    {
        Left,
        Right,
        Center,
        Hidden
    };

    StyleOptionGanttItem();
    StyleOptionGanttItem(const StyleOptionGanttItem &other);
    StyleOptionGanttItem &operator=(const StyleOptionGanttItem &other);

    QRectF boundingRect;
    QRectF itemRect;
    Position displayPosition = Left;
    AbstractGrid *grid = nullptr;
    QString text;
};
}

#ifndef QT_NO_DEBUG_STREAM

KDGANTT_EXPORT QDebug operator<<(QDebug dbg, KDGantt::StyleOptionGanttItem::Position p);
KDGANTT_EXPORT QDebug operator<<(QDebug dbg, const KDGantt::StyleOptionGanttItem &s);

#endif /* QT_NO_DEBUG_STREAM */

#endif /* KDGANTTSTYLEOPTIONGANTTITEM_H */
