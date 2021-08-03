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

#include "KDChartPieAttributes.h"
#include "KDChartPieAttributes_p.h"

#include <QDebug>

#include <KDABLibFakes>

#define d d_func()

using namespace KDChart;

PieAttributes::Private::Private()
{
}

PieAttributes::PieAttributes()
    : _d(new Private())
{
}

PieAttributes::PieAttributes(const PieAttributes &r)
    : _d(new Private(*r.d))
{
}

PieAttributes &PieAttributes::operator=(const PieAttributes &r)
{
    if (this == &r)
        return *this;

    *d = *r.d;

    return *this;
}

PieAttributes::~PieAttributes()
{
    delete _d;
    _d = nullptr;
}

bool PieAttributes::operator==(const PieAttributes &r) const
{
    return explodeFactor() == r.explodeFactor() && gapFactor(true) == r.gapFactor(true) && gapFactor(false) == r.gapFactor(false);
}

void PieAttributes::init()
{
}

void PieAttributes::setExplode(bool enabled)
{
    d->explodeFactor = (enabled ? 0.1 : 0.0);
}

bool PieAttributes::explode() const
{
    return (d->explodeFactor != 0.0);
}

void PieAttributes::setExplodeFactor(qreal factor)
{
    d->explodeFactor = factor;
}

qreal PieAttributes::explodeFactor() const
{
    return d->explodeFactor;
}

void PieAttributes::setGapFactor(bool circular, qreal factor)
{
    if (circular)
        d->tangentialGapFactor = factor;
    else
        d->radialGapFactor = factor;
}

qreal PieAttributes::gapFactor(bool circular) const
{
    return circular ? d->tangentialGapFactor : d->radialGapFactor;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::PieAttributes &a)
{
    dbg << "KDChart::PieAttributes(";
    dbg << "explodeFactor=" << a.explodeFactor() << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
