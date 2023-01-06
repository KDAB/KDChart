/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTRADARCOORDINATEPLANE_P_H
#define KDCHARTRADARCOORDINATEPLANE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartPolarCoordinatePlane_p.h"
#include "KDChartRadarCoordinatePlane.h"
#include "KDChartRadarGrid.h"

namespace KDChart {

class RadarCoordinatePlane::Private : public PolarCoordinatePlane::Private
{
    friend class RadarCoordinatePlane;

public:
    explicit Private()
    {
    }

    ~Private() override
    {
    }
    TextAttributes textAttributes;

    void initialize() override
    {
        grid = new RadarGrid();
    }
};

KDCHART_IMPL_DERIVED_PLANE(RadarCoordinatePlane, PolarCoordinatePlane)
}

#endif /* KDCHARTBARDIAGRAM_P_H */
