/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTPOLARDIAGRAM_H
#define KDCHARTPOLARDIAGRAM_H

#include "KDChartAbstractPolarDiagram.h"
#include "KDChartPosition.h"

QT_BEGIN_NAMESPACE
class QPolygonF;
QT_END_NAMESPACE

namespace KDChart {

/**
 * @brief PolarDiagram defines a common polar diagram
 */
class KDCHART_EXPORT PolarDiagram : public AbstractPolarDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY(PolarDiagram)
    KDCHART_DECLARE_DERIVED_DIAGRAM(PolarDiagram, PolarCoordinatePlane)

public:
    explicit PolarDiagram(
        QWidget *parent = nullptr, PolarCoordinatePlane *plane = nullptr);
    ~PolarDiagram() override;

protected:
    // Implement AbstractDiagram
    /** \reimp */
    void paint(PaintContext *paintContext) override;

public:
    /** \reimp */
    void resize(const QSizeF &area) override;

    // Implement AbstractPolarDiagram
    /** \reimp */
    qreal valueTotals() const override;
    /** \reimp */
    qreal numberOfValuesPerDataset() const override;
    /** \reimp */
    qreal numberOfGridRings() const override;

    virtual PolarDiagram *clone() const;

    /** \deprecated Use PolarCoordinatePlane::setStartPosition( qreal degrees ) instead. */
    Q_DECL_DEPRECATED void setZeroDegreePosition(int degrees);
    /** \deprecated Use qreal PolarCoordinatePlane::startPosition instead. */
    Q_DECL_DEPRECATED int zeroDegreePosition() const;

    void setRotateCircularLabels(bool rotateCircularLabels);
    bool rotateCircularLabels() const;

    /** Close each of the data series by connecting the last point to its
     * respective start point
     */
    void setCloseDatasets(bool closeDatasets);
    bool closeDatasets() const;

    void setShowDelimitersAtPosition(Position position,
                                     bool showDelimiters);
    void setShowLabelsAtPosition(Position position,
                                 bool showLabels);

    bool showDelimitersAtPosition(Position position) const;

    bool showLabelsAtPosition(Position position) const;

    virtual void paint(PaintContext *paintContext,
                       bool calculateListAndReturnScale,
                       qreal &newZoomX, qreal &newZoomY); // KDChart 3: references -> pointers

protected:
    /** \reimp */
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    virtual void paintPolarMarkers(PaintContext *ctx, const QPolygonF &polygon);

}; // End of class PolarDiagram
}

#endif // KDCHARTPOLARDIAGRAM_H
