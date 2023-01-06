/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTTHREEDATTRIBUTES_P_H
#define KDCHARTABSTRACTTHREEDATTRIBUTES_P_H

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

#include <KDChartAbstractThreeDAttributes.h>

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class AbstractThreeDAttributes::Private
{
    friend class AbstractThreeDAttributes;

public:
    Private();
    virtual ~Private() = default;

private:
    bool enabled = false;
    // FIXME: What is this needed for? height is not referenced anywhere.
    //    int height;
    qreal depth = 20;
    bool threeDBrushEnabled = false;
};

inline AbstractThreeDAttributes::AbstractThreeDAttributes(Private *p)
    : _d(p)
{
    init();
}
}

#endif // KDCHARTABSTRACTTHREEDATTRIBUTES_P_H
