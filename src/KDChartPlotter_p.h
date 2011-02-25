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
    class Plotter::Private : public AbstractCartesianDiagram::Private
    {
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
        bool useCompression;
        qreal mergeRadiusPercentage;
    protected:
        void init();
    };

    KDCHART_IMPL_DERIVED_DIAGRAM( Plotter, AbstractCartesianDiagram, CartesianCoordinatePlane )

    // we inherit privately, so that derived classes cannot call the
    // base class functions - those reference the wrong (unattached to
    // a diagram) d
    class Plotter::PlotterType : private Plotter::Private
    {
    public:
        explicit PlotterType( Plotter* d )
            : Plotter::Private()
            , m_private( d->d_func() )            
        {
        }
        virtual ~PlotterType() {}
        virtual Plotter::PlotType type() const = 0;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const = 0;
        virtual void paint(  PaintContext* ctx ) = 0;
        Plotter* diagram() const;

    protected:
        // method that make elements of m_private available to derived
        // classes:
        AttributesModel* attributesModel() const;
        QModelIndex attributesModelRootIndex() const;
        ReverseMapper& reverseMapper();
        CartesianDiagramDataCompressor& compressor() const;
        PlotterDiagramCompressor& plotterCompressor() const;
        bool useCompression() const;

        int datasetDimension() const;
/*        LineAttributes::MissingValuesPolicy getCellValues(
            int row, int column,
            bool shiftCountedXValuesByHalfSection,
            double& valueX, double& valueY ) const;
        double valueForCellTesting( int row, int column,
                                    bool& bOK,
                                    bool showHiddenCellsAsInvalid = false ) const;*/
        void paintAreas( PaintContext* ctx, const QModelIndex& index,
                         const QList<QPolygonF>& areas, const uint transparency );
/*        double valueForCell( int row, int column );*/
        void appendDataValueTextInfoToList(
            AbstractDiagram * diagram,
            DataValueTextInfoList & list,
            const QModelIndex & index,
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
