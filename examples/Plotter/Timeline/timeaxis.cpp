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

#include "timeaxis.h"

#include <QtCore/QDateTime>

TimeAxis::TimeAxis(KDChart::AbstractCartesianDiagram *parent)
    : KDChart::CartesianAxis(parent)
{
    // Intentionally left blank
}

const QString TimeAxis::customizedLabel(const QString &label) const
{
    // Here we are free to format the value to whatever we want. As example we
    // could also return a scientific notation with something like the following
    // both lines;
    // const int precision = 2;
    // return QString::number(label.toReal(), 'E', precision);

    // Format the time-value to a nice string representation.
    const QDateTime dateTime = QDateTime::fromSecsSinceEpoch(label.toDouble() * 3600.0);
    return dateTime.date().toString();
}
