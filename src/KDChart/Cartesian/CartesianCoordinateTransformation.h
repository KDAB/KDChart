/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef CARTESIANCOORDINATETRANSFORMATION_H
#define CARTESIANCOORDINATETRANSFORMATION_H

#include <QList>
#include <QPointF>
#include <QRectF>

#include "KDChartZoomParameters.h"

#include <cmath>
#include <limits>

namespace KDChart {

// FIXME: if this struct is used more often, we need to make it a class
// with proper accessor methods:

/**
 * \internal
 */
struct CoordinateTransformation
{
    CoordinateTransformation()
    {
    }

    CartesianCoordinatePlane::AxesCalcMode axesCalcModeY = CartesianCoordinatePlane::Linear;
    CartesianCoordinatePlane::AxesCalcMode axesCalcModeX = CartesianCoordinatePlane::Linear;

    ZoomParameters zoom;

    QTransform transform;
    QTransform backTransform;
    // a logarithmic scale cannot cross zero, so we have to know which side we are on.
    bool isPositiveX = true;
    bool isPositiveY = true;

    qreal logTransform(qreal value, bool isPositiveRange) const
    {
        if (isPositiveRange) {
            return log10(value);
        } else {
            return -log10(-value);
        }
    }

    qreal logTransformBack(qreal value, bool wasPositive) const
    {
        if (wasPositive) {
            return pow(10.0, value);
        } else {
            return -pow(10.0, -value);
        }
    }

    void updateTransform(const QRectF &constDataRect, const QRectF &screenRect)
    {
        QRectF dataRect = constDataRect;
        if (axesCalcModeX == CartesianCoordinatePlane::Logarithmic) {
            // the data will be scaled by logTransform() later, so scale its bounds as well
            isPositiveX = dataRect.left() >= 0.0;
            dataRect.setLeft(logTransform(dataRect.left(), isPositiveX));
            dataRect.setRight(logTransform(dataRect.right(), isPositiveX));
        }
        if (axesCalcModeY == CartesianCoordinatePlane::Logarithmic) {
            isPositiveY = dataRect.top() >= 0.0;
            dataRect.setTop(logTransform(dataRect.top(), isPositiveY));
            dataRect.setBottom(logTransform(dataRect.bottom(), isPositiveY));
        }

        transform.reset();
        // read the following transformation sequence from bottom to top(!)
        transform.translate(screenRect.left(), screenRect.bottom());
        transform.scale(screenRect.width(), screenRect.height());

        // TODO: mirror in case of "reverse" axes?

        // transform into screen space
        transform.translate(0.5, -0.5);
        transform.scale(zoom.xFactor, zoom.yFactor);
        transform.translate(-zoom.xCenter, 1.0 - zoom.yCenter);
        // zoom
        transform.scale(1.0 / dataRect.width(), 1.0 / dataRect.height());
        transform.translate(-dataRect.left(), -dataRect.bottom());
        // transform into the unit square

        backTransform = transform.inverted();
    }

    // convert data space point to screen point
    inline QPointF translate(const QPointF &dataPoint) const
    {
        QPointF data = dataPoint;
        if (axesCalcModeX == CartesianCoordinatePlane::Logarithmic) {
            data.setX(logTransform(data.x(), isPositiveX));
        }
        if (axesCalcModeY == CartesianCoordinatePlane::Logarithmic) {
            data.setY(logTransform(data.y(), isPositiveY));
        }

        return transform.map(data);
    }

    // convert screen point to data space point
    inline const QPointF translateBack(const QPointF &screenPoint) const
    {
        QPointF ret = backTransform.map(screenPoint);
        if (axesCalcModeX == CartesianCoordinatePlane::Logarithmic) {
            ret.setX(logTransformBack(ret.x(), isPositiveX));
        }
        if (axesCalcModeY == CartesianCoordinatePlane::Logarithmic) {
            ret.setY(logTransformBack(ret.y(), isPositiveY));
        }
        return ret;
    }
};

typedef QList<CoordinateTransformation> CoordinateTransformationList;
}

#endif
