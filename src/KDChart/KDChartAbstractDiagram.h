/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTDIAGRAM_H
#define KDCHARTABSTRACTDIAGRAM_H

#include <QAbstractItemView>
#include <QList>
#include <QRectF>

#include "KDChartAttributesModel.h"
#include "KDChartGlobal.h"
#include "KDChartMarkerAttributes.h"

namespace KDChart {

class AbstractCoordinatePlane;
class AttributesModel;
class DataValueAttributes;
class PaintContext;

/**
 * @brief AbstractDiagram defines the interface for diagram classes
 *
 * AbstractDiagram is the base class for diagram classes ("chart types").
 *
 * It defines the interface, that needs to be implemented for the diagram,
 * to function within the KDChart framework. It extends Interview's
 * QAbstractItemView.
 */
class KDCHART_EXPORT AbstractDiagram : public QAbstractItemView
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractDiagram)
    KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC(AbstractDiagram)

    friend class AbstractCoordinatePlane;
    friend class CartesianCoordinatePlane;
    friend class PolarCoordinatePlane;

protected:
    explicit inline AbstractDiagram(
        Private *p, QWidget *parent, AbstractCoordinatePlane *plane);
    explicit AbstractDiagram(
        QWidget *parent = nullptr, AbstractCoordinatePlane *plane = nullptr);

public:
    ~AbstractDiagram() override;

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare(const AbstractDiagram *other) const;

    /**
     * @brief Return the bottom left and top right data point, that the
     * diagram will display (unless the grid adjusts these values).
     *
     * This method returns a cached result of calculations done by
     * calculateDataBoundaries.
     * Classes derived from AbstractDiagram must implement the
     * calculateDataBoundaries function, to specify their own
     * way of calculating the data boundaries.
     * If derived classes want to force recalculation of the
     * data boundaries, they can call setDataBoundariesDirty()
     *
     * Returned value is in diagram coordinates.
     */
    const QPair<QPointF, QPointF> dataBoundaries() const;

    // protected: // FIXME: why should that be private? (Mirko)
    /**
     * Draw the diagram contents to the rectangle and painter, that are
     * passed in as part of the paint context.
     *
     * @param paintContext All information needed for painting.
     */
    virtual void paint(PaintContext *paintContext) = 0;

    /**
     * Called by the widget's sizeEvent. Adjust all internal structures,
     * that are calculated, dependending on the size of the widget.
     *
     * @param area Size of the widget
     */
    virtual void resize(const QSizeF &area) = 0;

    /** Associate a model with the diagram. */
    void setModel(QAbstractItemModel *model) override;

    /** Associate a seleection model with the diagrom. */
    void setSelectionModel(QItemSelectionModel *selectionModel) override;

    /**
     * Associate an AttributesModel with this diagram. Note that
     * the diagram does _not_ take ownership of the AttributesModel.
     * This should thus only be used with AttributesModels that
     * have been explicitly created by the user, and are owned
     * by her. Setting an AttributesModel that is internal to
     * another diagram is an error.
     *
     * Correct:
     *
     * \code
     * AttributesModel *am = new AttributesModel( model, 0 );
     * diagram1->setAttributesModel( am );
     * diagram2->setAttributesModel( am );
     *
     * \endcode
     *
     * Wrong:
     *
     * \code
     *
     * diagram1->setAttributesModel( diagram2->attributesModel() );
     *
     * \endcode
     *
     * @param model The AttributesModel to use for this diagram.
     * @see AttributesModel, usesExternalAttributesModel
     */
    virtual void setAttributesModel(AttributesModel *model);

    /**
     * Returns whether the diagram is using its own built-in attributes model
     * or an attributes model that was set via setAttributesModel.
     *
     * @see setAttributesModel
     */
    virtual bool usesExternalAttributesModel() const;

    /**
     * Returns the AttributesModel, that is used by this diagram.
     * By default each diagram owns its own AttributesModel, which
     * should never be deleted. Only if a user-supplied AttributesModel
     * has been set does the pointer returned here not belong to the
     * diagram.
     *
     * @return The AttributesModel associated with the diagram.
     * @see setAttributesModel
     */
    virtual AttributesModel *attributesModel() const;

    /** Set the root index in the model, where the diagram starts
     * referencing data for display. */
    void setRootIndex(const QModelIndex &idx) override;

    /** \reimp */
    QRect visualRect(const QModelIndex &index) const override;
    /** \reimp */
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
    /** \reimp */
    QModelIndex indexAt(const QPoint &point) const override;
    /** \reimp */
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
    /** \reimp */
    int horizontalOffset() const override;
    /** \reimp */
    int verticalOffset() const override;
    /** \reimp */
    bool isIndexHidden(const QModelIndex &index) const override;
    /** \reimp */
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    /** \reimp */
    QRegion visualRegionForSelection(const QItemSelection &selection) const override;
    virtual QRegion visualRegion(const QModelIndex &index) const;
    /** \reimp */
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;
    /** \reimp */
    void doItemsLayout() override;

    /**
     * The coordinate plane associated with the diagram. This determines
     * how coordinates in value space are mapped into pixel space. By default
     * this is a CartesianCoordinatePlane.
     * @return The coordinate plane associated with the diagram.
     */
    AbstractCoordinatePlane *coordinatePlane() const;

    /**
     * Set the coordinate plane associated with the diagram. This determines
     * how coordinates in value space are mapped into pixel space. The chart
     * takes ownership.
     */
    virtual void setCoordinatePlane(AbstractCoordinatePlane *plane);

    /**
     * Hide (or unhide, resp.) a data cell.
     *
     * \note Hidden data are still taken into account by the coordinate plane,
     * so neither the grid nor your axes' ranges will change, when you hide data.
     * For totally removing data from KD Chart's view you can use another approach:
     * e.g. you could define a proxy model on top of your data model, and register
     * the proxy model calling setModel() instead of registering your real data model.
     *
     * @param index The datapoint to set the hidden status for. With a dataset dimension
     * of two, this is the index of the key of each key/value pair.
     * @param hidden The hidden status to set.
     */
    void setHidden(const QModelIndex &index, bool hidden);

    /**
     * Hide (or unhide, resp.) a dataset.
     *
     * \note Hidden data are still taken into account by the coordinate plane,
     * so neither the grid nor your axes' ranges will change, when you hide data.
     * For totally removing data from KD Chart's view you can use another approach:
     * e.g. you could define a proxy model on top of your data model, and register
     * the proxy model calling setModel() instead of registering your real data model.
     *
     * @param dataset The dataset to set the hidden status for.
     * @param hidden The hidden status to set.
     */
    void setHidden(int dataset, bool hidden);

    /**
     * Hide (or unhide, resp.) all datapoints in the model.
     *
     * \note Hidden data are still taken into account by the coordinate plane,
     * so neither the grid nor your axes' ranges will change, when you hide data.
     * For totally removing data from KD Chart's view you can use another approach:
     * e.g. you could define a proxy model on top of your data model, and register
     * the proxy model calling setModel() instead of registering your real data model.
     *
     * @param hidden The hidden status to set.
     */
    void setHidden(bool hidden);

    /**
     * Retrieve the hidden status specified globally. This will fall
     * back automatically to the default settings ( = not hidden), if there
     * are no specific settings.
     * @return The global hidden status.
     */
    bool isHidden() const;

    /**
     * Retrieve the hidden status for the given dataset. This will fall
     * back automatically to what was set at diagram level, if there
     * are no dataset specific settings.
     * @param dataset The dataset to retrieve the hidden status for.
     * @return The hidden status for the given dataset.
     */
    bool isHidden(int dataset) const;

    /**
     * Retrieve the hidden status for the given index. This will fall
     * back automatically to what was set at dataset or diagram level, if there
     * are no datapoint specific settings.
     * @param index The datapoint to retrieve the hidden status for.
     * @return The hidden status for the given index.
     */
    bool isHidden(const QModelIndex &index) const;

    /**
     * Set the DataValueAttributes for the given index.
     * @param index The datapoint to set the attributes for. With a dataset dimension
     * of two, this is the index of the key of each key/value pair.
     * @param a The attributes to set.
     */
    void setDataValueAttributes(const QModelIndex &index,
                                const DataValueAttributes &a);

    /**
     * Set the DataValueAttributes for the given dataset.
     * @param dataset The dataset to set the attributes for.
     * @param a The attributes to set.
     */
    void setDataValueAttributes(int dataset, const DataValueAttributes &a);

    /**
     * Set the DataValueAttributes for all datapoints in the model.
     * @param a The attributes to set.
     */
    void setDataValueAttributes(const DataValueAttributes &a);

    /**
     * Retrieve the DataValueAttributes specified globally. This will fall
     * back automatically to the default settings, if there
     * are no specific settings.
     * @return The global DataValueAttributes.
     */
    DataValueAttributes dataValueAttributes() const;

    /**
     * Retrieve the DataValueAttributes for the given dataset. This will fall
     * back automatically to what was set at model level, if there
     * are no dataset specific settings.
     * @param dataset The dataset to retrieve the attributes for.
     * @return The DataValueAttributes for the given dataset.
     */
    DataValueAttributes dataValueAttributes(int dataset) const;

    /**
     * Retrieve the DataValueAttributes for the given index. This will fall
     * back automatically to what was set at dataset or model level, if there
     * are no datapoint specific settings.
     * @param index The datapoint to retrieve the attributes for. With a dataset dimension
     * of two, this is the index of the key of each key/value pair.
     * @return The DataValueAttributes for the given index.
     */
    DataValueAttributes dataValueAttributes(const QModelIndex &index) const;

    /**
     * Set the pen to be used, for painting the datapoint at the given index.
     * @param index The datapoint's index in the model. With a dataset dimension
     * of two, this is the index of the key of each key/value pair.
     * @param pen The pen to use.
     */
    void setPen(const QModelIndex &index, const QPen &pen);

    /**
     * Set the pen to be used, for painting the given dataset.
     * @param dataset The dataset to set the pen for.
     * @param pen The pen to use.
     */
    void setPen(int dataset, const QPen &pen);

    /**
     * Set the pen to be used, for painting all datasets in the model.
     * @param pen The pen to use.
     */
    void setPen(const QPen &pen);

    /**
     * Retrieve the pen to be used for painting datapoints globally. This will fall
     * back automatically to the default settings, if there
     * are no specific settings.
     * @return The pen to use for painting.
     */
    QPen pen() const;
    /**
     * Retrieve the pen to be used for the given dataset. This will fall
     * back automatically to what was set at model level, if there
     * are no dataset specific settings.
     * @param dataset The dataset to retrieve the pen for.
     * @return The pen to use for painting.
     */
    QPen pen(int dataset) const;
    /**
     * Retrieve the pen to be used, for painting the datapoint at the given
     * index in the model.
     * @param index The index of the datapoint in the model. With a dataset dimension
     * of two, this is the index of the key of each key/value pair.
     * @return The pen to use for painting.
     */
    QPen pen(const QModelIndex &index) const;

    /**
     * Set the brush to be used, for painting the datapoint at the given index.
     * @param index The datapoint's index in the model. With a dataset dimension
     * of two, this is the index of the key of each key/value pair.
     * @param brush The brush to use.
     */
    void setBrush(const QModelIndex &index, const QBrush &brush);

    /**
     * Set the brush to be used, for painting the given dataset.
     * @param dataset The dataset to set the brush for.
     * @param brush The brush to use.
     */
    void setBrush(int dataset, const QBrush &brush);

    /**
     * Set the brush to be used, for painting all datasets in the model.
     * @param brush The brush to use.
     */
    void setBrush(const QBrush &brush);

    /**
     * Retrieve the brush to be used for painting datapoints globally. This will fall
     * back automatically to the default settings, if there
     * are no specific settings.
     * @return The brush to use for painting.
     */
    QBrush brush() const;
    /**
     * Retrieve the brush to be used for the given dataset. This will fall
     * back automatically to what was set at model level, if there
     * are no dataset specific settings.
     * @param dataset The dataset to retrieve the brush for.
     * @return The brush to use for painting.
     */
    QBrush brush(int dataset) const;
    /**
     * Retrieve the brush to be used, for painting the datapoint at the given
     * index in the model.
     * @param index The index of the datapoint in the model. With a dataset dimension
     * of two, this is the index of the key of each key/value pair.
     * @return The brush to use for painting.
     */
    QBrush brush(const QModelIndex &index) const;

    /**
     * Set the unit prefix to be used on axes for one specific column.
     * @param prefix The prefix to be used.
     * @param column The column which should be set.
     * @param orientation The orientation of the axis to use.
     */
    void setUnitPrefix(const QString &prefix, int column, Qt::Orientation orientation);
    /**
     * Set the unit prefix to be used on axes for all columns.
     * @param prefix The prefix to be used.
     * @param orientation The orientation of the axis to use.
     */
    void setUnitPrefix(const QString &prefix, Qt::Orientation orientation);

    /**
     * Set the unit prefix to be used on axes for one specific column.
     * @param suffix The suffix to be used.
     * @param column The column which should be set.
     * @param orientation The orientation of the axis to use.
     */
    void setUnitSuffix(const QString &suffix, int column, Qt::Orientation orientation);
    /**
     * Set the unit prefix to be used on axes for all columns.
     * @param suffix The suffix to be used.
     * @param orientation The orientation of the axis to use.
     */
    void setUnitSuffix(const QString &suffix, Qt::Orientation orientation);

    /**
     * Retrieves the axis unit prefix for a specific column.
     * @param column The column whose prefix should be retrieved.
     * @param orientation The orientation of the axis.
     * @param fallback If true, the prefix for all columns is returned, when
     *                 none is set for the selected column.
     * @return The axis unit prefix.
     */
    QString unitPrefix(int column, Qt::Orientation orientation, bool fallback = false) const;
    /**
     * Retrieves the axis unit prefix.
     * @param orientation The orientation of the axis.
     * @return The axis unit prefix.
     */
    QString unitPrefix(Qt::Orientation orientation) const;

    /**
     * Retrieves the axis unit suffix for a specific column.
     * @param column The column whose prefix should be retrieved.
     * @param orientation The orientation of the axis.
     * @param fallback If true, the suffix for all columns is returned, when
     *                 none is set for the selected column.
     * @return The axis unit suffix.
     */
    QString unitSuffix(int column, Qt::Orientation orientation, bool fallback = false) const;
    /**
     * Retrieves the axis unit suffix.
     * @param orientation The orientation of the axis.
     * @return The axis unit suffix.
     */
    QString unitSuffix(Qt::Orientation orientation) const;

    /**
     * Set whether data value labels are allowed to overlap.
     * @param allow True means that overlapping labels are allowed.
     */
    void setAllowOverlappingDataValueTexts(bool allow);

    /**
     * @return Whether data value labels are allowed to overlap.
     */
    bool allowOverlappingDataValueTexts() const;

    /**
     * Set whether anti-aliasing is to be used while rendering
     * this diagram.
     * @param enabled True means that AA is enabled.
     */
    void setAntiAliasing(bool enabled);

    /**
     * @return Whether anti-aliasing is to be used for rendering
     * this diagram.
     */
    bool antiAliasing() const;

    /**
     * Set the palette to be used, for painting datasets to the default
     * palette.
     * @see KDChart::Palette.
     * FIXME: fold into one usePalette (KDChart::Palette&) method
     */
    void useDefaultColors();

    /**
     * Set the palette to be used, for painting datasets to the rainbow
     * palette.
     * @see KDChart::Palette.
     */
    void useRainbowColors();

    /**
     * Set the palette to be used, for painting datasets to the subdued
     * palette.
     * @see KDChart::Palette.
     */
    void useSubduedColors();

    /**
     * The set of item row labels currently displayed, for use in Abscissa axes, etc.
     * @return The set of item row labels currently displayed.
     */
    QStringList itemRowLabels() const;

    /**
     * The set of dataset labels currently displayed, for use in legends, etc.
     * @return The set of dataset labels currently displayed.
     */
    QStringList datasetLabels() const;

    /**
     * The set of dataset brushes currently used, for use in legends, etc.
     *
     * @note Cell-level override brushes, if set, take precedence over the
     * dataset values, so you might need to check these too, in order to find
     * the brush, that is used for a single cell.
     *
     * @return The current set of dataset brushes.
     */
    QList<QBrush> datasetBrushes() const;

    /**
     * The set of dataset pens currently used, for use in legends, etc.
     *
     * @note Cell-level override pens, if set, take precedence over the
     * dataset values, so you might need to check these too, in order to find
     * the pens, that is used for a single cell.
     *
     * @return The current set of dataset pens.
     */
    QList<QPen> datasetPens() const;

    /**
     * The set of dataset markers currently used, for use in legends, etc.
     *
     * @note Cell-level override markers, if set, take precedence over the
     * dataset values, so you might need to check these too, in order to find
     * the marker, that is shown for a single cell.
     *
     * @return The current set of dataset brushes.
     */
    QList<MarkerAttributes> datasetMarkers() const;

    /**
     * \deprecated
     *
     * \brief Deprecated method that turns the percent mode of this diagram on or off.
     *
     * This method is deprecated. Use the setType() method of a supporting diagram implementation
     * instead, e.g. BarDiagram::setType().
     *
     * \see percentMode
     */
    Q_DECL_DEPRECATED void setPercentMode(bool percent);

    /**
     * \brief Returns whether this diagram is drawn in percent mode.
     *
     * If true, all data points in the same column of a diagram will
     * be drawn at the same X coordinate and stacked up so that the distance from the
     * last data point (or the zero line) to a data point P is always the ratio of (Y-Value of P)/
     * (sum of all Y-Values in same column as P) relative to the diagrams height
     * (or width, if abscissa and ordinate are swapped).
     *
     * Note that this property is not applicable to all diagram types.
     */
    bool percentMode() const;

    virtual void paintMarker(QPainter *painter,
                             const MarkerAttributes &markerAttributes,
                             const QBrush &brush, const QPen &,
                             const QPointF &point, const QSizeF &size);

    /**
     * The dataset dimension of a diagram determines how many value dimensions
     * it expects each datapoint to have.
     * For each dimension and data series it will expect one column of values in the model.
     * If the dimension is 1, automatic values will be used for X.
     *
     * For example, a diagram with the default dimension of 1 will have one column
     * per data series (the Y values) and will use automatic values for X
     * (1, 2, 3, ... n).
     * If the dimension is 2, the diagram will use the first, (and the third,
     * fifth, etc) columns as X values, and the second, (and the fourth, sixth,
     * etc) column as Y values.
     * @return The dataset dimension of the diagram.
     */
    int datasetDimension() const;

    /**
     * \deprecated
     *
     * Sets the dataset dimension of the diagram. Using this method
     * is deprecated. Use the specific diagram types instead.
     */
    Q_DECL_DEPRECATED void setDatasetDimension(int dimension);

protected:
    void setDatasetDimensionInternal(int dimension);

public:
    void update() const;

    void paintMarker(QPainter *painter, const DataValueAttributes &a,
                     const QModelIndex &index,
                     const QPointF &pos);
    void paintMarker(QPainter *painter,
                     const QModelIndex &index,
                     const QPointF &pos);
    void paintDataValueText(QPainter *painter, const QModelIndex &index,
                            const QPointF &pos, qreal value);

    // reverse mapping:
    /** This method is added alongside with indexAt from QAIM,
        since in kdchart multiple indexes can be displayed at the same
        spot. */
    QModelIndexList indexesAt(const QPoint &point) const;
    QModelIndexList indexesIn(const QRect &rect) const;

protected:
    virtual bool checkInvariants(bool justReturnTheStatus = false) const;
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const = 0;

protected Q_SLOTS:
    void setDataBoundariesDirty() const;

protected:
    /**
     * \deprecated
     * This method is deprecated and provided for backward-compatibility only.
     * Your own diagram classes should call
     * d->paintDataValueTextsAndMarkers() instead
     * which also is taking care for showing your cell-specific comments, if any,
     */
    Q_DECL_DEPRECATED virtual void paintDataValueTexts(QPainter *painter);
    /**
     * \deprecated
     * This method is deprecated and provided for backward-compatibility only.
     * Your own diagram classes should call
     * d->paintDataValueTextsAndMarkers() instead
     * which also is taking care for showing your cell-specific comments, if any,
     */
    Q_DECL_DEPRECATED virtual void paintMarkers(QPainter *painter);
    void setAttributesModelRootIndex(const QModelIndex &);
    QModelIndex attributesModelRootIndex() const;

    /**
     * Helper method, retrieving the data value (DisplayRole) for a given row and column
     * @param row The row to query.
     * @param column The column to query.
     * @return The value of the display role at the given row and column as a qreal.
     * @deprecated
     */
    Q_DECL_DEPRECATED qreal valueForCell(int row, int column) const;

Q_SIGNALS:
    /** Diagrams are supposed to emit this signal, when the layout of one
            of their element changes. Layouts can change, for example, when
            axes are added or removed, or when the configuration was changed
            in a way that the axes or the diagram itself are displayed in a
            different geometry.
            Changes in the diagrams coordinate system also result
            in the layoutChanged() signal being emitted.
        */
    void layoutChanged(AbstractDiagram *);

    /**
     * This signal is emitted when this diagram is being destroyed, but before all the
     * data, i.e. the attributes model, is invalidated.
     */
    void aboutToBeDestroyed();

    /** This signal is emitted when either the model or the AttributesModel is replaced. */
    void modelsChanged();

    /** This signal is emitted just before the new attributes model is connected internally.
            It gives you a chance to connect to its signals first or perform other setup work. */
    void attributesModelAboutToChange(AttributesModel *newModel, AttributesModel *oldModel);

    /** This signal is emitted, when the model data is changed. */
    void modelDataChanged();

    /** This signal is emitted, when the hidden status of at least one data cell was (un)set. */
    void dataHidden();

    /** Emitted upon change of a property of the Diagram. */
    void propertiesChanged();

    /** Emitted upon change of a data boundary */
    void boundariesChanged();
    /** Emitted upon change of the view coordinate system */
    void viewportCoordinateSystemChanged();

private:
    QModelIndex conditionallyMapFromSource(const QModelIndex &sourceIndex) const;
};

typedef QList<AbstractDiagram *> AbstractDiagramList;
typedef QList<const AbstractDiagram *> ConstAbstractDiagramList;

/**
 * @brief Internally used class just adding a special constructor used by AbstractDiagram
 */
class PrivateAttributesModel : public AttributesModel
{
    Q_OBJECT
public:
    explicit PrivateAttributesModel(QAbstractItemModel *model, QObject *parent = nullptr)
        : AttributesModel(model, parent)
    {
    }
};
}

#endif
