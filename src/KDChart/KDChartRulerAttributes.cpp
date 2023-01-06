/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartRulerAttributes.h"

#include <limits>

#include <QDebug>
#include <QPen>

#include <KDABLibFakes>

#define d d_func()

using namespace KDChart;

class RulerAttributes::Private
{
    friend class RulerAttributes;

public:
    Private();

private:
    QPen tickMarkPen;
    QPen majorTickMarkPen;
    QPen minorTickMarkPen;

    bool majorTickMarkPenIsSet : 1;
    bool minorTickMarkPenIsSet : 1;

    bool showMajorTickMarks : 1;
    bool showMinorTickMarks : 1;

    bool showRulerLine : 1;

    bool majorTickLengthIsSet : 1;
    bool minorTickLengthIsSet : 1;

    bool showFirstTick : 1;

    int labelMargin = -1;
    int majorTickLength = 3;
    int minorTickLength = 2;

    RulerAttributes::TickMarkerPensMap customTickMarkPens;
};

RulerAttributes::Private::Private()
    : tickMarkPen(Qt::black)
    , majorTickMarkPen(Qt::black)
    , minorTickMarkPen(Qt::black)
    , majorTickMarkPenIsSet(false)
    , minorTickMarkPenIsSet(false)
    , showMajorTickMarks(true)
    , showMinorTickMarks(true)
    , showRulerLine(false)
    , majorTickLengthIsSet(false)
    , minorTickLengthIsSet(false)
    , showFirstTick(true)
{
    tickMarkPen.setCapStyle(Qt::FlatCap);
    majorTickMarkPen.setCapStyle(Qt::FlatCap);
    minorTickMarkPen.setCapStyle(Qt::FlatCap);
}

RulerAttributes::RulerAttributes()
    : _d(new Private())
{
    // this block left empty intentionally
}

RulerAttributes::RulerAttributes(const RulerAttributes &r)
    : _d(new Private(*r.d))
{
}

void RulerAttributes::setTickMarkPen(const QPen &pen)
{
    d->tickMarkPen = pen;
}

QPen RulerAttributes::tickMarkPen() const
{
    return d->tickMarkPen;
}

void RulerAttributes::setMajorTickMarkPen(const QPen &pen)
{
    d->majorTickMarkPen = pen;
    d->majorTickMarkPenIsSet = true;
}

bool RulerAttributes::majorTickMarkPenIsSet() const
{
    return d->majorTickMarkPenIsSet;
}

QPen RulerAttributes::majorTickMarkPen() const
{
    return d->majorTickMarkPenIsSet ? d->majorTickMarkPen : d->tickMarkPen;
}

void RulerAttributes::setMinorTickMarkPen(const QPen &pen)
{
    d->minorTickMarkPen = pen;
    d->minorTickMarkPenIsSet = true;
}

bool RulerAttributes::minorTickMarkPenIsSet() const
{
    return d->minorTickMarkPenIsSet;
}

QPen RulerAttributes::minorTickMarkPen() const
{
    return d->minorTickMarkPenIsSet ? d->minorTickMarkPen : d->tickMarkPen;
}

void RulerAttributes::setTickMarkPen(qreal value, const QPen &pen)
{
    if (!d->customTickMarkPens.contains(value))
        d->customTickMarkPens.insert(value, pen);
}

QPen RulerAttributes::tickMarkPen(qreal value) const
{
    QMapIterator<qreal, QPen> it(d->customTickMarkPens);
    while (it.hasNext()) {
        it.next();
        if (qAbs(value - it.key()) < std::numeric_limits<float>::epsilon())
            return it.value();
    }
    return d->tickMarkPen;
}

RulerAttributes::TickMarkerPensMap RulerAttributes::tickMarkPens() const
{
    return d->customTickMarkPens;
}

bool RulerAttributes::hasTickMarkPenAt(qreal value) const
{
    QMapIterator<qreal, QPen> it(d->customTickMarkPens);
    while (it.hasNext()) {
        it.next();
        if (qAbs(value - it.key()) < std::numeric_limits<float>::epsilon())
            return true;
    }
    return false;
}

void RulerAttributes::setTickMarkColor(const QColor &color)
{
    d->tickMarkPen.setColor(color);
}

QColor RulerAttributes::tickMarkColor() const
{
    return d->tickMarkPen.color();
}

void RulerAttributes::setShowMajorTickMarks(bool show)
{
    d->showMajorTickMarks = show;
}

bool RulerAttributes::showMajorTickMarks() const
{
    return d->showMajorTickMarks;
}

void RulerAttributes::setShowMinorTickMarks(bool show)
{
    d->showMinorTickMarks = show;
}

bool RulerAttributes::showMinorTickMarks() const
{
    return d->showMinorTickMarks;
}

void RulerAttributes::setLabelMargin(int margin)
{
    d->labelMargin = margin;
}

int RulerAttributes::labelMargin() const
{
    return d->labelMargin;
}

void RulerAttributes::setMajorTickMarkLength(int length)
{
    d->majorTickLength = length;
    d->majorTickLengthIsSet = true;
}

int RulerAttributes::majorTickMarkLength() const
{
    return d->majorTickLength;
}

bool RulerAttributes::majorTickMarkLengthIsSet() const
{
    return d->majorTickLengthIsSet;
}

void RulerAttributes::setMinorTickMarkLength(int length)
{
    d->minorTickLength = length;
    d->minorTickLengthIsSet = true;
}

int RulerAttributes::minorTickMarkLength() const
{
    return d->minorTickLength;
}

bool RulerAttributes::minorTickMarkLengthIsSet() const
{
    return d->minorTickLengthIsSet;
}

void RulerAttributes::setShowFirstTick(bool show)
{
    d->showFirstTick = show;
}

bool RulerAttributes::showFirstTick() const
{
    return d->showFirstTick;
}

RulerAttributes &RulerAttributes::operator=(const RulerAttributes &r)
{
    if (this == &r)
        return *this;

    *d = *r.d;

    return *this;
}

RulerAttributes::~RulerAttributes()
{
    delete _d;
    _d = nullptr;
}

bool RulerAttributes::operator==(const RulerAttributes &r) const
{
    bool isEqual = tickMarkPen() == r.tickMarkPen() && majorTickMarkPen() == r.majorTickMarkPen() && minorTickMarkPen() == r.minorTickMarkPen() && majorTickMarkPenIsSet() == r.majorTickMarkPenIsSet() && minorTickMarkPenIsSet() == r.minorTickMarkPenIsSet() && showMajorTickMarks() == r.showMajorTickMarks() && showMinorTickMarks() == r.showMinorTickMarks() && showRulerLine() == r.showRulerLine() && majorTickMarkLengthIsSet() == r.majorTickMarkLengthIsSet() && minorTickMarkLengthIsSet() == r.minorTickMarkLengthIsSet() && showFirstTick() == r.showFirstTick() && d->customTickMarkPens.size() == r.d->customTickMarkPens.size();
    if (!isEqual) {
        return false;
    }
    QMap<qreal, QPen>::ConstIterator it = d->customTickMarkPens.constBegin();
    QMap<qreal, QPen>::ConstIterator it2 = r.d->customTickMarkPens.constBegin();
    for (; it != d->customTickMarkPens.constEnd(); ++it, ++it2) {
        if (it.key() != it2.key() || it.value() != it2.value()) {
            return false;
        }
    }
    return true;
}

void RulerAttributes::setShowRulerLine(bool show)
{
    d->showRulerLine = show;
}

bool RulerAttributes::showRulerLine() const
{
    return d->showRulerLine;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::RulerAttributes &a)
{
    dbg << "KDChart::RulerAttributes("
        << "tickMarkPen=" << a.tickMarkPen()
        << "majorTickMarkPen=" << a.majorTickMarkPen()
        << "minorTickMarkPen=" << a.minorTickMarkPen();
    const RulerAttributes::TickMarkerPensMap pens(a.tickMarkPens());
    QMapIterator<qreal, QPen> it(pens);
    while (it.hasNext()) {
        it.next();
        dbg << "customTickMarkPen=(" << it.value() << " : " << it.key() << ")";
    }
    dbg << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
