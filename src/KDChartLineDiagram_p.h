/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
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

        void paintPolyline(
            PaintContext* ctx,
            const QBrush& brush, const QPen& pen,
            const QPolygonF& points ) const;

        LineDiagram* diagram;
        LineDiagramType* implementor; // the current type
        LineDiagramType* normalDiagram;
        LineDiagramType* stackedDiagram;
        LineDiagramType* percentDiagram;
        bool centerDataPoints;
        bool reverseDatasetOrder;
    };

    KDCHART_IMPL_DERIVED_DIAGRAM( LineDiagram, AbstractCartesianDiagram, CartesianCoordinatePlane )

    // we inherit privately, so that derived classes cannot call the
    // base class functions - those reference the wrong (unattached to
    // a diagram) d
    class LineDiagram::LineDiagramType : private LineDiagram::Private
    {
    public:
        explicit LineDiagramType( LineDiagram* d )
            : LineDiagram::Private()
            , m_private( d->d_func() )
        {
        }
        virtual ~LineDiagramType() {}
        virtual LineDiagram::LineType type() const = 0;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const = 0;
        virtual void paint(  PaintContext* ctx ) = 0;
        LineDiagram* diagram() const;

    protected:
        // method that make elements of m_private available to derived
        // classes:
        AttributesModel* attributesModel() const;
        QModelIndex attributesModelRootIndex() const;
        ReverseMapper& reverseMapper();
        CartesianDiagramDataCompressor& compressor() const;

        double interpolateMissingValue( const CartesianDiagramDataCompressor::CachePosition& pos ) const;

        int datasetDimension() const;
        LineAttributes::MissingValuesPolicy getCellValues(
            int row, int column,
            bool shiftCountedXValuesByHalfSection,
            double& valueX, double& valueY ) const;
        double valueForCellTesting( int row, int column,
                                    bool& bOK,
                                    bool showHiddenCellsAsInvalid = false ) const;
        void paintAreas( PaintContext* ctx, const QModelIndex& index,
                         const QList<QPolygonF>& areas, const uint transparency );
        double valueForCell( int row, int column );
        void appendDataValueTextInfoToList(
            AbstractDiagram * diagram,
            DataValueTextInfoList & list,
            const QModelIndex & index,
            const CartesianDiagramDataCompressor::CachePosition * position,
            const PositionPoints& points,
            const Position& autoPositionPositive,
            const Position& autoPositionNegative,
            const qreal value );


        const QPointF project( QPointF point, QPointF maxLimits,
                               double z, const QModelIndex& index ) const;

        void paintThreeDLines(
            PaintContext* ctx, const QModelIndex& index,
            const QPointF& from, const QPointF& to, const double depth  );

        void paintElements( PaintContext* ctx,
                            DataValueTextInfoList&,
                            LineAttributesInfoList&,
                            LineAttributes::MissingValuesPolicy );

        void paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt,
                                const QPointF& at );

        LineDiagram::Private* m_private;
    };

/*
  inline LineDiagram::LineDiagram( Private * p, CartesianCoordinatePlane* plane )
  : AbstractCartesianDiagram( p, plane ) { init(); }
  inline LineDiagram::Private * LineDiagram::d_func()
  { return static_cast<Private*>( AbstractCartesianDiagram::d_func() ); }
  inline const LineDiagram::Private * LineDiagram::d_func() const
  { return static_cast<const Private*>( AbstractCartesianDiagram::d_func() ); }
*/

}

#endif /* KDCHARTLINEDIAGRAM_P_H */
