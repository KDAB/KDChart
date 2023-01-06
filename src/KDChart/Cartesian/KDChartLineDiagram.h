/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTLINEDIAGRAM_H
#define KDCHARTLINEDIAGRAM_H

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartLineAttributes.h"
#include "KDChartValueTrackerAttributes.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QPolygonF;
QT_END_NAMESPACE

namespace KDChart {

class ThreeDLineAttributes;

/**
 * @brief LineDiagram defines a common line diagram.
 *
 * It provides different subtypes which are set using \a setType.
 */
class KDCHART_EXPORT LineDiagram : public AbstractCartesianDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY(LineDiagram)
    //    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( LineDiagram, CartesianCoordinatePlane * )

    KDCHART_DECLARE_DERIVED_DIAGRAM(LineDiagram, CartesianCoordinatePlane)

public:
    class LineDiagramType;
    friend class LineDiagramType;

    explicit LineDiagram(QWidget *parent = nullptr, CartesianCoordinatePlane *plane = nullptr);
    ~LineDiagram() override;

    virtual LineDiagram *clone() const;

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare(const LineDiagram *other) const;

    enum LineType
    {
        Normal = 0,
        Stacked = 1,
        Percent = 2
    };

    void setType(const LineType type);
    LineType type() const;

    /** If centerDataPoints() is true, all data points are moved by an
     * offset of 0.5 to the right. This is useful in conjunction with
     * bar diagrams, since data points are then centered just like bars.
     *
     * \sa centerDataPoints()
     */
    void setCenterDataPoints(bool center);
    /** @return option set by setCenterDataPoints() */
    bool centerDataPoints() const;

    /** With this property set to true, data sets in a normal line diagram
     * are drawn in reversed order. More clearly, the first (top-most) data set
     * in the source model will then appear in front. This is mostly due to
     * historical reasons.
     */
    void setReverseDatasetOrder(bool reverse);
    /** \see setReverseDatasetOrder */
    bool reverseDatasetOrder() const;

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

    /**
     * This property defines the strength of the line curvature - 0 for
     * straight lines between data points, 1 for a smooth curve through
     * the data points
     */
    void setLineTension(qreal tenson);
    qreal lineTension() const;

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
}; // End of class KDChartLineDiagram
}

#endif // KDCHARTLINEDIAGRAM_H
