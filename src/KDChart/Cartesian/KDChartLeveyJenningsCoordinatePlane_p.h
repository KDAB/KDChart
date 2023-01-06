/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTLEVEYJENNINGSCOORDINATEPLANE_P_H
#define KDCHARTLEVEYJENNINGSCOORDINATEPLANE_P_H

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

#include <QRectF>

#include "KDChartCartesianCoordinatePlane_p.h"
#include "KDChartLeveyJenningsGrid.h"

#include <KDABLibFakes>

namespace KDChart {

class CartesianAxis;

/**
 * \internal
 */

class LeveyJenningsCoordinatePlane::Private : public CartesianCoordinatePlane::Private
{
    friend class LeveyJenningsCoordinatePlane;

public:
    explicit Private();

    ~Private() override
    {
        // grid is delete in base class dtor
    }

    void initialize() override
    {
        grid = new LeveyJenningsGrid();
    }

    LeveyJenningsGridAttributes gridAttributes;
};

KDCHART_IMPL_DERIVED_PLANE(LeveyJenningsCoordinatePlane, CartesianCoordinatePlane)
}

#endif /* KDCHARTTERNARYCOORDINATEPLANE_P_H */
