/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHART_TEXT_AREA_P_H
#define KDCHART_TEXT_AREA_P_H

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

/** \file KDChartTextArea_p.h
 *  \internal
 */

#include "KDChartAbstractAreaBase_p.h"
#include "KDChartTextArea.h"

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class TextArea::Private : public AbstractAreaBase::Private
{
    friend class TextArea;

public:
    explicit Private();
    ~Private() override;

    Private(const Private &rhs)
        : AbstractAreaBase::Private(rhs)
    {
        // Just for consistency
    }
};

inline TextArea::TextArea(Private *p)
    : QObject()
    , AbstractAreaBase(p)
    , TextLayoutItem()
{
    init();
}
inline TextArea::Private *TextArea::d_func()
{
    return static_cast<Private *>(AbstractAreaBase::d_func());
}
inline const TextArea::Private *TextArea::d_func() const
{
    return static_cast<const Private *>(AbstractAreaBase::d_func());
}
}

#endif /* KDCHART_TEXT_AREA_P_H */
