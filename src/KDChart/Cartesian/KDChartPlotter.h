/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTPLOTTER_H
#define KDCHARTPLOTTER_H

#include "KDChartAbstractCartesianDiagram.h"

#include "KDChartLineAttributes.h"
#include "KDChartValueTrackerAttributes.h"

namespace KDChart {

class ThreeDLineAttributes;

/**
 * @brief Plotter defines a diagram type plotting two-dimensional data.
 */
class KDCHART_EXPORT Plotter : public AbstractCartesianDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY(Plotter)
    Q_ENUMS(CompressionMode)

    KDCHART_DECLARE_DERIVED_DIAGRAM(Plotter, CartesianCoordinatePlane)
    Q_PROPERTY(CompressionMode useDataCompression READ useDataCompression WRITE setUseDataCompression)
    Q_PROPERTY(qreal mergeRadiusPercentage READ mergeRadiusPercentage WRITE setMergeRadiusPercentage)

public:
    // SLOPE enables a compression based on minimal slope changes
    // DISTANCE is still buggy and can fail, same for BOTH, NONE is the default mode
    enum CompressionMode
    {
        SLOPE,
        DISTANCE,
        BOTH,
        NONE
    };
    class PlotterType;
    friend class PlotterType;

    explicit Plotter(QWidget *parent = nullptr, CartesianCoordinatePlane *plane = nullptr);
    ~Plotter() override;

    virtual Plotter *clone() const;

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare(const Plotter *other) const;

    enum PlotType
    {
        Normal = 0,
        Percent
    };

    void setType(const PlotType type);
    PlotType type() const;

    void setLineAttributes(const LineAttributes &a);
    void setLineAttributes(int column, const LineAttributes &a);
    void setLineAttributes(const QModelIndex &index, const LineAttributes &a);
    void resetLineAttributes(int column);
    void resetLineAttributes(const QModelIndex &index);
    LineAttributes lineAttributes() const;
    LineAttributes lineAttributes(int column) const;
    LineAttributes lineAttributes(const QModelIndex &index) const;

    void setThreeDLineAttributes(const ThreeDLineAttributes &a);
    void setThreeDLineAttributes(int column, const ThreeDLineAttributes &a);
    void setThreeDLineAttributes(const QModelIndex &index,
                                 const ThreeDLineAttributes &a);

    ThreeDLineAttributes threeDLineAttributes() const;
    ThreeDLineAttributes threeDLineAttributes(int column) const;
    ThreeDLineAttributes threeDLineAttributes(const QModelIndex &index) const;

    void setValueTrackerAttributes(const QModelIndex &index,
                                   const ValueTrackerAttributes &a);
    ValueTrackerAttributes valueTrackerAttributes(const QModelIndex &index) const;

    CompressionMode useDataCompression() const;
    void setUseDataCompression(CompressionMode value);

    qreal maxSlopeChange() const;
    void setMaxSlopeChange(qreal value);

    qreal mergeRadiusPercentage() const;
    void setMergeRadiusPercentage(qreal value);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(Q_COMPILER_MANGLES_RETURN_TYPE)
    // implement AbstractCartesianDiagram
    /* reimp */
    const int numberOfAbscissaSegments() const;
    /* reimp */
    const int numberOfOrdinateSegments() const;
#else
    // implement AbstractCartesianDiagram
    /* reimp */
    int numberOfAbscissaSegments() const override;
    /* reimp */
    int numberOfOrdinateSegments() const override;
#endif

protected Q_SLOTS:
    void connectAttributesModel(AttributesModel *);

protected:
    void paint(PaintContext *paintContext) override;

public:
    void resize(const QSizeF &area) override;

protected:
    qreal threeDItemDepth(const QModelIndex &index) const override;
    qreal threeDItemDepth(int column) const override;
    /** \reimp */
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
protected Q_SLOTS:
    void setDataBoundariesDirty();
    void calcMergeRadius();
}; // End of class KDChart::Plotter
}

#endif // KDCHARTLINEDIAGRAM_H
