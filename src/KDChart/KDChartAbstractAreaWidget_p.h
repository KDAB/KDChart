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

#ifndef KDCHARTABSTRACTAREAWIDGET_P_H
#define KDCHARTABSTRACTAREAWIDGET_P_H

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

/** \file KDChartAbstractAreaWidget_p.h
 *  \internal
 */

#include "KDChartAbstractAreaWidget.h"
#include "KDChartAbstractAreaBase_p.h"

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class AbstractAreaWidget::Private : public AbstractAreaBase::Private
{
    friend class AbstractAreaWidget;

public:
    explicit Private();
    ~Private() override;

    Private(const Private &rhs)
        : AbstractAreaBase::Private(rhs)
        , currentLayoutSize()
    {
        // Just for consistency
    }

    QSize currentLayoutSize;

    // non-virtual method, calling widget->resizeLayout( size )
    void resizeLayout(AbstractAreaWidget *widget, const QSize &sz);
};

inline AbstractAreaWidget::AbstractAreaWidget(AbstractAreaWidget::Private *p, QWidget *parent)
    : QWidget(parent)
    , AbstractAreaBase(p)
{
    init();
}
inline AbstractAreaWidget::Private *AbstractAreaWidget::d_func()
{
    return static_cast<Private *>(AbstractAreaBase::d_func());
}
inline const AbstractAreaWidget::Private *AbstractAreaWidget::d_func() const
{
    return static_cast<const Private *>(AbstractAreaBase::d_func());
}

}

#endif /* KDCHARTABSTRACTAREAWIDGET_P_H */
