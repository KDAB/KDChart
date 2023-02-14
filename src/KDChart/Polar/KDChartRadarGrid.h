/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTRADARGrid_H
#define KDCHARTRADARGrid_H

// #include "KDChartRadarCoordinatePlane.h"
#include "KDChartAbstractGrid.h"

namespace KDChart {

class PaintContext;
class RadarCoordinatePlane;

/**
 * \internal
 *
 * \brief Class for the grid in a radar plane.
 *
 * The RadarGrid interface is used
 * for calculating and for drawing
 * the grid lines of a radar charts including "axis"
 * labels.
 */
class RadarGrid : public AbstractGrid
{
public:
    RadarGrid()
        : AbstractGrid()
    {
    }
    ~RadarGrid() override
    {
    }

    void drawGrid(PaintContext *context) override;

private:
    DataDimensionsList calculateGrid(
        const DataDimensionsList &rawDataDimensions) const override;
};
}

#endif
