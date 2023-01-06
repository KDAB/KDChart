/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartPrintingParameters.h"

using namespace KDChart;

PrintingParameters::PrintingParameters()
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
