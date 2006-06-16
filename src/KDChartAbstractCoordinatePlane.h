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

#ifndef KDCHARTCOORDINATEPLANE_H
#define KDCHARTCOORDINATEPLANE_H

#include <QWidget>

#include "KDChartAbstractDiagram.h"

namespace KDChart {
    class GridAttributes;

    class KDCHART_EXPORT AbstractCoordinatePlane : public QWidget
    {
        Q_OBJECT

    public:
        class Private;

        explicit AbstractCoordinatePlane ( QWidget* parent = 0 );
        virtual ~AbstractCoordinatePlane();

        /**
         * Adds a diagram to this coordinate plane.
         * @param diagram The diagram to add.
         */
        virtual void addDiagram ( AbstractDiagram* diagram );

        /**
         * Replaces the diagram at the given position or appends, it there is no
         * diagram at that position yet.
         * @param diagram The diagram to put at the given position.
         * @param position The position to put the new diagram at.
         */
        virtual void replaceDiagram ( AbstractDiagram* diagram, int position = 0 );

        /**
         * Remove the diagram at the given position, if there is one.
         * @param position The position at which to remove the diagram.
         */
        virtual void removeDiagram( int position = 0 );

        /**
         * @return The first diagram associated with this coordinate plane.
         */
        AbstractDiagram* diagram();

        /**
         * @return The list of diagrams associated with this coordinate plane.
         */
        AbstractDiagramList diagrams();

        /**
         * Distribute the available space among the diagrams and axes.
         */
        virtual void layoutDiagrams() = 0;

        /**
         * Translate the given point in value space coordinates to a position
         * in pixel space.
         * @param diagramPoint The point in value coordinates.
         * @returns The translated point.
         */
        virtual const QPointF translate ( const QPointF& diagramPoint ) const = 0;

        /** \reimp */
        virtual QSize sizeHint() const;
        /** \reimp */
        virtual QSize minimumSizeHint() const;
        /** \reimp */
        virtual QSizePolicy sizePolicy() const;

        /**
         * @return The zoom factor in horizontal diretion that is applied
         * to all coordinate transformations.
         */
        virtual double zoomFactorX() const { return 1.0; }

        /**
         * @return The zoom factor in vertical direction that is applied
         * to all coordinate transformations.
         */
        virtual double zoomFactorY() const { return 1.0; }

        /**
         * Sets the zoom factor in horizontal direction that is applied
         * to all coordinate transformations.
         */
        virtual void setZoomFactorX( double /* factor */ ) {}

        /**
         * Sets the zoom factor in vertical direction that is applied
         * to all coordinate transformations.
         */
        virtual void setZoomFactorY( double /* factor */ ) {}

        /**
         * @return The center point (in value coordinates) of the
         * coordinate plane that is used for zoom operations.
         */
        virtual QPointF zoomCenter() const { return QPointF(0.0, 0.0); }

        /**
         * Set the point (in value coordinates) to be used as the
         * center point in zoom operations.
         * @param center The point to use.
         */
        virtual void setZoomCenter( QPointF /* center */ ) {}

        /**
         * Set the grid attributes to be used by this coordinate plane.
         * To disable grid painting, for example, your code should like this:
         * \code
         * GridAttributes ga = plane->gridAttributes();
         * ga.setGridVisible( false );
         * plane-setGridAttributes( ga );
         * \endcode
         */
        void setGridAttributes( const GridAttributes & );

        /**
         * @return The grid attributes used by this coordinate plane.
         */
        GridAttributes gridAttributes() const;

        /**
         * Set another coordinate plane to be used as the reference plane
         * for this one.
         * @param plane The coordinate plane to be used the reference plane
         * for this one.
         * @see referenceCoordinatePlane
         */
        void setReferenceCoordinatePlane( AbstractCoordinatePlane * plane );

        /**
         * There are two ways in which planes can be caused to interact in
         * where they are put layouting wise: The first is the reference plane. If
         * such a reference plane is set, on a plane, it will use the same cell in the
         * layout as that one. In addition to this, planes can share an axis. In that case
         * they will be layed out in relation to each other as suggested by the position
         * of the axis. If, for example Plane1 and Plane2 share an axis at position Left,
         * that will result in the layout: Axis Plane1 Plane 2, vertically. If Plane1
         * also happens to be Plane2's referece plane, both planes are drawn over each
         * other. The reference plane concept allows two planes to share the same space
         * even if neither has any axis, and in case there are shared axis, it is used
         * to decided, whether the planes should be painted on top of each other or
         * layed out vertically or horizontally next to each other.
         * @return The reference coordinate plane associated with this one.
         */
        AbstractCoordinatePlane * referenceCoordinatePlane() const;
    signals:
        /** Emitted when this coordinate plane is destroyed. */
        void destroyedCoordinatePlane( AbstractCoordinatePlane* );

        /** Emitted when the associated diagrams change. */
        void diagramsChanged();
    private:
        Private* d;
    };

}

#endif
