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

#define d d_func()

class KDChartMeasure::Private
{
    friend class ::KDChartMeasure;
public:
    Private();
private:
    int value;
    KDChartEnums::MeasureCalculationMode mode;
    AbstractArea* area;
    Qt::Orientation orientation;
};

KDChartMeasure::Private::Private() :
    value( 0 ),
    mode( KDChartEnums::MeasureCalculationModeAuto ),
    area(  0 ),
    orientation( Qt::Horizontal )
{
}


KDChartMeasure::KDChartMeasure()
    : _d( new Private() )
{
}

KDChartMeasure::KDChartMeasure( int value, KDChartEnums::MeasureCalculationMode mode )
    : _d( new Private() )
{
    _d->value = value;
    _d->mode = mode;
}

KDChartMeasure::KDChartMeasure( const KDChartMeasure& r )
    : _d( new Private( *r.d ) )
{
}

KDChartMeasure & KDChartMeasure::operator=( const KDChartMeasure& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

KDChartMeasure::~KDChartMeasure()
{
    delete _d; _d = 0;
}


bool KDChartMeasure::operator==( const KDChartMeasure& r )
{
    if( value() == r.value() &&
        calculationMode() == r.calculationMode() &&
        referenceArea() == r.referenceArea() &&
        referenceOrientation() == r.referenceOrientation() )
        return true;
    else
        return false;
}




QDomDocumentFragment KDChartMeasure::toXML() const
{
    return QDomDocumentFragment();
}

void KDChartMeasure::setValue( int value )
{
    d->value = value;
}

int KDChartMeasure::value() const
{
    return d->value;
}

void KDChartMeasure::setCalculationMode( KDChartEnums::MeasureCalculationMode mode )
{
    d->mode = mode;
}

KDChartEnums::MeasureCalculationMode KDChartMeasure::calculationMode() const
{
    return d->mode;
}

void KDChartMeasure::setReferenceArea( AbstractArea * area )
{
    d->area = area;
}

AbstractArea * KDChartMeasure::referenceArea() const
{
    return d->area;
}

void KDChartMeasure::setReferenceOrientation( Qt::Orientation orientation )
{
    d->orientation = orientation;
}

Qt::Orientation KDChartMeasure::referenceOrientation() const
{
    return d->orientation;
}

