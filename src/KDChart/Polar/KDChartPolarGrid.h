/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef KDCHARTPOLARGrid_H
#define KDCHARTPOLARGrid_H

#include "KDChartPolarCoordinatePlane.h"
#include "KDChartAbstractGrid.h"

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
