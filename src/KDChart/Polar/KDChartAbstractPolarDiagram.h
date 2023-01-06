/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTPOLARDIAGRAM_H
#define KDCHARTABSTRACTPOLARDIAGRAM_H

#include "KDChartAbstractDiagram.h"
#include "KDChartPolarCoordinatePlane.h"

namespace KDChart {

class GridAttributes;

/**
 * @brief Base class for diagrams based on a polar coordinate system.
 */
class KDCHART_EXPORT AbstractPolarDiagram : public AbstractDiagram
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractPolarDiagram)
    KDCHART_DECLARE_DERIVED_DIAGRAM(AbstractPolarDiagram, PolarCoordinatePlane)

public:
    explicit AbstractPolarDiagram(
        QWidget *parent = nullptr, PolarCoordinatePlane *plane = nullptr);
    ~AbstractPolarDiagram() override
    {
    }

    virtual qreal valueTotals() const = 0;
    virtual qreal numberOfValuesPerDataset() const = 0;
    virtual qreal numberOfDatasets() const
    {
        return 1;
    };
    virtual qreal numberOfGridRings() const = 0;

    const PolarCoordinatePlane *polarCoordinatePlane() const;

    int columnCount() const;
    int rowCount() const;
};
}

#endif
