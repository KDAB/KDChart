#include "KDChartThreeDAttributes.h"
#include "KDChartThreeDAttributes_p.h"
#include <qglobal.h>

#define d d_func()

using namespace KDChart;


ThreeDAttributes::Private::Private()
    : enabled( false ),
      height( 20 ),
      depth( 20 )
{
}


ThreeDAttributes::ThreeDAttributes()
    : _d( new Private() )
{
}

ThreeDAttributes::ThreeDAttributes( const ThreeDAttributes& r )
    : _d( new Private( *r.d ) )
{
}

ThreeDAttributes& ThreeDAttributes::operator= ( const ThreeDAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

ThreeDAttributes::~ThreeDAttributes()
{
    delete _d; _d = 0;
}


bool ThreeDAttributes::operator==( const ThreeDAttributes& r ) const
{
    if( isEnabled() == r.isEnabled() &&
        height() == r.height() &&
        depth() == r.depth() )
        return true;
    else
        return false;
}


void ThreeDAttributes::init( )
{

}

void ThreeDAttributes::setEnabled( bool enabled )
{
    d->enabled = enabled;
}

bool ThreeDAttributes::isEnabled() const
{
    return d->enabled;
}

void ThreeDAttributes::setHeight( int pixels )
{
    d->height = pixels;
}

int ThreeDAttributes::height() const
{
    return d->height;
}

void ThreeDAttributes::setDepth( int depth )
{
    d->depth = depth;
}


int ThreeDAttributes::depth() const
{
    return d->depth;
}


QDomDocumentFragment ThreeDAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: ThreeDAttributes::toXML()" );
    return QDomDocumentFragment();
}

