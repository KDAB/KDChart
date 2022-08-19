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

#ifndef KDCHARTTERNARYCOORDINATEPLANE_P_H
#define KDCHARTTERNARYCOORDINATEPLANE_P_H

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

#include "KDChartAbstractCoordinatePlane_p.h"
#include "KDChartTernaryGrid.h"

#include <KDABLibFakes>

namespace KDChart {

class TernaryAxis;

/**
 * \internal
 */

class TernaryCoordinatePlane::Private : public AbstractCoordinatePlane::Private
{
    friend class TernaryCoordinatePlane;

public:
    explicit Private();

    ~Private() override
    {
        // grid is delete in base class dtor
    }

    void initialize() override
    {
        grid = new TernaryGrid();
        xUnit = 0.0;
        yUnit = 0.0;
    }

    QList<TernaryAxis *> axes;

    TextAttributes labelAttributes;

    // the diagram is drawn within this rectangle, which is within
    // this widget:
    QRectF diagramRectContainer;
    // this is the "frame" of the plot area
    QRectF diagramRect;
    // multiply m_xUnit with a [0..1] value to get an isometric
    // widget coordinate
    qreal xUnit;
    // same for y:
    qreal yUnit;
};

KDCHART_IMPL_DERIVED_PLANE(TernaryCoordinatePlane, AbstractCoordinatePlane)
}

#endif /* KDCHARTTERNARYCOORDINATEPLANE_P_H */
