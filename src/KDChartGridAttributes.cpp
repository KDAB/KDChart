#include "KDChartGridAttributes.h"

#include <QPen>
#include <QDomDocumentFragment>

#define d d_func()

using namespace KDChart;

class GridAttributes::Private
{
    friend class GridAttributes;
public:
    Private();
private:
    bool visible;
    QPen pen;
    bool subVisible;
    QPen subPen;
    QPen zeroPen;
};

GridAttributes::Private::Private()
    : visible( true ),
      pen( Qt::blue),
      subVisible( true ),
      subPen( Qt::lightGray ),
      zeroPen( QColor( 0x00, 0x00, 0x80 ) )
{
}


GridAttributes::GridAttributes()
    : _d( new Private() )
{
}

GridAttributes::GridAttributes( const GridAttributes& r )
    : _d( new Private( *r.d ) )
{
}

GridAttributes & GridAttributes::operator=( const GridAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

GridAttributes::~GridAttributes()
{
    delete _d; _d = 0;
}


bool GridAttributes::operator==( const GridAttributes& r ) const
{
    return ( isGridVisible() == r.isGridVisible() &&
	     gridPen() == r.gridPen() &&
	     isSubGridVisible() == r.isSubGridVisible() &&
	     subGridPen() == r.subGridPen() &&
	     zeroLinePen() == r.zeroLinePen() );
}


void GridAttributes::setGridVisible( bool visible )
{
    d->visible = visible;
}

bool GridAttributes::isGridVisible() const
{
    return d->visible;
}

void GridAttributes::setGridPen( const QPen & pen )
{
    d->pen = pen;
}

QPen GridAttributes::gridPen() const
{
    return d->pen;
}

void GridAttributes::setSubGridVisible( bool visible )
{
    d->subVisible = visible;
}

bool GridAttributes::isSubGridVisible() const
{
    return d->subVisible;
}

void GridAttributes::setSubGridPen( const QPen & pen )
{
    d->subPen = pen;
}

QPen GridAttributes::subGridPen() const
{
    return d->subPen;
}

void GridAttributes::setZeroLinePen( const QPen & pen )
{
    d->zeroPen = pen;
}

QPen GridAttributes::zeroLinePen() const
{
    return d->zeroPen;
}

QDomDocumentFragment GridAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: GridAttributes::toXML()" );
    return QDomDocumentFragment();
}
