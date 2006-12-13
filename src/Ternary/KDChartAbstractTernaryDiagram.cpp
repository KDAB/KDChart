#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"

#include "KDChartAbstractTernaryDiagram_p.h"

using namespace KDChart;

AbstractTernaryDiagram::Private::Private()
    : AbstractDiagram::Private()
{
}

void AbstractTernaryDiagram::init()
{
}

#define d d_func()

AbstractTernaryDiagram::AbstractTernaryDiagram( QWidget* parent,
                                                TernaryCoordinatePlane* plane )
    : AbstractDiagram( parent, plane )
{
}

AbstractTernaryDiagram::~AbstractTernaryDiagram()
{
}

void AbstractTernaryDiagram::addAxis( TernaryAxis* axis )
{
    d->axesList.append( axis );
    // FIXME update
}

void AbstractTernaryDiagram::takeAxis( TernaryAxis* axis )
{

    int index = d->axesList.indexOf( axis );
    if ( index != -1 )
        d->axesList.removeAt( index );
    // FIXME update
}

TernaryAxisList AbstractTernaryDiagram::axes() const
{
    return d->axesList;
}

void AbstractTernaryDiagram::paint (PaintContext *paintContext)
{
    Q_FOREACH( TernaryAxis* axis, axes() ) {
        qDebug() << "AbstractTernaryDiagram::paint: painting" << axis;
        PainterSaver s( paintContext->painter() );
        axis->paint( paintContext->painter() );
    }
}

