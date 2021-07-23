/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
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
    /** \reimpl */
    void paint(PaintContext *paintContext) override;

public:
    /** \reimpl */
    void resize(const QSizeF &area) override;

    // Implement AbstractPolarDiagram
    /** \reimpl */
    qreal valueTotals() const override;
    /** \reimpl */
    qreal numberOfValuesPerDataset() const override;
    qreal numberOfDatasets() const override;
    /** \reimpl */
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
    /** \reimpl */
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
