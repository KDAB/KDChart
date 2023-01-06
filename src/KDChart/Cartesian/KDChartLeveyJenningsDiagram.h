/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTLEVEYJENNINGSDIAGRAM_H
#define KDCHARTLEVEYJENNINGSDIAGRAM_H

#include "KDChartLeveyJenningsCoordinatePlane.h"
#include "KDChartLineDiagram.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QPolygonF;
class QSvgRenderer;
QT_END_NAMESPACE

namespace KDChart {

class ThreeDLineAttributes;

/**
 * @brief LeveyDiagram defines a Levey Jennings chart.
 *
 * It provides different subtypes which are set using \a setType.
 */
class KDCHART_EXPORT LeveyJenningsDiagram : public LineDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY(LeveyJenningsDiagram)
    //    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( LineDiagram, CartesianCoordinatePlane * )

    KDCHART_DECLARE_DERIVED_DIAGRAM(LeveyJenningsDiagram, LeveyJenningsCoordinatePlane)

public:
    explicit LeveyJenningsDiagram(QWidget *parent = nullptr, LeveyJenningsCoordinatePlane *plane = nullptr);
    ~LeveyJenningsDiagram() override;

    LineDiagram *clone() const override;

    enum Symbol
    {
        OkDataPoint,
        NotOkDataPoint,
        LotChanged,
        SensorChanged,
        FluidicsPackChanged
    };

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare(const LeveyJenningsDiagram *other) const;

    void setLotChangedSymbolPosition(Qt::Alignment pos);
    Qt::Alignment lotChangedSymbolPosition() const;

    void setFluidicsPackChangedSymbolPosition(Qt::Alignment pos);
    Qt::Alignment fluidicsPackChangedSymbolPosition() const;

    void setSensorChangedSymbolPosition(Qt::Alignment pos);
    Qt::Alignment sensorChangedSymbolPosition() const;

    void setExpectedMeanValue(float meanValue);
    float expectedMeanValue() const;

    void setExpectedStandardDeviation(float sd);
    float expectedStandardDeviation() const;

    float calculatedMeanValue() const;
    float calculatedStandardDeviation() const;

    void setFluidicsPackChanges(const QVector<QDateTime> &changes);
    QVector<QDateTime> fluidicsPackChanges() const;

    void setSensorChanges(const QVector<QDateTime> &changes);
    QVector<QDateTime> sensorChanges() const;

    void setScanLinePen(const QPen &pen);
    QPen scanLinePen() const;

    void setSymbol(Symbol symbol, const QString &filename);
    QString symbol(Symbol symbol) const;

    /* \reimp */
    void setModel(QAbstractItemModel *model) override;

    QPair<QDateTime, QDateTime> timeRange() const;
    void setTimeRange(const QPair<QDateTime, QDateTime> &timeRange);

protected:
    void paint(PaintContext *paintContext) override;
    void drawChanges(PaintContext *paintContext);

    virtual void drawDataPointSymbol(PaintContext *paintContext, const QPointF &pos, bool ok);
    virtual void drawLotChangeSymbol(PaintContext *paintContext, const QPointF &pos);
    virtual void drawSensorChangedSymbol(PaintContext *paintContext, const QPointF &pos);
    virtual void drawFluidicsPackChangedSymbol(PaintContext *paintContext, const QPointF &pos);

    virtual QRectF iconRect() const;

    QSvgRenderer *iconRenderer(Symbol symbol);

    /** \reimp */
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;

protected Q_SLOTS:
    void calculateMeanAndStandardDeviation() const;
}; // End of class KDChartLineDiagram
}

#endif // KDCHARTLINEDIAGRAM_H
