/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef TIMEAXIS_H
#define TIMEAXIS_H

#include <KDChartCartesianAxis>

class TimeAxis : public KDChart::CartesianAxis
{
    Q_OBJECT
public:
    explicit TimeAxis(KDChart::AbstractCartesianDiagram *parent);
    const QString customizedLabel(const QString &label) const override;
};

#endif
