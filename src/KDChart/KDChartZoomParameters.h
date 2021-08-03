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

#ifndef ZOOMPARAMETERS_H
#define ZOOMPARAMETERS_H

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

#include <QPointF>

namespace KDChart {
/**
      * ZoomParameters stores the center and the factor of zooming internally
      * \internal
      */
class ZoomParameters
{
public:
    ZoomParameters()
    {
    }

    ZoomParameters(qreal xFactor, qreal yFactor, const QPointF &center)
        : xFactor(xFactor)
        , yFactor(yFactor)
        , xCenter(center.x())
        , yCenter(center.y())
    {
    }

    void setCenter(const QPointF &center)
    {
        xCenter = center.x();
        yCenter = center.y();
    }
    const QPointF center() const
    {
        return QPointF(xCenter, yCenter);
    }

    qreal xFactor = 1.0;
    qreal yFactor = 1.0;

    qreal xCenter = 0.5;
    qreal yCenter = 0.5;
};
}

#endif
