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

#ifndef KDCHARTABSTRACTGRID_H
#define KDCHARTABSTRACTGRID_H

// #include <QObject>
#include <QPair>

#include "KDChartAbstractCoordinatePlane.h"
#include "KDChartGridAttributes.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartCartesianAxis.h"

namespace KDChart {

    class PaintContext;

    /**
     * \internal
     *
     * \brief Base class for grid classes: cartesian, polar, ...
     *
     * The AbstractGrid interface is the base class used by
     * AbstractCoordinatePlane, for calculating and for drawing
     * the grid lines of the plane.
     */
    class AbstractGrid // : public QObject
    {
        // Q_OBJECT
    protected:
        AbstractGrid ();
        virtual ~AbstractGrid();

    public:
        static bool isBoundariesValid(const QRectF& r );
        static bool isBoundariesValid(const QPair<QPointF,QPointF>& b );

        virtual void calculateGrid(
            qreal initialStart,
            qreal initialEnd,
            qreal initialFontSize );

        virtual void drawGrid( PaintContext* context );

        AbstractCoordinatePlane* plane;
        GridAttributes gridAttributes;

        qreal startValue;
        qreal endValue;
        qreal stepWidth;
    };

}

#endif
