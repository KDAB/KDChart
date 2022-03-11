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

#include "KDChartPaintContext.h"
#include "KDChartAbstractCoordinatePlane.h"

#include <QRectF>
#include <QPainter>

#include <KDABLibFakes>

using namespace KDChart;

#define d (d_func())

class PaintContext::Private
{
public:
    QPainter *painter = nullptr;
    QRectF rect;
    AbstractCoordinatePlane *plane = nullptr;

    Private()
    {
    }
};

PaintContext::PaintContext()
    : _d(new Private())
{
}

PaintContext::~PaintContext()
{
    delete _d;
}

const QRectF PaintContext::rectangle() const
{
    return d->rect;
}

void PaintContext::setRectangle(const QRectF &rect)
{
    d->rect = rect;
}

QPainter *PaintContext::painter() const
{
    return d->painter;
}

void PaintContext::setPainter(QPainter *painter)
{
    d->painter = painter;
}

AbstractCoordinatePlane *PaintContext::coordinatePlane() const
{
    return d->plane;
}

void PaintContext::setCoordinatePlane(AbstractCoordinatePlane *plane)
{
    d->plane = plane;
}
