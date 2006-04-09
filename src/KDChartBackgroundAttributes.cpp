#include "KDChartBackgroundAttributes.h"
#include <QPixmap>
#include <QtXml/QDomDocumentFragment>

#define d d_func()

using namespace KDChart;

class BackgroundAttributes::Private
{
    friend class KDChart::BackgroundAttributes;
public:
    Private();
private:
    bool visible;
    QBrush brush;
    BackgroundPixmapMode pixmapMode;
    QPixmap pixmap;
};

BackgroundAttributes::Private::Private() :
    visible( false ),
    pixmapMode( BackgroundAttributes::BackgroundPixmapModeNone )
{
}


BackgroundAttributes::BackgroundAttributes()
    : _d( new Private() )
{
}

BackgroundAttributes::BackgroundAttributes( const BackgroundAttributes& r )
    : _d( new Private( *r.d ) )
{
}

BackgroundAttributes & BackgroundAttributes::operator=( const BackgroundAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

bool BackgroundAttributes::operator==( const BackgroundAttributes& r ) const
{
    return ( isVisible() == r.isVisible() &&
	     brush() == r.brush() &&
	     pixmapMode() == r.pixmapMode() &&
	     pixmap().serialNumber() == r.pixmap().serialNumber() );
}


BackgroundAttributes::~BackgroundAttributes()
{
    delete _d; _d = 0;
}




void BackgroundAttributes::setVisible( bool visible )
{
    d->visible = visible;
}


bool BackgroundAttributes::isVisible() const
{
    return d->visible;
    return false;
}

void BackgroundAttributes::setBrush( const QBrush &brush )
{
    d->brush = brush;
}

QBrush BackgroundAttributes::brush() const
{
    return d->brush;
}

void BackgroundAttributes::setPixmapMode( BackgroundPixmapMode mode )
{
    d->pixmapMode = mode;
}

BackgroundAttributes::BackgroundPixmapMode BackgroundAttributes::pixmapMode() const
{
    return d->pixmapMode;
}

void BackgroundAttributes::setPixmap( const QPixmap &backPixmap )
{
    d->pixmap = backPixmap;
}

QPixmap BackgroundAttributes::pixmap() const
{
    return d->pixmap;
}

QDomDocumentFragment BackgroundAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment BackgroundAttributes::toXML() const" );
    return QDomDocumentFragment();
}


















