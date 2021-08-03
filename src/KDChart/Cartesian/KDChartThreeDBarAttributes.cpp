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

#include "KDChartThreeDBarAttributes.h"
#include "KDChartThreeDBarAttributes_p.h"

#include <QDebug>

#include <KDABLibFakes>

#define d d_func()

using namespace KDChart;

ThreeDBarAttributes::Private::Private()
{
}

ThreeDBarAttributes::ThreeDBarAttributes()
    : AbstractThreeDAttributes(new Private())
{
}

ThreeDBarAttributes::ThreeDBarAttributes(const ThreeDBarAttributes &r)
    : AbstractThreeDAttributes(new Private(*r.d))
{
}

ThreeDBarAttributes &ThreeDBarAttributes::operator=(const ThreeDBarAttributes &r)
{
    if (this == &r)
        return *this;

    *d = *r.d;

    return *this;
}

ThreeDBarAttributes::~ThreeDBarAttributes()
{
}

void ThreeDBarAttributes::init()
{
}

bool ThreeDBarAttributes::operator==(const ThreeDBarAttributes &r) const
{
    return (useShadowColors() == r.useShadowColors() && angle() == r.angle() && AbstractThreeDAttributes::operator==(r));
}

void ThreeDBarAttributes::setUseShadowColors(bool shadowColors)
{
    d->useShadowColors = shadowColors;
}

bool ThreeDBarAttributes::useShadowColors() const
{
    return d->useShadowColors;
}

void ThreeDBarAttributes::setAngle(uint threeDAngle)
{
    d->angle = threeDAngle;
}

uint ThreeDBarAttributes::angle() const
{
    return d->angle;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::ThreeDBarAttributes &a)
{
    dbg << "KDChart::ThreeDBarAttributes(";
    dbg = operator<<(dbg, static_cast<const AbstractThreeDAttributes &>(a));
    dbg << "useShadowColors=" << a.useShadowColors()
        << "angle=" << a.angle() << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
