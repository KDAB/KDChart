#include "KDChartAbstractThreeDAttributes.h"
#include "KDChartAbstractThreeDAttributes_p.h"

#include <QDebug>

#include <KDABLibFakes>

#define d d_func()


using namespace KDChart;


AbstractThreeDAttributes::Private::Private()
    : enabled( false ),
      depth( 20 )
{
}


AbstractThreeDAttributes::AbstractThreeDAttributes()
    : _d( new Private() )
{
}

AbstractThreeDAttributes::AbstractThreeDAttributes( const AbstractThreeDAttributes& r )
    : _d( new Private( *r.d ) )
{
}

AbstractThreeDAttributes& AbstractThreeDAttributes::operator= ( const AbstractThreeDAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

AbstractThreeDAttributes::~AbstractThreeDAttributes()
{
    delete _d; _d = 0;
}


bool AbstractThreeDAttributes::operator==( const AbstractThreeDAttributes& r ) const
{
    if( isEnabled() == r.isEnabled() &&
        depth() == r.depth() )
        return true;
    else
        return false;
}


void AbstractThreeDAttributes::init( )
{

}

void AbstractThreeDAttributes::setEnabled( bool enabled )
{
    d->enabled = enabled;
}

bool AbstractThreeDAttributes::isEnabled() const
{
    return d->enabled;
}

void AbstractThreeDAttributes::setDepth( double depth )
{
    d->depth = depth;
}


double AbstractThreeDAttributes::depth() const
{
    return d->depth;
}


double AbstractThreeDAttributes::validDepth() const
{
    return isEnabled() ? d->depth : 0.0;
}


#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::AbstractThreeDAttributes& a)
{
    dbg << "enabled="<<a.isEnabled()
        << "depth="<<a.depth();
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
