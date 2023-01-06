/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTRADARDIAGRAM_P_H
#define KDCHARTRADARDIAGRAM_P_H

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

#include "KDChartAbstractPolarDiagram_p.h"
#include "KDChartRadarDiagram.h"

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class RadarDiagram::Private : public AbstractPolarDiagram::Private
{
    friend class RadarDiagram;

public:
    Private();
    ~Private() override;

    Private(const Private &rhs)
        : AbstractPolarDiagram::Private(rhs)
        , closeDatasets(rhs.closeDatasets)
        , reverseData(rhs.reverseData)
        , fillAlpha(rhs.fillAlpha)
    {
    }

private:
    bool closeDatasets = false;
    bool reverseData = false;
    qreal fillAlpha = 0.0;
    LabelPaintCache labelPaintCache;
};

KDCHART_IMPL_DERIVED_DIAGRAM(RadarDiagram, AbstractPolarDiagram, RadarCoordinatePlane)
}

#endif /* KDCHARTRADARDIAGRAM_P_H */
