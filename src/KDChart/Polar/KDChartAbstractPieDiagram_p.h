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

#ifndef KDCHARTABSTRACTPIEDIAGRAM_P_H
#define KDCHARTABSTRACTPIEDIAGRAM_P_H

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
#include <KDChartAbstractThreeDAttributes.h>

#include <KDABLibFakes>

namespace KDChart {

class PolarCoordinatePlane;

/**
 * \internal
 */
class AbstractPieDiagram::Private : public AbstractPolarDiagram::Private
{
    friend class AbstractPieDiagram;

public:
    Private();
    ~Private() override;

    Private(const Private &rhs)
        : AbstractPolarDiagram::Private(rhs)
        , granularity(rhs.granularity)
        , autoRotateLabels(false)
    {
    }

private:
    qreal granularity;
    bool autoRotateLabels;
};

KDCHART_IMPL_DERIVED_DIAGRAM(AbstractPieDiagram, AbstractPolarDiagram, PolarCoordinatePlane)

}
#endif /* KDCHARTABSTRACTPIEDIAGRAM_P_H */
