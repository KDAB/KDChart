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

#ifndef ADJUSTED_CARTESIAN_AXIS_H
#define ADJUSTED_CARTESIAN_AXIS_H

#include <KDChartCartesianAxis>
#include <KDChartGlobal>


class AdjustedCartesianAxis: public KDChart::CartesianAxis
{
    Q_OBJECT

    Q_DISABLE_COPY( AdjustedCartesianAxis )

public:
    explicit AdjustedCartesianAxis( KDChart::AbstractCartesianDiagram* diagram = 0 );

    virtual const QString customizedLabel( const QString& label ) const;

    void setBounds( qreal lower, qreal upper ) {
        m_lowerBound = lower;
        m_upperBound = upper;
    }
    qreal lowerBound() const { return m_lowerBound; }
    qreal upperBound() const { return m_upperBound; }

private:
    qreal m_lowerBound;
    qreal m_upperBound;
};

#endif // ADJUSTED_CARTESIAN_AXIS_H
