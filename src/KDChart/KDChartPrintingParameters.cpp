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

#include "KDChartPrintingParameters.h"

using namespace KDChart;

PrintingParameters::PrintingParameters()
    : scaleFactor(1.0)
{
}

PrintingParameters *PrintingParameters::instance()
{
    static PrintingParameters instance;
    return &instance;
}

void PrintingParameters::setScaleFactor(const qreal scaleFactor)
{
    instance()->scaleFactor = scaleFactor;
}

void PrintingParameters::resetScaleFactor()
{
    instance()->scaleFactor = 1.0;
}

QPen PrintingParameters::scalePen(const QPen &pen)
{
    if (instance()->scaleFactor == 1.0)
        return pen;

    QPen resultPen = pen;
    resultPen.setWidthF(resultPen.widthF() * instance()->scaleFactor);
    if (resultPen.widthF() == 0.0)
        resultPen.setWidthF(instance()->scaleFactor);

    return resultPen;
}
