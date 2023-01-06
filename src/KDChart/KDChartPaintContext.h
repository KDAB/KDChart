/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef PAINTCONTEXT_H
#define PAINTCONTEXT_H

#include "KDChartGlobal.h"
#include <QRectF>

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

namespace KDChart {

class AbstractCoordinatePlane;

/**
 * @brief Stores information about painting diagrams
 * \internal
 */
class KDCHART_EXPORT PaintContext
{
public:
    PaintContext();
    ~PaintContext();

    const QRectF rectangle() const;
    void setRectangle(const QRectF &rect);

    QPainter *painter() const;
    void setPainter(QPainter *painter);

    AbstractCoordinatePlane *coordinatePlane() const;
    void setCoordinatePlane(AbstractCoordinatePlane *plane);

private:
    class Private;
    Private *_d;
    Private *d_func()
    {
        return _d;
    }
    const Private *d_func() const
    {
        return _d;
    }
};
}

#endif /* PAINTCONTEXT_H */
