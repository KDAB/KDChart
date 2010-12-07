#include "KDChartValueTrackerAttributes.h"

#include <KDABLibFakes>
#include <QPen>
#include <QSizeF>
#include <QBrush>

#define d d_func()

using namespace KDChart;

class ValueTrackerAttributes::Private
{
    friend class ValueTrackerAttributes;
    public:
        Private();
    private:
        QPen pen;
        QSizeF markerSize;
        bool enabled;
        QBrush areaBrush;
        Qt::Orientations orientations;
};

ValueTrackerAttributes::Private::Private()
    : pen( QPen( QColor( 80, 80, 80, 200 ) ) ),
      markerSize( QSizeF( 6.0, 6.0 ) ),
      enabled( false ),
      areaBrush( QBrush() ),
      orientations(Qt::Vertical|Qt::Horizontal)
{
}


ValueTrackerAttributes::ValueTrackerAttributes()
    : _d( new Private() )
{
}

ValueTrackerAttributes::ValueTrackerAttributes( const ValueTrackerAttributes& r )
    : _d( new Private( *r.d ) )
{
}

ValueTrackerAttributes & ValueTrackerAttributes::operator=( const ValueTrackerAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

ValueTrackerAttributes::~ValueTrackerAttributes()
{
    delete _d; _d = 0;
}


bool ValueTrackerAttributes::operator==( const ValueTrackerAttributes& r ) const
{
    return ( pen() == r.pen() &&
             areaBrush() == r.areaBrush() &&
             markerSize() == r.markerSize() &&
             isEnabled() == r.isEnabled() );
}

void ValueTrackerAttributes::setPen( const QPen& pen )
{
    d->pen = pen;
}

QPen ValueTrackerAttributes::pen() const
{
    return d->pen;
}

void ValueTrackerAttributes::setAreaBrush( const QBrush& brush )
{
    d->areaBrush = brush;
}

QBrush ValueTrackerAttributes::areaBrush() const
{
    return d->areaBrush;
}

void ValueTrackerAttributes::setMarkerSize( const QSizeF& size )
{
    d->markerSize = size;
}

QSizeF ValueTrackerAttributes::markerSize() const
{
    return d->markerSize;
}

Qt::Orientations ValueTrackerAttributes::orientations() const
{
    return d->orientations;
}

void ValueTrackerAttributes::setOrientations( Qt::Orientations orientations )
{
    d->orientations = orientations;
}

void ValueTrackerAttributes::setEnabled( bool enabled )
{
    d->enabled = enabled;
}

bool ValueTrackerAttributes::isEnabled() const
{
    return d->enabled;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::ValueTrackerAttributes& va)
{
    dbg << "KDChart::ValueTrackerAttributes("
            << "pen="<<va.pen()
            << "markerSize="<<va.markerSize()
            << "enabled="<<va.isEnabled()
            << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
