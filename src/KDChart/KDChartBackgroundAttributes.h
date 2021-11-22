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

#ifndef KDCHARTBACKGROUNDATTRIBUTES_H
#define KDCHARTBACKGROUNDATTRIBUTES_H

#include <QDebug>
#include <QMetaType>
#include <QBrush>
#include "KDChartGlobal.h"

namespace KDChart {

/**
  * Set of attributes usable for background pixmaps
  */
class KDCHART_EXPORT BackgroundAttributes
{
public:
    BackgroundAttributes();
    BackgroundAttributes(const BackgroundAttributes &);
    BackgroundAttributes &operator=(const BackgroundAttributes &);

    ~BackgroundAttributes();

    enum BackgroundPixmapMode {
        BackgroundPixmapModeNone,
        BackgroundPixmapModeCentered,
        BackgroundPixmapModeScaled,
        BackgroundPixmapModeStretched
    };

    void setVisible(bool visible);
    bool isVisible() const;

    void setBrush(const QBrush &brush);
    QBrush brush() const;

    void setPixmapMode(BackgroundPixmapMode mode);
    BackgroundPixmapMode pixmapMode() const;

    void setPixmap(const QPixmap &backPixmap);
    QPixmap pixmap() const;

    bool operator==(const BackgroundAttributes &) const;
    inline bool operator!=(const BackgroundAttributes &other) const
    {
        return !operator==(other);
    }

    bool isEqualTo(const BackgroundAttributes &other, bool ignorePixmap = false) const;

private:
    KDCHART_DECLARE_PRIVATE_BASE_VALUE(BackgroundAttributes)
}; // End of class BackgroundAttributes
}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::BackgroundAttributes &);
#endif /* QT_NO_DEBUG_STREAM */

KDCHART_DECLARE_SWAP_SPECIALISATION(KDChart::BackgroundAttributes)

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(KDChart::BackgroundAttributes, Q_MOVABLE_TYPE);
QT_END_NAMESPACE

Q_DECLARE_METATYPE(KDChart::BackgroundAttributes)

#endif // KDCHARTBACKGROUNDATTRIBUTES_H
