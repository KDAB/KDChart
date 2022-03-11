/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef KDGANTTSTYLEOPTIONGANTTITEM_H
#define KDGANTTSTYLEOPTIONGANTTITEM_H

#include "kdganttglobal.h"

#include <QStyleOptionViewItem>
#include <QRectF>
#include <QDebug>

namespace KDGantt {
class AbstractGrid;
class KDGANTT_EXPORT StyleOptionGanttItem : public QStyleOptionViewItem
{
public:
    enum Position {
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
