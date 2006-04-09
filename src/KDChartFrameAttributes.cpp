#include "KDChartFrameAttributes.h"
#include <QtXml/QDomDocumentFragment>

#define d d_func()

using namespace KDChart;

class FrameAttributes::Private
{
    friend class FrameAttributes;
public:
    Private();
private:
    bool visible;
    QPen pen;
    int padding;
};

FrameAttributes::Private::Private() :
    visible( false ),
    padding( 0 )
{
}


FrameAttributes::FrameAttributes()
    : _d( new Private() )
{
}

FrameAttributes::FrameAttributes( const FrameAttributes& r )
    : _d( new Private( *r.d ) )
{
}

FrameAttributes & FrameAttributes::operator=( const FrameAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

FrameAttributes::~FrameAttributes()
{
    delete _d; _d = 0;
}


bool FrameAttributes::operator==( const FrameAttributes& r ) const
{
    return ( isVisible() == r.isVisible() &&
	     pen() == r.pen() &&
	     padding() == r.padding() );
}




void FrameAttributes::setVisible( bool visible )
{
    d->visible = visible;
}

bool FrameAttributes::isVisible() const
{
    return d->visible;
}

void FrameAttributes::setPen( const QPen & pen )
{
    d->pen = pen;
}

QPen FrameAttributes::pen() const
{
    return d->pen;
}

void FrameAttributes::setPadding( int padding )
{
    d->padding = padding;
}

int FrameAttributes::padding() const
{
    return d->padding;
}

QDomDocumentFragment FrameAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment FrameAttributes::toXML() const" );
    return QDomDocumentFragment();
}















