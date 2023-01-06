/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartMarkerAttributes.h"
#include <QColor>
#include <QDebug>
#include <QMap>
#include <QPainterPath>
#include <QPen>
#include <QSizeF>
#include <qglobal.h>

#include <KDABLibFakes>

using namespace KDChart;

class MarkerAttributes::Private
{
    friend class ::KDChart::MarkerAttributes;

public:
    Private();

private:
    bool visible = false;
    bool threeD = false;
    QMap<uint, uint> markerStylesMap;
    uint markerStyle = MarkerSquare;
    QSizeF markerSize;
    QColor markerColor;
    QPainterPath customMarkerPath;
    QPen markerPen;
};

MarkerAttributes::Private::Private()
    : markerSize(10, 10)
    , markerPen(Qt::black)
{
}

MarkerAttributes::MarkerAttributes()
    : _d(new Private)
{
}

MarkerAttributes::MarkerAttributes(const MarkerAttributes &r)
    : _d(new Private(*r._d))
{
}

MarkerAttributes &MarkerAttributes::operator=(const MarkerAttributes &r)
{
    MarkerAttributes copy(r);
    copy.swap(*this);
    return *this;
}

MarkerAttributes::~MarkerAttributes()
{
    delete _d;
    _d = nullptr;
}

#define d d_func()

bool MarkerAttributes::operator==(const MarkerAttributes &r) const
{
    /*
    qDebug() << "MarkerAttributes::operator== finds"
            << "b" << (isVisible() == r.isVisible())
            << "c" << (markerStylesMap() == r.markerStylesMap())
            << "d" << (markerStyle() == r.markerStyle()) << markerStyle() <<r.markerStyle()
            << "e" << (markerSize() == r.markerSize())
            << "f" << (markerColor() == r.markerColor())
            << "p" << (customMarkerPath() == r.customMarkerPath())
            << "g" << (pen() == r.pen())
            << "h" << (markerColor() == r.markerColor()) << markerColor() << r.markerColor();
    */
    return (isVisible() == r.isVisible() && markerStylesMap() == r.markerStylesMap() && markerStyle() == r.markerStyle() && markerSize() == r.markerSize() && markerColor() == r.markerColor() && customMarkerPath() == r.customMarkerPath() && pen() == r.pen());
}

void MarkerAttributes::setVisible(bool visible)
{
    d->visible = visible;
}

bool MarkerAttributes::isVisible() const
{
    return d->visible;
}

void MarkerAttributes::setThreeD(bool value)
{
    d->threeD = value;
}

bool MarkerAttributes::threeD() const
{
    return d->threeD;
}

void MarkerAttributes::setMarkerStylesMap(const MarkerStylesMap &map)
{
    d->markerStylesMap = map;
}

MarkerAttributes::MarkerStylesMap MarkerAttributes::markerStylesMap() const
{
    return d->markerStylesMap;
}

void MarkerAttributes::setMarkerStyle(uint style)
{
    d->markerStyle = style;
}

uint MarkerAttributes::markerStyle() const
{
    return d->markerStyle;
}

void MarkerAttributes::setMarkerSize(const QSizeF &size)
{
    d->markerSize = size;
}

QSizeF MarkerAttributes::markerSize() const
{
    return d->markerSize;
}

void MarkerAttributes::setMarkerColor(const QColor &color)
{
    d->markerColor = color;
}

QColor MarkerAttributes::markerColor() const
{
    return d->markerColor;
}

void MarkerAttributes::setCustomMarkerPath(const QPainterPath &path)
{
    d->customMarkerPath = path;
}

QPainterPath MarkerAttributes::customMarkerPath() const
{
    return d->customMarkerPath;
}

void MarkerAttributes::setPen(const QPen &pen)
{
    d->markerPen = pen;
}

QPen MarkerAttributes::pen() const
{
    return d->markerPen;
}

#undef d

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const MarkerAttributes &ma)
{
    return dbg << "KDChart::MarkerAttributes("
               << "visible=" << ma.isVisible()
               << "markerStylesMap=" << ma.markerStylesMap()
               << "markerStyle=" << ma.markerStyle()
               << "markerColor=" << ma.markerColor()
               << "customMarkerPath=" << ma.customMarkerPath()
               << "pen=" << ma.pen()
               << ")";
}
#endif
