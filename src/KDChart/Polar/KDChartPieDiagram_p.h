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

#ifndef KDCHARTPIEDIAGRAM_P_H
#define KDCHARTPIEDIAGRAM_P_H

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

#include "KDChartAbstractPieDiagram_p.h"

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class PieDiagram::Private : public AbstractPieDiagram::Private
{
    friend class PieDiagram;

public:
    Private();
    ~Private() override;

    Private(const Private &rhs)
        : AbstractPieDiagram::Private(rhs)
        , startAngles()
        , angleLens()
        , size(0)
        , labelDecorations(NoDecoration)
        , isCollisionAvoidanceEnabled(false)
    {
        // just for consistency
    }

protected:
    // this information needed temporarily at drawing time
    QVector<qreal> startAngles;
    QVector<qreal> angleLens;
    qreal size;
    LabelPaintCache labelPaintCache;
    PieDiagram::LabelDecorations labelDecorations;
    bool isCollisionAvoidanceEnabled = false;
};

KDCHART_IMPL_DERIVED_DIAGRAM(PieDiagram, AbstractPieDiagram, PolarCoordinatePlane)
}

#endif /* KDCHARTPIEDIAGRAM_P_H */
