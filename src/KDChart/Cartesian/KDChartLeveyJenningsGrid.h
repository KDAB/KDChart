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

#ifndef KDCHARTLEVEYJENNINGSGRID_H
#define KDCHARTLEVEYJENNINGSGRID_H

#include "KDChartCartesianGrid.h"

namespace KDChart {

class PaintContext;

/**
 * \internal
 *
 * \brief Class for the grid in a Levey Jennings plane.
 *
 * The LeveyJenningsGrid interface is used
 * for calculating and for drawing
 * the horizontal grid lines, and the vertical grid lines
 * of a Levey Jennings coordinate plane.
 */
class LeveyJenningsGrid : public CartesianGrid
{
public:
    LeveyJenningsGrid()
        : CartesianGrid()
    {
    }
    ~LeveyJenningsGrid() override
    {
    }

    void drawGrid(PaintContext *context) override;

private:
    DataDimensionsList calculateGrid(const DataDimensionsList &rawDataDimensions) const override;
    DataDimension calculateGridXY(const DataDimension &rawDataDimension,
                                  Qt::Orientation orientation, bool adjustLower, bool adjustUpper) const override;
    void calculateStepWidth(qreal start_, qreal end_, const QList<qreal> &granularities, Qt::Orientation orientation,
                            qreal &stepWidth, qreal &subStepWidth, bool adjustLower, bool adjustUpper) const override;
};
}

#endif
