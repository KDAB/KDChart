/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTRINGDIAGRAM_H
#define KDCHARTRINGDIAGRAM_H

#include "KDChartAbstractPieDiagram.h"

namespace KDChart {

/**
 * @brief RingDiagram defines a common ring diagram
 */
class KDCHART_EXPORT RingDiagram : public AbstractPieDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY(RingDiagram)
    KDCHART_DECLARE_DERIVED_DIAGRAM(RingDiagram, PolarCoordinatePlane)

public:
    explicit RingDiagram(
        QWidget *parent = nullptr, PolarCoordinatePlane *plane = nullptr);
    ~RingDiagram() override;

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
    qreal numberOfDatasets() const override;
    /** \reimp */
    qreal numberOfGridRings() const override;

    qreal valueTotals(int dataset) const;

    virtual RingDiagram *clone() const;

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare(const RingDiagram *other) const;

    void setRelativeThickness(bool relativeThickness);
    bool relativeThickness() const;

    virtual void setExpandWhenExploded(bool expand);
    virtual bool expandWhenExploded() const;

protected:
    /** \reimp */
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    void drawOneSlice(QPainter *painter, uint dataset, uint slice, qreal granularity);
    void drawPieSurface(QPainter *painter, uint dataset, uint slice, qreal granularity);
    QPointF pointOnEllipse(const QRectF &rect, int dataset, int slice, bool outer, qreal angle,
                           qreal totalGapFactor, qreal totalExplodeFactor);
}; // End of class RingDiagram
}

#endif // KDCHARTRINGDIAGRAM_H
