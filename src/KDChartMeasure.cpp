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

