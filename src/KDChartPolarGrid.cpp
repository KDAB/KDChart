/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "KDChartPolarGrid.h"
#include "KDChartPaintContext.h"

#include <KDABLibFakes>

using namespace KDChart;


DataDimensionsList PolarGrid::calculateGrid(
    const DataDimensionsList& rawDataDimensions ) const
{
    qDebug("Calling PolarGrid::calculateGrid()");
    DataDimensionsList l;

    //FIXME(khz): do the real calculation

    l = rawDataDimensions;

    return l;
}


void PolarGrid::drawGrid( PaintContext* context )
{
    qDebug("Calling PolarGrid::drawGrid()");
}
