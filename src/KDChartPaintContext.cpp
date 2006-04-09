#include <QRectF>
#include <QPainter>

#include "KDChartPaintContext.h"
#include "KDChartCoordinatePlane.h"

using namespace KDChart;

#define d (d_func())

class PaintContext::Private {

public:
    QPainter* painter;
    QRectF rect;
    CoordinatePlane* plane;

    Private()
        : painter( 0 )
        , plane ( 0 )
    {}
};

PaintContext::PaintContext()
    : _d ( new Private() )
{
}

const QRectF PaintContext::rectangle() const
{
    return d->rect;
}

void PaintContext::setRectangle ( const QRectF& rect )
{
    d->rect = rect;
}

QPainter* PaintContext::painter() const
{
    return d->painter;
}

void PaintContext::setPainter( QPainter* painter )
{
    d->painter = painter;
}

CoordinatePlane* PaintContext::coordinatePlane() const
{
    return d->plane;
}

void PaintContext::setCoordinatePlane( CoordinatePlane* plane)
{
    d->plane = plane;
}
