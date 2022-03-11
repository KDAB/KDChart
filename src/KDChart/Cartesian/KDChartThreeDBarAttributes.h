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

#ifndef KDCHARTTHREEDBARATTRIBUTES_H
#define KDCHARTTHREEDBARATTRIBUTES_H

#include <QMetaType>
#include "KDChartAbstractThreeDAttributes.h"
#include "KDChartGlobal.h"

namespace KDChart {

/**
    * @brief A set of 3D bar attributes
    */
class KDCHART_EXPORT ThreeDBarAttributes : public AbstractThreeDAttributes
{
public:
    ThreeDBarAttributes();
    ThreeDBarAttributes(const ThreeDBarAttributes &);
    ThreeDBarAttributes &operator=(const ThreeDBarAttributes &);

    ~ThreeDBarAttributes() override;

    /* threeD Bars specific */
    void setUseShadowColors(bool useShadowColors);
    bool useShadowColors() const;

    //Pending Michel I am not sure this will be used
    void setAngle(uint threeDAngle);
    uint angle() const;

    bool operator==(const ThreeDBarAttributes &) const;
    inline bool operator!=(const ThreeDBarAttributes &other) const
    {
        return !operator==(other);
    }

    KDCHART_DECLARE_SWAP_DERIVED(ThreeDBarAttributes)

    KDCHART_DECLARE_PRIVATE_DERIVED(ThreeDBarAttributes)

}; // End of class ThreeDBarAttributes
}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::ThreeDBarAttributes &);
#endif /* QT_NO_DEBUG_STREAM */

KDCHART_DECLARE_SWAP_SPECIALISATION_DERIVED(KDChart::ThreeDBarAttributes)

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(KDChart::ThreeDBarAttributes, Q_MOVABLE_TYPE);
QT_END_NAMESPACE

Q_DECLARE_METATYPE(KDChart::ThreeDBarAttributes)

#endif // KDCHARTTHREEDBARATTRIBUTES_H
