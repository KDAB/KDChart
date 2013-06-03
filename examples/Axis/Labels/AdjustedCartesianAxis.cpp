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

#include "AdjustedCartesianAxis.h"


AdjustedCartesianAxis::AdjustedCartesianAxis ( KDChart::AbstractCartesianDiagram* diagram )
    : CartesianAxis ( diagram )
    , m_lowerBound(0.0)
    , m_upperBound(0.0)
{
    // this bloc left empty intentionally
}

const QString AdjustedCartesianAxis::customizedLabel( const QString& label ) const
{
    bool bOK;
    qreal val = label.toDouble( & bOK );
    if ( bOK ) {
        if ( val < m_lowerBound )
            return tr("LOW");
        else if ( val > m_upperBound )
            return tr("HIGH");
    }
    return label;
}
