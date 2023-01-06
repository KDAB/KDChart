/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartAbstractThreeDAttributes.h"
#include "KDChartAbstractThreeDAttributes_p.h"

#include <KDABLibFakes>
#include <QBrush>
#include <QDebug>

#define d d_func()

using namespace KDChart;

AbstractThreeDAttributes::Private::Private()
{
}

AbstractThreeDAttributes::AbstractThreeDAttributes()
    : _d(new Private())
{
}

AbstractThreeDAttributes::AbstractThreeDAttributes(const AbstractThreeDAttributes &r)
    : _d(new Private(*r.d))
{
}

AbstractThreeDAttributes &AbstractThreeDAttributes::operator=(const AbstractThreeDAttributes &r)
{
    if (this == &r)
        return *this;

    *d = *r.d;

    return *this;
}

AbstractThreeDAttributes::~AbstractThreeDAttributes()
{
    delete _d;
    _d = nullptr;
}

bool AbstractThreeDAttributes::operator==(const AbstractThreeDAttributes &r) const
{
    return isEnabled() == r.isEnabled() && depth() == r.depth() && isThreeDBrushEnabled() == r.isThreeDBrushEnabled();
}

void AbstractThreeDAttributes::init()
{
}

void AbstractThreeDAttributes::setEnabled(bool enabled)
{
    d->enabled = enabled;
}

bool AbstractThreeDAttributes::isEnabled() const
{
    return d->enabled;
}

void AbstractThreeDAttributes::setDepth(qreal depth)
{
    d->depth = depth;
}

qreal AbstractThreeDAttributes::depth() const
{
    return d->depth;
}

qreal AbstractThreeDAttributes::validDepth() const
{
    return isEnabled() ? d->depth : 0.0;
}

bool AbstractThreeDAttributes::isThreeDBrushEnabled() const
{
    return d->threeDBrushEnabled;
}

void AbstractThreeDAttributes::setThreeDBrushEnabled(bool enabled)
{
    d->threeDBrushEnabled = enabled;
}

QBrush AbstractThreeDAttributes::threeDBrush(const QBrush &brush, const QRectF &rect) const
{
    if (isThreeDBrushEnabled()) {
        QLinearGradient gr(rect.topLeft(), rect.bottomRight());
        gr.setColorAt(0.0, brush.color());
        gr.setColorAt(0.5, brush.color().lighter(180));
        gr.setColorAt(1.0, brush.color());
        return QBrush(gr);
    }
    return brush;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::AbstractThreeDAttributes &a)
{
    dbg << "enabled=" << a.isEnabled()
        << "depth=" << a.depth();
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
