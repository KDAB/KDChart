/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTGRIDATTRIBUTES_H
#define KDCHARTGRIDATTRIBUTES_H

#include "KDChartEnums.h"
#include "KDChartGlobal.h"
#include <QMetaType>

QT_BEGIN_NAMESPACE
class QPen;
QT_END_NAMESPACE

namespace KDChart {

/**
 * @brief A set of attributes controlling the appearance of grids
 */
class KDCHART_EXPORT GridAttributes
{
public:
    GridAttributes();
    GridAttributes(const GridAttributes &);
    GridAttributes &operator=(const GridAttributes &);

    ~GridAttributes();

    void setGridVisible(bool visible);
    bool isGridVisible() const;

    /**
     * When this is enabled, grid lines are drawn only where axis annotations are.
     * Otherwise annotations are disregarded as far as the grid is concerned.
     *
     * The default is false.
     */
    void setLinesOnAnnotations(bool);
    bool linesOnAnnotations() const;

    void setGridStepWidth(qreal stepWidth = 0.0);
    qreal gridStepWidth() const;

    void setGridSubStepWidth(qreal subStepWidth = 0.0);
    qreal gridSubStepWidth() const;

    /**
     * Specify which granularity sequence is to be used to find a matching
     * grid granularity.
     *
     * See details explained at KDChartEnums::GranularitySequence.
     *
     * You might also want to use setAdjustBoundsToGrid for fine-tuning the
     * start/end value.
     *
     * \sa setAdjustBoundsToGrid, GranularitySequence
     */
    void setGridGranularitySequence(KDChartEnums::GranularitySequence sequence);
    KDChartEnums::GranularitySequence gridGranularitySequence() const;

    /**
     * By default visible bounds of the data area are adjusted to match
     * a main grid line.
     * If you set the respective adjust flag to false the bound will
     * not start at a grid line's value but it will be the exact value
     * of the data range set.
     *
     * \sa CartesianCoordinatePlane::setHorizontalRange
     * \sa CartesianCoordinatePlane::setVerticalRange
     */
    void setAdjustBoundsToGrid(bool adjustLower, bool adjustUpper);
    bool adjustLowerBoundToGrid() const;
    bool adjustUpperBoundToGrid() const;

    void setGridPen(const QPen &pen);
    QPen gridPen() const;

    void setSubGridVisible(bool visible);
    bool isSubGridVisible() const;

    void setSubGridPen(const QPen &pen);
    QPen subGridPen() const;

    void setOuterLinesVisible(bool visible);
    bool isOuterLinesVisible() const;

    void setZeroLinePen(const QPen &pen);
    QPen zeroLinePen() const;

    bool operator==(const GridAttributes &) const;
    inline bool operator!=(const GridAttributes &other) const
    {
        return !operator==(other);
    }

private:
    KDCHART_DECLARE_PRIVATE_BASE_VALUE(GridAttributes)
}; // End of class GridAttributes
}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::GridAttributes &);
#endif /* QT_NO_DEBUG_STREAM */

KDCHART_DECLARE_SWAP_SPECIALISATION(KDChart::GridAttributes)

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(KDChart::GridAttributes, Q_MOVABLE_TYPE);
QT_END_NAMESPACE

Q_DECLARE_METATYPE(KDChart::GridAttributes)

#endif // KDCHARTGRIDATTRIBUTES_H
