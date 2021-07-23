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

#ifndef KDCHARTTHREEDLINEATTRIBUTES_H
#define KDCHARTTHREEDLINEATTRIBUTES_H

#include <QMetaType>
#include "KDChartAbstractThreeDAttributes.h"
#include "KDChartGlobal.h"

namespace KDChart {

/**
    * @brief A set of 3D line attributes
    */
class KDCHART_EXPORT ThreeDLineAttributes : public AbstractThreeDAttributes
{
public:
    ThreeDLineAttributes();
    ThreeDLineAttributes(const ThreeDLineAttributes &);
    ThreeDLineAttributes &operator=(const ThreeDLineAttributes &);

    ~ThreeDLineAttributes() override;

    /* threeD lines specific */
    void setLineXRotation(const uint degrees);
    uint lineXRotation() const;
    void setLineYRotation(const uint degrees);
    uint lineYRotation() const;

    bool operator==(const ThreeDLineAttributes &) const;
    inline bool operator!=(const ThreeDLineAttributes &other) const
    {
        return !operator==(other);
    }

    KDCHART_DECLARE_SWAP_DERIVED(ThreeDLineAttributes)

    KDCHART_DECLARE_PRIVATE_DERIVED(ThreeDLineAttributes)

}; // End of class ThreeDLineAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::ThreeDLineAttributes &);
#endif /* QT_NO_DEBUG_STREAM */

KDCHART_DECLARE_SWAP_SPECIALISATION_DERIVED(KDChart::ThreeDLineAttributes)

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(KDChart::ThreeDLineAttributes, Q_MOVABLE_TYPE);
QT_END_NAMESPACE

Q_DECLARE_METATYPE(KDChart::ThreeDLineAttributes)

#endif // KDCHARTTHREEDLINEATTRIBUTES_H
