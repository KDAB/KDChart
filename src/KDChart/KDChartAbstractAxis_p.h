/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTAXIS_P_H
#define KDCHARTAXIS_P_H

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

#include <QQueue>
#include <QTimer>

#include "KDChartAbstractArea_p.h"
#include "KDChartAbstractDiagram.h"
#include <KDChartDiagramObserver.h>
#include <KDChartRulerAttributes.h>
#include <KDChartTextAttributes.h>

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class AbstractAxis::Private : public AbstractArea::Private
{
    friend class AbstractAxis;

public:
    Private(AbstractDiagram *diagram, AbstractAxis *axis);
    ~Private() override;

    bool setDiagram(AbstractDiagram *diagram, bool delayedInit = false);
    void unsetDiagram(AbstractDiagram *diagram);
    AbstractDiagram *diagram() const
    {
        return mDiagram;
    }
    bool hasDiagram(AbstractDiagram *diagram) const;

    void updateLayouts();

    DiagramObserver *observer;

    TextAttributes textAttributes;
    RulerAttributes rulerAttributes;
    QStringList hardLabels;
    QStringList hardShortLabels;
    QQueue<AbstractDiagram *> secondaryDiagrams;

protected:
    AbstractDiagram *mDiagram;
    AbstractAxis *mAxis;
};

inline AbstractAxis::AbstractAxis(Private *p, AbstractDiagram *diagram)
    : AbstractArea(p)
{
    Q_UNUSED(diagram);
    init();
    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

inline AbstractAxis::Private *AbstractAxis::d_func()
{
    return static_cast<Private *>(AbstractArea::d_func());
}
inline const AbstractAxis::Private *AbstractAxis::d_func() const
{
    return static_cast<const Private *>(AbstractArea::d_func());
}
}
#endif /* KDCHARTAREA_P_H */
