/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTMEASURE_H
#define KDCHARTMEASURE_H

#include <QDebug>
#include <Qt>
#include "KDChartGlobal.h"
#include "KDChartEnums.h"

class QDomDocumentFragment;

namespace KDChart {

    class AbstractArea;


class KDCHART_EXPORT Measure
{
public:
    Measure();
    Measure( qreal value, KDChartEnums::MeasureCalculationMode mode = KDChartEnums::MeasureCalculationModeAuto );
    Measure( const Measure& );
    Measure &operator= ( const Measure& );

    ~Measure();

    QDomDocumentFragment toXML() const;

    void setValue( qreal value );
    qreal value() const;

    void setCalculationMode( KDChartEnums::MeasureCalculationMode mode );
    KDChartEnums::MeasureCalculationMode calculationMode() const;

    void setReferenceArea( AbstractArea * area );
    AbstractArea * referenceArea() const;

    void setReferenceOrientation( Qt::Orientation orientation );
    Qt::Orientation referenceOrientation() const;

    bool operator==( const Measure& );

private:
    qreal m_value;
    KDChartEnums::MeasureCalculationMode m_mode;
    AbstractArea*   m_area;
    Qt::Orientation m_orientation;

}; // End of class Measure

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::Measure& );
#endif /* QT_NO_DEBUG_STREAM */

#endif // KDCHARTMEASURE_H
