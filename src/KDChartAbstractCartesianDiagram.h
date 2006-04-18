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

#ifndef KDCHARTABSTRACTCARTESIANDIAGRAM_H
#define KDCHARTABSTRACTCARTESIANDIAGRAM_H

#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartCartesianAxis.h"

namespace KDChart {

    class GridAttributes;
    class PaintContext;

    class KDCHART_EXPORT AbstractCartesianDiagram : public AbstractDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( AbstractCartesianDiagram )
        KDCHART_DECLARE_PRIVATE_DERIVED( AbstractCartesianDiagram )
    protected:
        explicit inline AbstractCartesianDiagram( Private *p, CartesianCoordinatePlane* parent );

	virtual void drawGrid( PaintContext* context );

    public:
        explicit AbstractCartesianDiagram ( CartesianCoordinatePlane* parent = 0 );
        virtual ~AbstractCartesianDiagram() {}

        virtual const int numberOfAbscissaSegments () const = 0;
        virtual const int numberOfOrdinateSegments () const = 0;
        virtual void addAxes( CartesianAxis * axis );
        virtual KDChart::CartesianAxisList axes () const;

        virtual void setReferenceDiagram( AbstractCartesianDiagram* diagarm, const QPointF& offset = QPointF() );
        virtual AbstractCartesianDiagram* referenceDiagram() const;
        virtual QPointF referenceDiagramOffset() const;

        virtual GridAttributes* gridAttributes();
        virtual const GridAttributes* gridAttributes() const;
    };

}

#endif
