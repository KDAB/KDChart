/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartValueTrackerAttributes.h"

#include <KDABLibFakes>
#include <QBrush>
#include <QPen>
#include <QSizeF>

#define d d_func()

using namespace KDChart;

class ValueTrackerAttributes::Private
{
    friend class ValueTrackerAttributes;

public:
    Private();

private:
    QPen linePen;
    QPen markerPen;
    QBrush markerBrush;
    QBrush arrowBrush;
    QSizeF markerSize;
    bool enabled = false;
    QBrush areaBrush;
    Qt::Orientations orientations;
};

ValueTrackerAttributes::Private::Private()
    : linePen(QPen(QColor(80, 80, 80, 200)))
    , markerSize(QSizeF(6.0, 6.0))
    , areaBrush(QBrush())
    , orientations(Qt::Vertical | Qt::Horizontal)
{
    markerPen = linePen;
    arrowBrush = linePen.color();
}

ValueTrackerAttributes::ValueTrackerAttributes()
    : _d(new Private())
{
}

ValueTrackerAttributes::ValueTrackerAttributes(const ValueTrackerAttributes &r)
    : _d(new Private(*r.d))
{
}

ValueTrackerAttributes &ValueTrackerAttributes::operator=(const ValueTrackerAttributes &r)
{
    if (this == &r)
        return *this;

    *d = *r.d;

    return *this;
}

ValueTrackerAttributes::~ValueTrackerAttributes()
{
    delete _d;
    _d = nullptr;
}

bool ValueTrackerAttributes::operator==(const ValueTrackerAttributes &r) const
{
    return (linePen() == r.linePen() && markerPen() == r.markerPen() && markerBrush() == r.markerBrush() && arrowBrush() == r.arrowBrush() && areaBrush() == r.areaBrush() && markerSize() == r.markerSize() && isEnabled() == r.isEnabled());
}

void ValueTrackerAttributes::setPen(const QPen &pen)
{
    d->linePen = pen;
    d->markerPen = pen;
    d->markerBrush = QBrush();
    d->arrowBrush = pen.color();
}

QPen ValueTrackerAttributes::pen() const
{
    return d->linePen;
}

void ValueTrackerAttributes::setLinePen(const QPen &pen)
{
    d->linePen = pen;
}

QPen ValueTrackerAttributes::linePen() const
{
    return d->linePen;
}

void ValueTrackerAttributes::setMarkerPen(const QPen &pen)
{
    d->markerPen = pen;
}

QPen ValueTrackerAttributes::markerPen() const
{
    return d->markerPen;
}

void ValueTrackerAttributes::setMarkerBrush(const QBrush &brush)
{
    d->markerBrush = brush;
}

QBrush ValueTrackerAttributes::markerBrush() const
{
    return d->markerBrush;
}

void ValueTrackerAttributes::setArrowBrush(const QBrush &brush)
{
    d->arrowBrush = brush;
}

QBrush ValueTrackerAttributes::arrowBrush() const
{
    return d->arrowBrush;
}

void ValueTrackerAttributes::setAreaBrush(const QBrush &brush)
{
    d->areaBrush = brush;
}

QBrush ValueTrackerAttributes::areaBrush() const
{
    return d->areaBrush;
}

void ValueTrackerAttributes::setMarkerSize(const QSizeF &size)
{
    d->markerSize = size;
}

QSizeF ValueTrackerAttributes::markerSize() const
{
    return d->markerSize;
}

Qt::Orientations ValueTrackerAttributes::orientations() const
{
    return d->orientations;
}

void ValueTrackerAttributes::setOrientations(Qt::Orientations orientations)
{
    d->orientations = orientations;
}

void ValueTrackerAttributes::setEnabled(bool enabled)
{
    d->enabled = enabled;
}

bool ValueTrackerAttributes::isEnabled() const
{
    return d->enabled;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::ValueTrackerAttributes &va)
{
    dbg << "KDChart::ValueTrackerAttributes("
        << "linePen=" << va.linePen()
        << "markerPen=" << va.markerPen()
        << "markerBrush=" << va.markerBrush()
        << "arrowBrush=" << va.arrowBrush()
        << "markerSize=" << va.markerSize()
        << "enabled=" << va.isEnabled()
        << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
