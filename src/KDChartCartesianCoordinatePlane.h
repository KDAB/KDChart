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

#ifndef KDCHARTCARTESIANCOORDINATEPLANE_H
#define KDCHARTCARTESIANCOORDINATEPLANE_H

#include "KDChartAbstractCoordinatePlane.h"

namespace KDChart {

    class PaintContext;
    class AbstractDiagram;
    class CartesianAxis;

    class KDCHART_EXPORT CartesianCoordinatePlane : public AbstractCoordinatePlane
    {
        Q_OBJECT
        Q_DISABLE_COPY( CartesianCoordinatePlane )
        KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( CartesianCoordinatePlane, QWidget* )
    friend class CartesianAxis;

    public:
        explicit CartesianCoordinatePlane ( QWidget* parent = 0 );
        ~CartesianCoordinatePlane();

        void addDiagram ( AbstractDiagram* diagram );

        void setIsometricScaling ( bool onOff );

        bool doesIsometricScaling();

        const QPointF translate ( const QPointF& diagramPoint ) const;

        virtual double zoomFactorX() const;
        virtual double zoomFactorY() const;

        virtual void setZoomFactorX( double factor );
        virtual void setZoomFactorY( double factor );

        virtual QPointF zoomCenter() const;

        virtual void setZoomCenter( QPointF center );


        /**
         * Allows setting the boundaries of the visible value space area
         * that the plane displays in horizontal direction. The horizontal
         * viewport. To disable use of this range, set both values to the same
         * thing, which constiutes a null range.
         * @param  A pair of values representing the smalles and the largest
         * horizontal value space coordinate that are still visible.
         */
        void setHorizontalRange( const QPair<qreal, qreal> & );

        /**
         * Allows setting the boundaries of the visible value space area
         * that the plane displays in vertical direction. The vertical
         * viewport. To disable use of this range, set both values to the same
         * thing, which constiutes a null range.
         * @param  A pair of values representing the smalles and the largest
         * vertical value space coordinate that are still visible.
         */
        void setVerticalRange( const QPair<qreal, qreal> & );

        /**
         * @return The largest and smallest visible horizontal value space
         * value. If this is not explicitely set,or if both values are the same,
         * the plane will use the union of the dataBoundaries of all
         * associated diagrams.
         * \see KDChart::AbstractDiagram::dataBoundaries
         */
        QPair<qreal, qreal> horizontalRange() const;

        /**
         * @return The largest and smallest visible horizontal value space
         * value. If this is not explicitely set, or if both values are the same,
         * the plane will use the union of the dataBoundaries of all
         * associated diagrams.
         * \see KDChart::AbstractDiagram::dataBoundaries
         */
        QPair<qreal, qreal> verticalRange() const;

        enum AxesCalcMode { Linear, Logarithmic };

        AxesCalcMode axesCalcMode() const;
	void setAxesCalcMode( AxesCalcMode mode );

    protected:
        void paintEvent ( QPaintEvent* );
        void resizeEvent ( QResizeEvent* );

        void layoutDiagrams();

    protected slots:
        void slotLayoutChanged( AbstractDiagram* );

    private:
        void paintGrid( PaintContext* );
        QRectF calculateDataBoundingRect() const;
    };

}

#endif
