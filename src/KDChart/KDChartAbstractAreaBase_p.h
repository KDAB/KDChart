/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTAREABASE_P_H
#define KDCHARTABSTRACTAREABASE_P_H

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

/** \file KDChartAbstractAreaBase_p.h
 *  \internal
 */

#include "KDChartAbstractAreaBase.h"
#include "KDChartBackgroundAttributes.h"
#include "KDChartFrameAttributes.h"
#include "KDChartTextAttributes.h"

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class AbstractAreaBase::Private
{
    friend class AbstractAreaBase;

public:
    explicit Private();
    virtual ~Private();

    Private(const Private &rhs)
        : amountOfLeftOverlap(0)
        , amountOfRightOverlap(0)
        , amountOfTopOverlap(0)
        , amountOfBottomOverlap(0)
        , visible(rhs.visible)
        , frameAttributes(rhs.frameAttributes)
        , backgroundAttributes(rhs.backgroundAttributes)
    {
    }

protected:
    void init();

    // These are set each time the area's sizeHint()
    // (or the maximumSize(), resp.) is calculated:
    // They store additional layout-information about
    // space needed around the area.
    // Other classes (e.g. KDChart::AutoSpacer) can use
    // these data to determine how much space has to
    // be added additionally ...
    mutable int amountOfLeftOverlap;
    mutable int amountOfRightOverlap;
    mutable int amountOfTopOverlap;
    mutable int amountOfBottomOverlap;

private:
    bool visible = true;
    KDChart::FrameAttributes frameAttributes;
    KDChart::BackgroundAttributes backgroundAttributes;
};

inline AbstractAreaBase::AbstractAreaBase(AbstractAreaBase::Private *p)
    : _d(p)
{
    init();
}
}
#endif /* KDCHARTABSTRACTAREABASE_P_H */
