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

#include "KDChartThreeDLineAttributes.h"
#include "KDChartThreeDLineAttributes_p.h"

#include <QDebug>

#include <KDABLibFakes>

#define d d_func()

using namespace KDChart;

ThreeDLineAttributes::Private::Private()
{
}

ThreeDLineAttributes::ThreeDLineAttributes()
    : AbstractThreeDAttributes(new Private())
{
}

ThreeDLineAttributes::ThreeDLineAttributes(const ThreeDLineAttributes &r)
    : AbstractThreeDAttributes(new Private(*r.d))
{
}

ThreeDLineAttributes &ThreeDLineAttributes::operator=(const ThreeDLineAttributes &r)
{
    if (this == &r)
        return *this;

    *d = *r.d;

    return *this;
}

ThreeDLineAttributes::~ThreeDLineAttributes()
{
}

void ThreeDLineAttributes::init()
{
}

bool ThreeDLineAttributes::operator==(const ThreeDLineAttributes &r) const
{
    return (lineXRotation() == r.lineXRotation() && lineYRotation() == r.lineYRotation() && AbstractThreeDAttributes::operator==(r));
}

void ThreeDLineAttributes::setLineXRotation(const uint degrees)
{
    d->lineXRotation = degrees;
}

uint ThreeDLineAttributes::lineXRotation() const
{
    return d->lineXRotation;
}

void ThreeDLineAttributes::setLineYRotation(const uint degrees)
{
    d->lineYRotation = degrees;
}

uint ThreeDLineAttributes::lineYRotation() const
{
    return d->lineYRotation;
}

#if !defined(QT_NO_DEBUG_STREAM)

QDebug operator<<(QDebug dbg, const KDChart::ThreeDLineAttributes &a)
{
    dbg << "KDChart::ThreeDLineAttributes(";
    dbg = operator<<(dbg, static_cast<const AbstractThreeDAttributes &>(a));
    dbg << " lineXRotation=" << a.lineXRotation()
        << " lineYRotation=" << a.lineYRotation()
        << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
