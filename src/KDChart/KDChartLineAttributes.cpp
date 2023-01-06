/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartLineAttributes.h"
#include <QDebug>

#include <KDABLibFakes>

#define d d_func()

using namespace KDChart;

class LineAttributes::Private
{
    friend class LineAttributes;

public:
    Private();

private:
    // Areas
    MissingValuesPolicy missingValuesPolicy = MissingValuesAreBridged;
    bool displayArea = false;
    uint transparency = 255;
    int areaBoundingDataset = -1;
};

LineAttributes::Private::Private()
{
}

LineAttributes::LineAttributes()
    : _d(new Private())
{
}

LineAttributes::LineAttributes(const LineAttributes &r)
    : _d(new Private(*r.d))
{
}

LineAttributes &LineAttributes::operator=(const LineAttributes &r)
{
    if (this == &r)
        return *this;

    *d = *r.d;

    return *this;
}

LineAttributes::~LineAttributes()
{
    delete _d;
    _d = nullptr;
}

bool LineAttributes::operator==(const LineAttributes &r) const
{
    return missingValuesPolicy() == r.missingValuesPolicy() && displayArea() == r.displayArea() && transparency() == r.transparency() && areaBoundingDataset() == r.areaBoundingDataset();
}

void LineAttributes::setMissingValuesPolicy(MissingValuesPolicy policy)
{
    d->missingValuesPolicy = policy;
}

LineAttributes::MissingValuesPolicy LineAttributes::missingValuesPolicy() const
{
    return d->missingValuesPolicy;
}

void LineAttributes::setDisplayArea(bool display)
{
    d->displayArea = display;
}

bool LineAttributes::displayArea() const
{
    return d->displayArea;
}

void LineAttributes::setTransparency(uint alpha)
{
    if (alpha > 255)
        alpha = 255;
    d->transparency = alpha;
}

uint LineAttributes::transparency() const
{
    return d->transparency;
}

void LineAttributes::setAreaBoundingDataset(int dataset)
{
    d->areaBoundingDataset = dataset;
}

int LineAttributes::areaBoundingDataset() const
{
    return d->areaBoundingDataset;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::LineAttributes &a)
{
    dbg << "KDChart::LineAttributes("
        //     MissingValuesPolicy missingValuesPolicy;
        << "bool=" << a.displayArea()
        << "transparency=" << a.transparency()
        << "areaBoundingDataset=" << a.areaBoundingDataset()
        << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
