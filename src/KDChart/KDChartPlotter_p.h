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

#ifndef KDCHARTPLOTTER_P_H
#define KDCHARTPLOTTER_P_H

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

#include "KDChartPlotter.h"

#include <QPainterPath>

#include "KDChartThreeDLineAttributes.h"
#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartCartesianDiagramDataCompressor_p.h"
#include "KDChartPlotterDiagramCompressor.h"

#include <KDABLibFakes>


namespace KDChart {

    class PaintContext;

/**
 * \internal
 */
    class Plotter::Private : public QObject, public AbstractCartesianDiagram::Private
    {
        Q_OBJECT
        friend class Plotter;
        friend class PlotterType;

    public:
        Private();
        Private( const Private& rhs );
        ~Private();

        void setCompressorResolution(
            const QSizeF& size,
            const AbstractCoordinatePlane* plane );
        void paintPolyline(
            PaintContext* ctx,
            const QBrush& brush, const QPen& pen,
            const QPolygonF& points ) const;

        Plotter* diagram;
        PlotterType* implementor; // the current type
        PlotterType* normalPlotter;
        PlotterType* percentPlotter;
        PlotterDiagramCompressor plotterCompressor;
        Plotter::CompressionMode useCompression;
        qreal mergeRadiusPercentage;
    protected:
        void init();
    public Q_SLOTS:
        void changedProperties();
    };

    KDCHART_IMPL_DERIVED_DIAGRAM( Plotter, AbstractCartesianDiagram, CartesianCoordinatePlane )

    class Plotter::PlotterType
    {
    public:
        explicit PlotterType( Plotter* d )
            : m_private( d->d_func() )
        {
            m_private->init();
        }
        virtual ~PlotterType() {}
        virtual Plotter::PlotType type() const = 0;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const = 0;
        virtual void paint( PaintContext* ctx ) = 0;
        Plotter* diagram() const;
        Plotter::CompressionMode useCompression() const;
        void setUseCompression( Plotter::CompressionMode value );
        PlotterDiagramCompressor& plotterCompressor() const;

        Plotter::Private* plotterPrivate() const { return m_private; }

    protected:
        // method that make elements of m_private available to derived
        // classes:
        AttributesModel* attributesModel() const;
        QModelIndex attributesModelRootIndex() const;
        ReverseMapper& reverseMapper();
        CartesianDiagramDataCompressor& compressor() const;        

        int datasetDimension() const;

        void paintAreas( PaintContext* ctx, const QModelIndex& index,
                         const QList<QPolygonF>& areas, uint opacity );

        void addLabel( LabelPaintCache* lpc, const AbstractDiagram* diagram, const QModelIndex& index,
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

        void paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt,
                                const QPointF& at );

        Plotter::Private* m_private;
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
