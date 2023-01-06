/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "AdjustedCartesianAxis.h"

AdjustedCartesianAxis::AdjustedCartesianAxis(KDChart::AbstractCartesianDiagram *diagram)
    : CartesianAxis(diagram)
{
    // this block left empty intentionally
}

const QString AdjustedCartesianAxis::customizedLabel(const QString &label) const
{
    bool bOK;
    qreal val = label.toDouble(&bOK);
    if (bOK) {
        if (val < m_lowerBound)
            return tr("LOW");
        else if (val > m_upperBound)
            return tr("HIGH");
    }
    return label;
}
