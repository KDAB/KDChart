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

#include "KDChartThreeDPieAttributes.h"
#include "KDChartThreeDPieAttributes_p.h"

#include <QDebug>

#include <KDABLibFakes>

#define d d_func()

using namespace KDChart;

ThreeDPieAttributes::Private::Private()
{
}

ThreeDPieAttributes::ThreeDPieAttributes()
    : AbstractThreeDAttributes(new Private())
{
    init();
}

ThreeDPieAttributes::ThreeDPieAttributes(const ThreeDPieAttributes &r)
    : AbstractThreeDAttributes(new Private(*r.d))
{
}

ThreeDPieAttributes &ThreeDPieAttributes::operator=(const ThreeDPieAttributes &r)
{
    if (this == &r)
        return *this;

    *d = *r.d;

    return *this;
}

ThreeDPieAttributes::~ThreeDPieAttributes()
{
}

void ThreeDPieAttributes::init()
{
    setDepth(-10);
}

bool ThreeDPieAttributes::operator==(const ThreeDPieAttributes &r) const
{
    return (useShadowColors() == r.useShadowColors() && AbstractThreeDAttributes::operator==(r));
}

void ThreeDPieAttributes::setUseShadowColors(bool shadowColors)
{
    d->useShadowColors = shadowColors;
}

bool ThreeDPieAttributes::useShadowColors() const
{
    return d->useShadowColors;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::ThreeDPieAttributes &a)
{
    dbg << "KDChart::ThreeDPieAttributes(";
    dbg = operator<<(dbg, static_cast<const AbstractThreeDAttributes &>(a));
    dbg << "useShadowColors=" << a.useShadowColors() << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
