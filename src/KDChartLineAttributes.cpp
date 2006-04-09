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
    LineType lineType;
    //Areas
    bool displayArea;
    uint transparency;
    //ThreeDLines
    //uint lineXRotation;
    //uint lineYRotation;
};


LineAttributes::Private::Private()
    : lineType ( Normal ),
      displayArea( false ),
      transparency( 255 )
      //lineXRotation( 15 ),
      //lineYRotation( 15 )
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
    if( type() == r.type() &&
        displayArea() == r.displayArea() &&
        transparency() == r.transparency() )
        return true;
    else
        return false;
}



void LineAttributes::setType( const LineType type )
{
   d->lineType = type;
}

LineAttributes::LineType LineAttributes::type() const
{
   return d->lineType;
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

#if 0

void LineAttributes::setLineXRotation( uint degrees )
{
    d->lineXRotation = degrees;
}

uint LineAttributes::lineXRotation() const
{
    return d->lineXRotation;
}

void LineAttributes::setLineYRotation( uint degrees )
{
    d->lineYRotation = degrees;
}

uint LineAttributes::lineYRotation() const
{
    return d->lineYRotation;
}
#endif


QDomDocumentFragment LineAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: LineAttributes::toXML()" );
    return QDomDocumentFragment();
}

