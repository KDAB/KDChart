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


#define d d_func()

class Measure::Private
{
    friend class Measure;
public:
    Private();
private:
    double value;
    KDChartEnums::MeasureCalculationMode mode;
    AbstractArea* area;
    Qt::Orientation orientation;
};

Measure::Private::Private() :
    value( 0 ),
    mode( KDChartEnums::MeasureCalculationModeAuto ),
    area(  0 ),
    orientation( Qt::Horizontal )
{
}


Measure::Measure()
    : _d( new Private() )
{
}

Measure::Measure( double value, KDChartEnums::MeasureCalculationMode mode )
    : _d( new Private() )
{
    _d->value = value;
    _d->mode = mode;
}

Measure::Measure( const Measure& r )
    : _d( new Private( *r.d ) )
{
}

Measure & Measure::operator=( const Measure& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

Measure::~Measure()
{
    delete _d; _d = 0;
}


bool Measure::operator==( const Measure& r )
{
    if( value() == r.value() &&
        calculationMode() == r.calculationMode() &&
        referenceArea() == r.referenceArea() &&
        referenceOrientation() == r.referenceOrientation() )
        return true;
    else
        return false;
}




QDomDocumentFragment Measure::toXML() const
{
    return QDomDocumentFragment();
}

void Measure::setValue( double value )
{
    d->value = value;
}

double Measure::value() const
{
    return d->value;
}

void Measure::setCalculationMode( KDChartEnums::MeasureCalculationMode mode )
{
    d->mode = mode;
}

KDChartEnums::MeasureCalculationMode Measure::calculationMode() const
{
    return d->mode;
}

void Measure::setReferenceArea( AbstractArea * area )
{
    d->area = area;
}

AbstractArea * Measure::referenceArea() const
{
    return d->area;
}

void Measure::setReferenceOrientation( Qt::Orientation orientation )
{
    d->orientation = orientation;
}

Qt::Orientation Measure::referenceOrientation() const
{
    return d->orientation;
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
