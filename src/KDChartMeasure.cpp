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

#include "KDChartMeasure.h"
#include <QtXml/QDomDocumentFragment>
#include <KDChartTextAttributes.h>
#include <KDChartFrameAttributes.h>
#include <KDChartBackgroundAttributes.h>


namespace KDChart {


Measure::Measure()
  : m_value( 0 ),
    m_mode(  KDChartEnums::MeasureCalculationModeAuto ),
    m_area(  0 ),
    m_orientation( Qt::Horizontal )
{
    // this bloc left empty intentionally
}

Measure::Measure( qreal value, KDChartEnums::MeasureCalculationMode mode )
  : m_value( value ),
    m_mode(  mode ),
    m_area(  0 ),
    m_orientation( Qt::Horizontal )
{
    // this bloc left empty intentionally
}

Measure::Measure( const Measure& r )
  : m_value( r.value() ),
    m_mode(  r.calculationMode() ),
    m_area(  r.referenceArea() ),
    m_orientation( r.referenceOrientation() )
{
    // this bloc left empty intentionally
}

Measure & Measure::operator=( const Measure& r )
{
    if( this != &r ){
        m_value = r.value();
        m_mode  = r.calculationMode();
        m_area  = r.referenceArea();
        m_orientation = r.referenceOrientation();
    }

    return *this;
}

Measure::~Measure()
{
    // this bloc left empty intentionally
}


bool Measure::operator==( const Measure& r )
{
    return( m_value == r.value() &&
            m_mode  == r.calculationMode() &&
            m_area  == r.referenceArea() &&
            m_orientation == r.referenceOrientation() );
}



QDomDocumentFragment Measure::toXML() const
{
    return QDomDocumentFragment();
}


}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::Measure& m)
{
    dbg << "KDChart::Measure("
	<< "value="<<m.value()
	<< "calculationmode="<<m.calculationMode()
	<< "referencearea="<<m.referenceArea()
	<< "referenceorientation="<<m.referenceOrientation()
	<< ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
