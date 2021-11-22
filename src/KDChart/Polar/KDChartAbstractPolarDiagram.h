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

#ifndef KDCHARTABSTRACTPOLARDIAGRAM_H
#define KDCHARTABSTRACTPOLARDIAGRAM_H

#include "KDChartPolarCoordinatePlane.h"
#include "KDChartAbstractDiagram.h"

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
