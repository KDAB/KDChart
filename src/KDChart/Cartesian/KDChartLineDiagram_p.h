/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDCHARTLINEDIAGRAM_P_H
#define KDCHARTLINEDIAGRAM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartLineDiagram.h"

#include <QPainterPath>

#include "KDChartThreeDLineAttributes.h"
#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartCartesianDiagramDataCompressor_p.h"

#include <KDABLibFakes>


namespace KDChart {

    class PaintContext;

/**
 * \internal
 */
    class LineDiagram::Private : public AbstractCartesianDiagram::Private
    {
        friend class LineDiagram;
        friend class LineDiagramType;

    public:
        Private();
        Private( const Private& rhs );
        ~Private();

        LineDiagram* diagram;
        LineDiagramType* implementor; // the current type
        LineDiagramType* normalDiagram;
        LineDiagramType* stackedDiagram;
        LineDiagramType* percentDiagram;
        bool centerDataPoints;
        bool reverseDatasetOrder;
    };

    KDCHART_IMPL_DERIVED_DIAGRAM( LineDiagram, AbstractCartesianDiagram, CartesianCoordinatePlane )

    class LineDiagram::LineDiagramType
    {
    public:
        explicit LineDiagramType( LineDiagram* d )
            : m_private( d->d_func() )
        {
        }
        virtual ~LineDiagramType() {}
        virtual LineDiagram::LineType type() const = 0;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const = 0;
        virtual void paint( PaintContext* ctx ) = 0;
        LineDiagram* diagram() const;

    protected:
        // make some elements of m_private available to derived classes:
        AttributesModel* attributesModel() const;
        QModelIndex attributesModelRootIndex() const;
        ReverseMapper& reverseMapper();
        CartesianDiagramDataCompressor& compressor() const;

        qreal interpolateMissingValue( const CartesianDiagramDataCompressor::CachePosition& pos ) const;

        int datasetDimension() const;

		void paintAreas( PaintContext* ctx, const QModelIndex& index,
                         const QList<QPolygonF>& areas, const uint transparency );
        qreal valueForCell( int row, int column ) const;
        void addLabel( LabelPaintCache* lpc, const AbstractDiagram* diagram, const QModelIndex& index,
                       const CartesianDiagramDataCompressor::CachePosition* position,
                       const PositionPoints& points, const Position& autoPositionPositive,
                       const Position& autoPositionNegative, qreal value );


        const QPointF project( QPointF point, QPointF maxLimits,
                               qreal z, const QModelIndex& index ) const;

        void paintThreeDLines(
            PaintContext* ctx, const QModelIndex& index,
            const QPointF& from, const QPointF& to, const qreal depth );

        void paintElements( PaintContext* ctx,
                            const LabelPaintCache&,
                            LineAttributesInfoList&,
                            LineAttributes::MissingValuesPolicy );

        LineDiagram::Private* m_private;
    };
}

#endif /* KDCHARTLINEDIAGRAM_P_H */
