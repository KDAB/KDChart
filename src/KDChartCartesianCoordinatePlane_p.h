/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTCARTESIANCOORDINATEPLANE_P_H
#define KDCHARTCARTESIANCOORDINATEPLANE_P_H

#include "KDChartAbstractCoordinatePlane_p.h"
#include "CartesianCoordinateTransformation.h"

namespace KDChart {

class CartesianCoordinatePlane::Private : public AbstractCoordinatePlane::Private
{
    friend class CartesianCoordinatePlane;
public:
    explicit Private()
        : initialResizeEventReceived ( false )
        , isometricScaling ( false )
        , horizontalMin(0), horizontalMax(0)
        , verticalMin(0), verticalMax(0)
    {}

    virtual ~Private() { }

    // the coordinate plane will calculate the coordinate transformation:
    CoordinateTransformation coordinateTransformation;
    // the whole drawing area, includes diagrams and axes, but maybe smaller
    // than (width, height):
    QRectF drawingArea;
    // the reactangle occupied by the diagrams, in plane coordinates
    QRectF diagramArea;
    // true after the first resize event came in
    bool initialResizeEventReceived;
    // true if the coordinate plane scales isometrically
    bool isometricScaling;

    qreal horizontalMin;
    qreal horizontalMax;
    qreal verticalMin;
    qreal verticalMax;
};


inline CartesianCoordinatePlane::CartesianCoordinatePlane( Private * p, QWidget * parent )
    : AbstractCoordinatePlane( p, parent ) {}
inline CartesianCoordinatePlane::Private * CartesianCoordinatePlane::d_func()
{ return static_cast<Private*>( AbstractCoordinatePlane::d_func() ); }
inline const CartesianCoordinatePlane::Private * CartesianCoordinatePlane::d_func() const
{ return static_cast<const Private*>( AbstractCoordinatePlane::d_func() ); }

}

#endif /* KDCHARTBARDIAGRAM_P_H */

