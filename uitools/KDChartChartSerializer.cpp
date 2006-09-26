
/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "KDChartChartSerializer.h"

#include <QDebug>

using namespace KDChart;

ChartSerializer::ChartSerializer( )
{
}

ChartSerializer::~ChartSerializer( )
{
}

bool ChartSerializer::saveExtraInfo( const Chart * chart, DomWidget * ui_widget )
{
    Q_UNUSED( chart)
    Q_UNUSED( ui_widget )
    return true;
}


bool ChartSerializer::loadExtraInfo( Chart * chart, const DomWidget * ui_widget )
{
    Q_UNUSED( chart)
    Q_UNUSED( ui_widget )
    return true;
}

