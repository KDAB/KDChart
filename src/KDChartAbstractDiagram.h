/****************************************************************************
 ** Copyright (C) 2006 KlarÃ¤lvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTABSTRACTDIAGRAM_H
#define KDCHARTABSTRACTDIAGRAM_H

#include <QList>
#include <QRectF>
#include <QAbstractItemView>

#include "KDChartGlobal.h"
#include "KDChartMarkerAttributes.h"


namespace KDChart {

    class AbstractCoordinatePlane;
    class DataValueAttributes;
    class PaintContext;
    class DatasetProxyModel;

    /**
     * @brief AbstractDiagram defines the interface for diagram classes
     *
     * AbstractDiagram is the base class for diagram classes ("chart types").
     * It defines the interface that needs to be implemented for the diagram
     * to function within the KDChart framework. It extends Interview's
     * QAbstractItemView.
     */
    class KDCHART_EXPORT AbstractDiagram : public QAbstractItemView
    {
        Q_OBJECT
        Q_DISABLE_COPY( AbstractDiagram )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( AbstractDiagram )
    protected:
        explicit inline AbstractDiagram( Private *p, AbstractCoordinatePlane* parent );
        explicit AbstractDiagram ( AbstractCoordinatePlane* plane = 0 );
    public:
        virtual ~AbstractDiagram();


        /*! Return the bottom left and top right data point the diagram will
         * display, in diagram coordinates.
         */
        virtual const QPair<QPointF, QPointF> dataBoundaries() const;


        /**
         * Draw the diagram contents to the rectangle and painter that are
         * passed in as part of the paint context.
         * @param paintContext All information needed for painting.
         */
        virtual void paint ( PaintContext* paintContext ) = 0;


        /**
         * Called by the widget's sizeEvent. Adjust all internal structures that
         * are sensitive to the size of the widget.
         * @param area
         */
        virtual void resize ( const QSizeF& area ) = 0;

        /** Associate a model with the diagram. */
        virtual void setModel ( QAbstractItemModel * model );

        virtual QAbstractItemModel* model() const;

        /**
         * Return the proxy model responsible for selecing the data to be
         * displayed by this diagram from the underlying real model, if there
         * is one.
         * @return The dataset proxy model associated with this diagram, or 0
         * if there is none.
         */
        virtual DatasetProxyModel* datasetProxy ();

                /**
         * Set whether the diagram should use its own attributes model, or share one
         * with all other diagrams showing the same data (using the same model).
         * Per default this is false, which means if two diagrams show the same model
         * data, and you set a certain pen or brush for a dataset, all diagrams will
         * use that pen or brush to paint the dataset.
         * @param privateAttributes Whether the diagram should use a private set of
         * attributes and not share them with others showing the same data.
                 */
        void usePrivateAttributes( bool privateAttributes );


       /** Set the root index in the model where the diagram starts
        * referencing data for display. */
        virtual void setRootIndex ( const QModelIndex& idx );

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

        /**
         * The coordinate plane associated with the diagram. This determines
         * how coordinates in value space are mapped into pixel space. By default
         * this is a CartesianCoordinatePlane.
         * @return The coordinate plane associated with the diagram.
         */
        AbstractCoordinatePlane* coordinatePlane() const;

        /**
         * Set the coordinate plane associated with the diagram. This determines
         * how coordinates in value space are mapped into pixel space. The chart
         * takes ownership.
         * @return The coordinate plane associated with the diagram.
         */
        void setCoordinatePlane( AbstractCoordinatePlane* plane );


        /**
         * The the DataValueAttributes for the given index.
         * @param index The datapoint to set the attributes for.
         * @param a The attributes to set.
         */
        void setDataValueAttributes( const QModelIndex & index,
                                     const DataValueAttributes & a );

        /**
         * The the DataValueAttributes for the given dataset.
         * @param dataset The dataset to set the attributes for.
         * @param a The attributes to set.
         */
        void setDataValueAttributes( int dataset, const DataValueAttributes & a );

        /**
         * The the DataValueAttributes for all datapoints in the model.
         * @param a The attributes to set.
         */
        void setDataValueAttributes( const DataValueAttributes & a );

        /**
         * Retrieve the DataValueAttributes for the given index. This will fall
         * back automatically to what was set at dataset or model level, if there
         * are no datapoint specific settings.
         * @param index The datapoint to retrive the attributes for.
         * @return The DataValueAttributes for the given index.
         */
        DataValueAttributes dataValueAttributes( const QModelIndex & index ) const;

        /**
         * Set the pen to be used for paiting the datapoint at the given index.
         * @param index The datapoint's index in the model.
         * @param pen The pen to use.
         */
        void setPen( const QModelIndex& index, const QPen& pen );

        /**
         * Set the pen to be used for paiting the given dataset.
         * @param dataset The dataset's row in the model.
         * @param pen The pen to use.
         */
        void setPen( int dataset, const QPen& pen );

        /**
         * Set the pen to be used for paiting all datasets in the model.
         * @param pen The pen to use.
         */
        void setPen( const QPen& pen );

        /**
         * Retrieve the pen to be used for painting the datapoint at the given
         * index in the model.
         * @param index The index of the datapoint in the model.
         * @return The pen to use for painting.
         */
        QPen pen( const QModelIndex& index ) const;

        /**
         * Set the brush to be used for paiting the datapoint at the given index.
         * @param index The datapoint's index in the model.
         * @param brush The brush to use.
         */
        void setBrush( const QModelIndex& index, const QBrush& brush);

        /**
         * Set the brush to be used for paiting the given dataset.
         * @param dataset The dataset's row in the model.
         * @param pen The brush to use.
         */
        void setBrush( int dataset, const QBrush& brush );

        /**
         * Set the brush to be used for paiting all datasets in the model.
         * @param brush The brush to use.
         */
        void setBrush( const QBrush& brush);

        /**
         * Retrieve the brush to be used for painting the datapoint at the given
         * index in the model.
         * @param index The index of the datapoint in the model.
         * @return The brush to use for painting.
         */
        QBrush brush( const QModelIndex& index ) const;

        void setAllowOverlappingDataValueTexts( bool allow );
        bool allowOverlappingDataValueTexts() const;

        /**
         * Set the palette to be used for painting datasets to the default
         * palette. @see KDChart::Palette.
         */
        void useDefaultColors();

        /**
         * Set the palette to be used for painting datasets to the rainbow
         * palette. @see KDChart::Palette.
         */
        void useRainbowColors();

        /**
         * Set the palette to be used for painting datasets to the subdued
         * palette. @see KDChart::Palette.
        */
        void useSubduedColors();

        /**
         * The set of dataset labels currently displayed, for use in legends, etc.
         * @return The set of dataset labels currently displayed.
         */
        QStringList datasetLabels() const;

        /**
         * The set of dataset brushes currently used, for use in legends, etc. This
         * ignores cell-level override brushes.
         * @return The current set of dataset brushes.
         */
        QList<QBrush> datasetBrushes() const;

        /**
         * The set of dataset pens currently used, for use in legends, etc. This
         * ignores cell-level override pens.
         * @return The current set of dataset pens.
         */
        QList<QPen> datasetPens() const;

        /**
         * The set of dataset markers currently used, for use in legends, etc. This
         * ignores cell-level override markers.
         * @return The current set of dataset brushes.
         */
        QList<MarkerAttributes> datasetMarkers() const;


        // configure the ordinate in percent mode - values 0 to 100
        void setPercentMode( bool percent );
        bool percentMode() const;


        // FIXME merge the rest from KDChartDiagram.h

        virtual void paintMarker( QPainter* painter, const MarkerAttributes& markerAttributes,
                                  const QBrush& brush, const QPen&, const QPointF& point, const QSizeF& size );

    protected:
        virtual bool checkInvariants() const;
        virtual void paintDataValueTexts( QPainter* painter );
        void paintDataValueText( QPainter* painter, const QModelIndex& index,
                                 const QPointF& pos, double value );
        void paintMarker(  QPainter* painter, const QModelIndex& index,
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
