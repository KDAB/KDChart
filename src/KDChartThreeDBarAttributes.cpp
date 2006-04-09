#include "KDChartThreeDBarAttributes.h"
#include "KDChartThreeDBarAttributes_p.h"
#include <qglobal.h>

#define d d_func()

using namespace KDChart;

ThreeDBarAttributes::Private::Private()
    : useShadowColors( true ),
      angle( 45 )
{
}


ThreeDBarAttributes::ThreeDBarAttributes()
    : ThreeDAttributes( new Private() )
{

}

ThreeDBarAttributes::ThreeDBarAttributes( const ThreeDBarAttributes& r )
    : ThreeDAttributes( new Private( *r.d) )
{
}

ThreeDBarAttributes& ThreeDBarAttributes::operator= ( const ThreeDBarAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

ThreeDBarAttributes::~ThreeDBarAttributes()
{
}

void ThreeDBarAttributes::init()
{
}


bool ThreeDBarAttributes::operator==( const ThreeDBarAttributes& r ) const
{
    return ( useShadowColors() == r.useShadowColors() &&
             angle() == r.angle() &&
             ThreeDAttributes::operator==(r));
}



void ThreeDBarAttributes::setUseShadowColors( bool shadowColors )
{
    d->useShadowColors = shadowColors;
}

bool ThreeDBarAttributes::useShadowColors() const
{
    return d->useShadowColors;
}

void ThreeDBarAttributes::setAngle( uint threeDAngle )
{
    d->angle = threeDAngle;
}

uint ThreeDBarAttributes::angle() const
{
    return d->angle;
}


QDomDocumentFragment ThreeDBarAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: ThreeDBarAttribute::toXML()" );
    return QDomDocumentFragment();
}


