/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTBARDIAGRAM_H
#define KDCHARTBARDIAGRAM_H

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartBarAttributes.h"

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

namespace KDChart {

class ThreeDBarAttributes;

/**
 * @brief BarDiagram defines a common bar diagram.
 *
 * It provides different subtypes which are set using \a setType.
 */
class KDCHART_EXPORT BarDiagram : public AbstractCartesianDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY(BarDiagram)

    KDCHART_DECLARE_DERIVED_DIAGRAM(BarDiagram, CartesianCoordinatePlane)

public:
    class BarDiagramType;
    friend class BarDiagramType;

    explicit BarDiagram(
        QWidget *parent = nullptr, CartesianCoordinatePlane *plane = nullptr);
    ~BarDiagram() override;

    virtual BarDiagram *clone() const;
    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare(const BarDiagram *other) const;

    enum BarType
    {
        Normal,
        Stacked,
        Percent,
        Rows Q_DECL_ENUMERATOR_DEPRECATED ///< @deprecated Use BarDiagram::setOrientation() instead
    };

    void setType(const BarType type);
    BarType type() const;

    void setOrientation(Qt::Orientation orientation);
    Qt::Orientation orientation() const;

    void setBarAttributes(const BarAttributes &a);
    void setBarAttributes(int column, const BarAttributes &a);
    void setBarAttributes(const QModelIndex &index, const BarAttributes &a);

    BarAttributes barAttributes() const;
    BarAttributes barAttributes(int column) const;
    BarAttributes barAttributes(const QModelIndex &index) const;

    void setThreeDBarAttributes(const ThreeDBarAttributes &a);
    void setThreeDBarAttributes(int column, const ThreeDBarAttributes &a);
    void setThreeDBarAttributes(const QModelIndex &index,
                                const ThreeDBarAttributes &a);
    ThreeDBarAttributes threeDBarAttributes() const;
    ThreeDBarAttributes threeDBarAttributes(int column) const;
    ThreeDBarAttributes threeDBarAttributes(const QModelIndex &index) const;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(Q_COMPILER_MANGLES_RETURN_TYPE)
    // implement AbstractCartesianDiagram
    /** \reimp */
    const int numberOfAbscissaSegments() const;
    /** \reimp */
    const int numberOfOrdinateSegments() const;
#else
    // implement AbstractCartesianDiagram
    /** \reimp */
    int numberOfAbscissaSegments() const override;
    /** \reimp */
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

private:
    void calculateValueAndGapWidths(int rowCount, int colCount,
                                    qreal groupWidth,
                                    qreal &barWidth,
                                    qreal &spaceBetweenBars,
                                    qreal &spaceBetweenGroups);
}; // End of class BarDiagram
}

#endif // KDCHARTBARDIAGRAM_H
