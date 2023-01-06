/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTPOLARGrid_H
#define KDCHARTPOLARGrid_H

#include "KDChartAbstractGrid.h"
#include "KDChartPolarCoordinatePlane.h"

namespace KDChart {

class PaintContext;
class PolarCoordinatePlane;

/**
 * \internal
 *
 * \brief Class for the grid in a polar plane.
 *
 * The PolarGrid interface is used
 * for calculating and for drawing
 * the sagittal grid lines, and the circular grid lines
 * of a polar coordinate plane.
 */
class PolarGrid : public AbstractGrid
{
public:
    PolarGrid()
        : AbstractGrid()
    {
    }
    ~PolarGrid() override
    {
    }

    void drawGrid(PaintContext *context) override;

private:
    DataDimensionsList calculateGrid(
        const DataDimensionsList &rawDataDimensions) const override;
};
}

#endif
