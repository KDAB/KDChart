#include "KDChartLineAttributes.h"
#include <qglobal.h>

#define d d_func()

using namespace KDChart;

class LineAttributes::Private
{
    friend class ::LineAttributes;
public:
    Private();

private:
    //Areas
    bool displayArea;
    uint transparency;
};


LineAttributes::Private::Private()
    : displayArea( false ),
      transparency( 255 )
{
}


LineAttributes::LineAttributes()
    : _d( new Private() )
{
}

LineAttributes::LineAttributes( const LineAttributes& r )
    : _d( new Private( *r.d ) )
{
}

LineAttributes& LineAttributes::operator= ( const LineAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

LineAttributes::~LineAttributes()
{
    delete _d; _d = 0;
}

bool LineAttributes::operator==( const LineAttributes& r ) const
{
    if( displayArea() == r.displayArea() &&
        transparency() == r.transparency() )
        return true;
    else
        return false;
}

void LineAttributes::setDisplayArea( bool display )
{
    d->displayArea = display;
}

bool LineAttributes::displayArea() const
{
   return d->displayArea;
}

void LineAttributes::setTransparency( uint alpha )
{
     if ( alpha > 255 )
        alpha = 255;
    d->transparency = alpha;
}

uint LineAttributes::transparency() const
{
     return d->transparency;
}

QDomDocumentFragment LineAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: LineAttributes::toXML()" );
    return QDomDocumentFragment();
}

