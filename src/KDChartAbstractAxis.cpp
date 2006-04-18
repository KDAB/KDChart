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

#include "KDChartAbstractAxis.h"
#include "KDChartAbstractAxis_p.h"
#include <QDebug>
#include <KDChartAbstractDiagram.h>

using namespace KDChart;


AbstractAxis::Private::Private()
{
    // PENDING(miroslav) Code from KDChartAxis::Private::Private goes here
}


AbstractAxis::Private::~Private()
{
    // PENDING(miroslav) Code from KDChartAxis::Private::~Private goes here
}


AbstractAxis::AbstractAxis ( AbstractDiagram* parent )
    : AbstractArea( new Private(), parent )
{   // FIXME decide internal structure, how to save parent
    init();
}

AbstractAxis::~AbstractAxis()
{
    emit destroyedAxis( this );
}


void AbstractAxis::init()
{
}
