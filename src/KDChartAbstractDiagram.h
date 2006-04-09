#ifndef KDCHARTABSTRACTDIAGRAM_H
#define KDCHARTABSTRACTDIAGRAM_H

#include <QList>
#include <QRectF>
#include <QAbstractItemView>

#include "KDChartGlobal.h"
#include "KDChartMarkerAttributes.h"

/**
 * AbstractDiagram defines the interface every diagram class 
 * has to implement.
 */
namespace KDChart {

    class CoordinatePlane;
    class DataValueAttributes;
    class ThreeDAttributes;
    class ThreeDLineAttributes;
    class PaintContext;
    class DatasetProxyModel;

    // This class is internal; but it's exported for qtests/KDChartAttributesModel
    class KDCHART_EXPORT AbstractDiagram : public QAbstractItemView
    {
        Q_OBJECT
        Q_DISABLE_COPY( AbstractDiagram )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( AbstractDiagram )
    protected:
        explicit inline AbstractDiagram( Private *p, CoordinatePlane* parent );
        explicit AbstractDiagram ( CoordinatePlane* plane = 0 );
    public:
        virtual ~AbstractDiagram();


        /*! Return the bottom left and top right data point the diagram will
         * display, in diagram coordinates.
         */
        virtual const QPair<QPointF, QPointF> dataBoundaries() const;
        // calculate values to paint and stores
        virtual void paint ( PaintContext* paintContext ) = 0;
        virtual void resize ( const QSizeF& area ) = 0;

        /** \reimpl */
        virtual void setModel ( QAbstractItemModel * model );

        // implement QAbstractItemView:
        /** \reimpl */
        virtual QRect visualRect(const QModelIndex &index) const;
        /** \reimpl */
        virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
        /** \reimpl */
        virtual QModelIndex indexAt(const QPoint &point) const;
        /** \reimpl */
        virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
        /** \reimpl */
        virtual int horizontalOffset() const;
        /** \reimpl */
        virtual int verticalOffset() const;
        /** \reimpl */
        virtual bool isIndexHidden(const QModelIndex &index) const;
        /** \reimpl */
        virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
        /** \reimpl */
        virtual QRegion visualRegionForSelection(const QItemSelection &selection) const;

        const CoordinatePlane* coordinatePlane() const;

        void setCoordinatePlane( CoordinatePlane* plane );


        // FIXME merge the rest from KDChartDiagram.h
        void setDataValueAttributes( const QModelIndex & index,
                                     const DataValueAttributes & a );
        KDChart::DataValueAttributes dataValueAttributes( const QModelIndex & index ) const;

        void setDataValueAttributes( int column, const DataValueAttributes & a );
        KDChart::DataValueAttributes dataValueAttributes( int column ) const;

        void setAllDataValueAttributes( const DataValueAttributes & a );
        KDChart::DataValueAttributes allDataValueAttributes() const;

        //Pen
        void setPen( const QModelIndex& index, const QPen& pen );
        void setPen( const QPen& pen );
        void setPen( int column, const QPen& pen );
        QPen pen( const QModelIndex& index ) const; 

        void setBrush( const QModelIndex& index, const QBrush& brush);
        void setBrush( const QBrush& brush);
        void setBrush( int column, const QBrush& brush );
        QBrush brush( const QModelIndex& index ) const;

        void setAllowOverlappingDataValueTexts( bool allow );
        bool allowOverlappingDataValueTexts() const;

        void useDefaultColors();
        void useRainbowColors();
        void useSubduedColors();

        void usePrivateAttributes( bool privateAttributes );

        virtual DatasetProxyModel* datasetProxy ();

        QStringList datasetLabels() const;
        QList<QBrush> datasetBrushes() const;
        QList<MarkerAttributes> datasetMarkers() const;

    protected:
        virtual bool checkInvariants() const;
        virtual void paintDataValueTexts( QPainter* painter );
        void paintDataValueText( QPainter* painter, const QModelIndex& index,
                                 const QPointF& pos, double value );
        void paintMarkers(  QPainter* painter, const QModelIndex& index,
                            const QPointF& pos );
        virtual void paintMarkers( QPainter* painter );

    signals:
        /** Diagrams are supposed to emit this signal when the layout of one
            of their element changes. Layouts can change, for example, when
            axes are added or removed or the configuration changed in a way that
            the axes or the diagram itself are displayed in a different
            geometry. Changes in the diagrams coordinate system also require
            to emit layoutChanged(). */
        void layoutChanged( AbstractDiagram* );

    protected slots:
        void slotModelReset();
    };

    class AbstractDiagramList : public QList<AbstractDiagram*> {};

}

#endif
