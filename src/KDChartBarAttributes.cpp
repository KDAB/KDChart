#include "KDChartBarAttributes.h"
#include <qglobal.h>

#define d d_func()

using namespace KDChart;

class BarAttributes::Private
{
    friend class ::BarAttributes;
public:
    Private();

private:
    double datasetGap;
    bool useFixedDatasetGap;
    double valueBlockGap;
    bool useFixedValueBlockGap;
    double barWidth;
    bool useFixedBarWidth;
    bool drawSolidExcessArrows;
    double groupGapFactor;
    double barGapFactor;
};


BarAttributes::Private::Private()
    :datasetGap( 6 ),
    useFixedDatasetGap( false ),
    valueBlockGap( 24 ),
    useFixedValueBlockGap( false ),
    barWidth( -1 ),
    useFixedBarWidth( false ),
    drawSolidExcessArrows( false ),
    groupGapFactor( 1.0 ),
    barGapFactor( 0.5 )
{
}


BarAttributes::BarAttributes()
    : _d( new Private() )
{
}

BarAttributes::BarAttributes( const BarAttributes& r )
    : _d( new Private( *r.d ) )
{
}

BarAttributes& BarAttributes::operator= ( const BarAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

BarAttributes::~BarAttributes()
{
    delete _d; _d = 0;
}


bool BarAttributes::operator==( const BarAttributes& r ) const
{
    if( fixedDataValueGap() == r.fixedDataValueGap() &&
        useFixedDataValueGap() == r.useFixedDataValueGap() &&
        fixedValueBlockGap() == r.fixedValueBlockGap() &&
        useFixedValueBlockGap() == r.useFixedValueBlockGap() &&
        fixedBarWidth() == r.fixedBarWidth() &&
        useFixedBarWidth() == r.useFixedBarWidth() &&
        groupGapFactor() == r.groupGapFactor() &&
        barGapFactor() == r.barGapFactor() &&
        drawSolidExcessArrows() == r.drawSolidExcessArrows() )
        return true;
    else
        return false;
}


void BarAttributes::setFixedDataValueGap( double gap )
{
    d->datasetGap = gap;
}

double BarAttributes::fixedDataValueGap() const
{
    return d->datasetGap;
}

void BarAttributes::setUseFixedDataValueGap( bool gapIsFixed )
{
    d->useFixedDatasetGap = gapIsFixed;
}

bool BarAttributes::useFixedDataValueGap() const
{
    return d->useFixedDatasetGap;
}

void BarAttributes::setFixedValueBlockGap( double gap )
{
    d->valueBlockGap = gap;
}

double BarAttributes::fixedValueBlockGap() const
{
    return d->valueBlockGap;
}

void BarAttributes::setUseFixedValueBlockGap( bool gapIsFixed )
{
    d->useFixedValueBlockGap = gapIsFixed;
}

bool BarAttributes::useFixedValueBlockGap() const
{
    return d->useFixedValueBlockGap;
}

void BarAttributes::setFixedBarWidth( double width )
{
    d->barWidth = width;
}

double BarAttributes::fixedBarWidth() const
{

    return d->barWidth;
}

void BarAttributes::setUseFixedBarWidth( bool useFixedBarWidth )
{
    d->useFixedBarWidth = useFixedBarWidth;
}

bool BarAttributes::useFixedBarWidth() const
{
    return d->useFixedBarWidth;
}

void BarAttributes::setGroupGapFactor( double gapFactor )
{
    d->groupGapFactor = gapFactor;
}

double BarAttributes::groupGapFactor() const
{
    return d->groupGapFactor;
}

void BarAttributes::setBarGapFactor( double gapFactor )
{
    d->barGapFactor = gapFactor;
}

double BarAttributes::barGapFactor() const
{
    return d->barGapFactor;
}

void BarAttributes::setDrawSolidExcessArrows( bool solidArrows )
{
    d->drawSolidExcessArrows = solidArrows;
}

bool BarAttributes::drawSolidExcessArrows() const
{
    return d->drawSolidExcessArrows;
}

QDomDocumentFragment BarAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: BarAttributes::toXML()" );
    return QDomDocumentFragment();
}

