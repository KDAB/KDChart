/****************************************************************************
** Copyright (C) 2001-2021 Klaralvdalens Datakonsult AB.  All rights reserved.
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
** Free Software Foundation and appearing in the file LICENSE.GPL included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDCHARTRADARDIAGRAM_H
#define KDCHARTRADARDIAGRAM_H

#include "KDChartPolarDiagram.h"
#include "KDChartPosition.h"
#include "KDChartRadarCoordinatePlane.h"

QT_BEGIN_NAMESPACE
class QPolygonF;
QT_END_NAMESPACE

namespace KDChart
{
/**
 * @brief RadarDiagram defines a common radar diagram
 */
class KDCHART_EXPORT RadarDiagram : public AbstractPolarDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY(RadarDiagram)
    KDCHART_DECLARE_DERIVED_DIAGRAM(RadarDiagram, RadarCoordinatePlane)

public:
    explicit RadarDiagram(QWidget *parent = nullptr, RadarCoordinatePlane *plane = nullptr);
    ~RadarDiagram() override;

    virtual void paint(PaintContext *paintContext, bool calculateListAndReturnScale, qreal &newZoomX, qreal &newZoomY);
    /** \reimpl */
    void resize(const QSizeF &area) override;

    /** \reimpl */
    [[nodiscard]] qreal valueTotals() const override;
    /** \reimpl */
    [[nodiscard]] qreal numberOfValuesPerDataset() const override;
    /** \reimpl */
    [[nodiscard]] qreal numberOfGridRings() const override;

    /**
     * if val is true the diagram will mirror the diagram datapoints
     */
    void setReverseData(bool val);
    bool reverseData();

    [[nodiscard]] virtual RadarDiagram *clone() const;

    /**
     * Close each of the data series by connecting the last point to its
     * respective start point
     */
    void setCloseDatasets(bool closeDatasets);
    [[nodiscard]] bool closeDatasets() const;

    /**
     * Fill the areas of the radar chart with there respective color defined
     * via KDChart::DatasetBrushRole. The value defines the alpha of the
     * color to use. If set to 0.0 (the default) then the radar areas will
     * not be filled with any color. If set to 1.0 then the areas will be
     * solid filled and are not transparent.
     */
    [[nodiscard]] qreal fillAlpha() const;
    void setFillAlpha(qreal alphaF);

protected:
    /** \reimpl */
    [[nodiscard]] const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void paint(PaintContext *paintContext) override;

}; // End of class RadarDiagram

}

#endif // KDCHARTRADARDIAGRAM_H
