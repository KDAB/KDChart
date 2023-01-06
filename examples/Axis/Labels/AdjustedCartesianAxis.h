/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef ADJUSTED_CARTESIAN_AXIS_H
#define ADJUSTED_CARTESIAN_AXIS_H

#include <KDChartCartesianAxis>
#include <KDChartGlobal>

class AdjustedCartesianAxis : public KDChart::CartesianAxis
{
    Q_OBJECT

    Q_DISABLE_COPY(AdjustedCartesianAxis)

public:
    explicit AdjustedCartesianAxis(KDChart::AbstractCartesianDiagram *diagram = nullptr);

    const QString customizedLabel(const QString &label) const override;

    void setBounds(qreal lower, qreal upper)
    {
        m_lowerBound = lower;
        m_upperBound = upper;
    }
    qreal lowerBound() const
    {
        return m_lowerBound;
    }
    qreal upperBound() const
    {
        return m_upperBound;
    }

private:
    qreal m_lowerBound = 0.0;
    qreal m_upperBound = 0.0;
};

#endif // ADJUSTED_CARTESIAN_AXIS_H
