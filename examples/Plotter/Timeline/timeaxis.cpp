/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "timeaxis.h"

#include <QtCore/QDateTime>

TimeAxis::TimeAxis( KDChart::AbstractCartesianDiagram* parent )
    : KDChart::CartesianAxis( parent )
{
    // Intentionally left blank
}

const QString TimeAxis::customizedLabel( const QString& label ) const
{
    // Here we are free to format the value to whatever we want. As example we
    // could also return a scientific notation with something like the following
    // both lines;
    //const int precision = 2;
    //return QString::number(label.toReal(), 'E', precision);

    // Format the time-value to a nice string representation.
    const QDateTime dateTime = QDateTime::fromTime_t( label.toDouble() * 3600.0 );
    return dateTime.date().toString();
}
