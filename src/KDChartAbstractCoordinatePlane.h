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

        virtual void addDiagram ( AbstractDiagram* diagram );
        virtual void replaceDiagram ( AbstractDiagram* diagram, int position = 0 );
        virtual void removeDiagram( int position = 0 );

        AbstractDiagram* diagram();
        AbstractDiagramList diagrams();

        virtual void layoutDiagrams() = 0;

        virtual const QPointF translate ( const QPointF& diagramPoint ) const = 0;

        virtual QSize sizeHint() const;
        virtual QSize minimumSizeHint() const;
        virtual QSizePolicy sizePolicy() const;

        virtual double zoomFactorX() const { return 1.0; }
        virtual double zoomFactorY() const { return 1.0; }

        virtual void setZoomFactorX( double /* factor */ ) {}
        virtual void setZoomFactorY( double /* factor */ ) {}

        virtual QPointF zoomCenter() const { return QPointF(0.0, 0.0); }

        virtual void setZoomCenter( QPointF /* center */ ) {}

        void setGridAttributes( const GridAttributes & );
        GridAttributes gridAttributes() const;

        void setReferenceCoordinatePlane( AbstractCoordinatePlane * plane );
        AbstractCoordinatePlane * referenceCoordinatePlane() const;
    signals:
        void destroyedCoordinatePlane( AbstractCoordinatePlane* );
        void diagramsChanged();
    private:
        Private* d;
    };

}

#endif
