#include "KDChartMarkerAttributes.h"
#include <QColor>
#include <QMap>
#include <QtXml/QDomDocumentFragment>
#include <qglobal.h>

#define d d_func()

using namespace KDChart;

class MarkerAttributes::Private
{
    friend class MarkerAttributes;
public:
    Private();
private:
    bool visible;
    QMap<uint,MarkerStyle> markerStylesMap;
    MarkerStyle markerStyle;
    QSize markerSize;
    QColor markerColor;
};

MarkerAttributes::Private::Private()
{
}


MarkerAttributes::MarkerAttributes()
    : _d( new Private() )
{
    setVisible( false );
    setMarkerStyle( MarkerSquare );
    setMarkerSize( QSize(10,10) );
}

MarkerAttributes::MarkerAttributes( const MarkerAttributes& r )
    : _d( new Private( *r.d ) )
{

}

MarkerAttributes & MarkerAttributes::operator=( const MarkerAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

MarkerAttributes::~MarkerAttributes()
{
    delete _d; _d = 0;
}

bool MarkerAttributes::operator==( const MarkerAttributes& r ) const
{
    return ( isVisible() == r.isVisible() &&
	     markerStylesMap() == r.markerStylesMap() &&
	     markerStyle() == r.markerStyle() &&
	     markerSize() == r.markerSize() &&
	     markerColor() == r.markerColor() );
}



void MarkerAttributes::setVisible( bool visible )
{
    d->visible = visible;
}

bool MarkerAttributes::isVisible() const
{
    return d->visible;
}

void MarkerAttributes::setMarkerStylesMap( MarkerStylesMap map )
{
    d->markerStylesMap = map;
}

MarkerAttributes::MarkerStylesMap MarkerAttributes::markerStylesMap() const
{
    return d->markerStylesMap;
}

void MarkerAttributes::setMarkerStyle( const MarkerStyle style )
{
    d->markerStyle = style;
}

MarkerAttributes::MarkerStyle MarkerAttributes::markerStyle() const
{
    return d->markerStyle;
}

void MarkerAttributes::setMarkerSize( const QSize size )
{
    d->markerSize = size;
}

QSize MarkerAttributes::markerSize() const
{
    return d->markerSize;
}

void MarkerAttributes::setMarkerColor( const QColor color )
{
    d->markerColor = color;
}

QColor MarkerAttributes::markerColor() const
{
    return d->markerColor;
}

QDomDocumentFragment MarkerAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment MarkerAttributes::toXML() const" );
    return QDomDocumentFragment();
}






































