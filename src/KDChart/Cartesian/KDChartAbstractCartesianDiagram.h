/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTCARTESIANDIAGRAM_H
#define KDCHARTABSTRACTCARTESIANDIAGRAM_H

#include "KDChartAbstractDiagram.h"
#include "KDChartCartesianAxis.h"
#include "KDChartCartesianCoordinatePlane.h"

namespace KDChart {

class GridAttributes;

/**
 * @brief Base class for diagrams based on a cartesian coordianate system.
 *
 * The AbstractCartesianDiagram interface adds those elements that are
 * specific to diagrams based on a cartesian coordinate system to the
 * basic AbstractDiagram interface.
 */
class KDCHART_EXPORT AbstractCartesianDiagram : public AbstractDiagram
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractCartesianDiagram)
    KDCHART_DECLARE_DERIVED_DIAGRAM(AbstractCartesianDiagram, CartesianCoordinatePlane)

public:
    explicit AbstractCartesianDiagram(QWidget *parent = nullptr, CartesianCoordinatePlane *plane = nullptr);
    ~AbstractCartesianDiagram() override;

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare(const AbstractCartesianDiagram *other) const;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(Q_COMPILER_MANGLES_RETURN_TYPE)
    virtual const int numberOfAbscissaSegments() const = 0;
    virtual const int numberOfOrdinateSegments() const = 0;
#else
    virtual int numberOfAbscissaSegments() const = 0;
    virtual int numberOfOrdinateSegments() const = 0;
#endif
    /**
     * Add the axis to the diagram. The diagram takes ownership of the axis
     * and will delete it.
     *
     * To gain back ownership (e.g. for assigning the axis to another diagram)
     * use the takeAxis method, before calling addAxis on the other diagram.
     *
     * \sa takeAxis
     */
    virtual void addAxis(CartesianAxis *axis);
    /**
     * Removes the axis from the diagram, without deleting it.
     *
     * The diagram no longer owns the axis, so it is
     * the caller's responsibility to delete the axis.
     *
     * \sa addAxis
     */
    virtual void takeAxis(CartesianAxis *axis);
    /**
     * @return a list of all axes added to the diagram
     */
    virtual KDChart::CartesianAxisList axes() const;

    /**
     * Triggers layouting of all coordinate planes on the current chart.
     * Normally you don't need to call this method. It's handled automatically for you.
     */
    virtual void layoutPlanes();
    /** \reimp */
    void setCoordinatePlane(AbstractCoordinatePlane *plane) override;

    /**
     * Makes this diagram use another diagram \a diagram as reference diagram with relative offset
     * \a offset.
     * To share cartesian axes between different diagrams there might be cases when you need that.
     * Normally you don't.
     * \sa examples/SharedAbscissa
     */
    virtual void setReferenceDiagram(AbstractCartesianDiagram *diagram, const QPointF &offset = QPointF());
    /**
     * @return this diagram's reference diagram
     *  \sa setReferenceDiagram
     */
    virtual AbstractCartesianDiagram *referenceDiagram() const;
    /**
     * @return the relative offset of this diagram's reference diagram
     * \sa setReferenceDiagram
     */
    virtual QPointF referenceDiagramOffset() const;

    /* reimp */
    void setModel(QAbstractItemModel *model) override;
    /* reimp */
    void setRootIndex(const QModelIndex &index) override;
    /* reimp */
    void setAttributesModel(AttributesModel *amodel) override;

protected Q_SLOTS:
    void connectAttributesModel(AttributesModel *);

protected:
    /** @return the 3D item depth of the model index \a index */
    virtual qreal threeDItemDepth(const QModelIndex &index) const = 0;
    /** @return the 3D item depth of the data set \a column */
    virtual qreal threeDItemDepth(int column) const = 0;
};
}

#endif
