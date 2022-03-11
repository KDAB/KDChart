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

#ifndef KDCHARTLEVEYJENNINGSDIAGRAM_P_H
#define KDCHARTLEVEYJENNINGSDIAGRAM_P_H

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

#include <QDateTime>

#include "KDChartThreeDLineAttributes.h"
#include "KDChartLineDiagram_p.h"

#include <KDABLibFakes>

QT_BEGIN_NAMESPACE
class QSvgRenderer;
QT_END_NAMESPACE

namespace KDChart {

class PaintContext;

/**
 * \internal
 */
class LeveyJenningsDiagram::Private : public LineDiagram::Private
{
    friend class LeveyJenningsDiagram;

public:
    Private();
    Private(const Private &rhs);
    ~Private() override;

    void setYAxisRange() const;

    Qt::Alignment lotChangedPosition;
    Qt::Alignment fluidicsPackChangedPosition;
    Qt::Alignment sensorChangedPosition;

    QVector<QDateTime> fluidicsPackChanges;
    QVector<QDateTime> sensorChanges;

    QPen scanLinePen;

    QMap<LeveyJenningsDiagram::Symbol, QString> icons;
    QMap<LeveyJenningsDiagram::Symbol, QSvgRenderer *> iconRenderer;

    QPair<QDateTime, QDateTime> timeRange;

    float expectedMeanValue;
    float expectedStandardDeviation;

    mutable float calculatedMeanValue;
    mutable float calculatedStandardDeviation;
};

KDCHART_IMPL_DERIVED_DIAGRAM(LeveyJenningsDiagram, LineDiagram, LeveyJenningsCoordinatePlane)
}

#endif /* KDCHARTLINEDIAGRAM_P_H */
