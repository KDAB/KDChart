/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHART_POLAR_COORDINATEPLANE_H
#define KDCHART_POLAR_COORDINATEPLANE_H

#include "KDChartAbstractCoordinatePlane.h"

namespace KDChart {

class Chart;
class PaintContext;

/**
 * @brief Polar coordinate plane
 */
class KDCHART_EXPORT PolarCoordinatePlane : public AbstractCoordinatePlane
{
    Q_OBJECT

    Q_DISABLE_COPY(PolarCoordinatePlane)
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT(PolarCoordinatePlane, Chart *)

public:
    struct CoordinateTransformation;
    typedef QList<CoordinateTransformation> CoordinateTransformationList;

    explicit PolarCoordinatePlane(Chart *parent = nullptr);
    ~PolarCoordinatePlane() override;

    void addDiagram(AbstractDiagram *diagram) override;

    const QPointF translate(const QPointF &diagramPoint) const override;
    const QPointF translatePolar(const QPointF &diagramPoint) const;

    /** \brief Specify the rotation of the coordinate plane.
     *
     * In a pie diagram this indicates the position where the first pie starts,
     * in a polar diagram it specifies the Zero position of the circular axis:
     *
     * \image html polar-plane-start-position.png "Illustration of \"start position\" property"
     *
     * \sa startPosition
     */
    void setStartPosition(qreal degrees);
    /** Retrieve the rotation of the coordinate plane.
     * \sa setStartPosition
     */
    qreal startPosition() const;

    qreal zoomFactorX() const override;
    qreal zoomFactorY() const override;

    void setZoomFactors(qreal factorX, qreal factorY) override;
    void setZoomFactorX(qreal factor) override;
    void setZoomFactorY(qreal factor) override;

    QPointF zoomCenter() const override;

    void setZoomCenter(const QPointF &center) override;

    /**
     * Set the attributes to be used for grid lines drawn in circular
     * direction (or in sagittal direction, resp.).
     *
     * To disable circular grid painting, for example, your code should like this:
     * \code
     * GridAttributes ga = plane->gridAttributes( bool );
     * ga.setGridVisible( false );
     * plane-setGridAttributes( bool, ga );
     * \endcode
     *
     * \note setGridAttributes overwrites the global attributes that
     * were set by AbstractCoordinatePlane::setGlobalGridAttributes.
     * To re-activate these global attributes you can call
     * resetGridAttributes.
     *
     * \sa resetGridAttributes, gridAttributes
     * \sa AbstractCoordinatePlane::setGlobalGridAttributes
     * \sa hasOwnGridAttributes
     */
    void setGridAttributes(bool circular, const GridAttributes &);

    /**
     * Reset the attributes to be used for grid lines drawn in circular
     * direction (or in sagittal direction, resp.).
     * By calling this method you specify that the global attributes set by
     * AbstractCoordinatePlane::setGlobalGridAttributes be used.
     *
     * \sa setGridAttributes, gridAttributes
     * \sa AbstractCoordinatePlane::globalGridAttributes
     * \sa hasOwnGridAttributes
     */
    void resetGridAttributes(bool circular);

    /**
     * \return The attributes used for grid lines drawn in circular
     * direction (or in sagittal direction, resp.).
     *
     * \note This function always returns a valid set of grid attributes:
     * If no special grid attributes were set for this direction
     * the global attributes are returned, as returned by
     * AbstractCoordinatePlane::globalGridAttributes.
     *
     * \sa setGridAttributes
     * \sa resetGridAttributes
     * \sa AbstractCoordinatePlane::globalGridAttributes
     * \sa hasOwnGridAttributes
     */
    const GridAttributes gridAttributes(bool circular) const;

    /**
     * \return Returns whether the grid attributes have been set for the
     * respective direction via setGridAttributes( bool circular ).
     *
     * If false, the grid will use the global attributes set
     * by AbstractCoordinatePlane::globalGridAttributes (or the default
     * attributes, resp.)
     *
     * \sa setGridAttributes
     * \sa resetGridAttributes
     * \sa AbstractCoordinatePlane::globalGridAttributes
     */
    bool hasOwnGridAttributes(bool circular) const;

    qreal angleUnit() const;
    qreal radiusUnit() const;

    /** reimp */
    void paint(QPainter *) override;

protected:
    DataDimensionsList getDataDimensionsList() const override;
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    void layoutDiagrams() override;
protected Q_SLOTS:
    void slotLayoutChanged(AbstractDiagram *diagram);
    void adjustZoomAndRepaint();

private:
    void setHasOwnGridAttributes(bool circular, bool on);
};
}

#endif
