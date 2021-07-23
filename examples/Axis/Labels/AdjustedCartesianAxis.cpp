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

#include "AdjustedCartesianAxis.h"

AdjustedCartesianAxis::AdjustedCartesianAxis(KDChart::AbstractCartesianDiagram *diagram)
    : CartesianAxis(diagram)
    , m_lowerBound(0.0)
    , m_upperBound(0.0)
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
