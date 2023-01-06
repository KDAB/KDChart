/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTTHREEDATTRIBUTES_H
#define KDCHARTABSTRACTTHREEDATTRIBUTES_H

#include "KDChartGlobal.h"
#include <QMetaType>

namespace KDChart {

/**
 * @brief Base class for 3D attributes
 */
class KDCHART_EXPORT AbstractThreeDAttributes
{
public:
    AbstractThreeDAttributes();
    AbstractThreeDAttributes(const AbstractThreeDAttributes &);
    AbstractThreeDAttributes &operator=(const AbstractThreeDAttributes &);

    virtual ~AbstractThreeDAttributes() = 0;

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setDepth(qreal depth);
    qreal depth() const;

    // returns the depth(), if is isEnabled() is true, otherwise returns 0.0
    qreal validDepth() const;

    bool isThreeDBrushEnabled() const;
    void setThreeDBrushEnabled(bool enabled);
    virtual QBrush threeDBrush(const QBrush &brush, const QRectF &rect) const;

    bool operator==(const AbstractThreeDAttributes &) const;
    inline bool operator!=(const AbstractThreeDAttributes &other) const
    {
        return !operator==(other);
    }

    KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC(AbstractThreeDAttributes)

    KDCHART_DECLARE_SWAP_BASE(AbstractThreeDAttributes)

}; // End of class AbstractThreeDAttributes
}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::AbstractThreeDAttributes &);
#endif /* QT_NO_DEBUG_STREAM */

#endif // KDCHARTABSTRACTTHREEDATTRIBUTES_H
