#include "KDChartAbstractCoordinatePlane.h"
#include "KDChartGridAttributes.h"

using namespace KDChart;

class CoordinatePlane::Private
{
public:
    AbstractDiagramList diagrams;
    GridAttributes gridAttributes;
};

CoordinatePlane::CoordinatePlane ( QWidget* parent )
    : QWidget ( parent )
    , d ( new Private() )
{
}

CoordinatePlane::~CoordinatePlane()
{
}

void CoordinatePlane::addDiagram ( AbstractDiagram* diagram )
{
    // diagrams are invisible and paint through their paint() method
    diagram->hide();

    d->diagrams.append ( diagram );
    diagram->setParent ( this );
    diagram->setCoordinatePlane( this );
    layoutDiagrams();
}


/*virtual*/
void CoordinatePlane::replaceDiagram ( AbstractDiagram* diagram, int position )
{
    if ( d->diagrams.size() <= position ) {
        addDiagram( diagram );
    } else {
        diagram->hide();
        d->diagrams.replace( position, diagram );
        diagram->setParent ( this );
        diagram->setCoordinatePlane( this );
        layoutDiagrams();
    }
}


/*virtual*/
void CoordinatePlane::removeDiagram( int position/* = 0 */ )
{
    if ( position >= 0 && d->diagrams.size() > position ) {
        d->diagrams.removeAt( position );
        layoutDiagrams();
    }
}

AbstractDiagram* CoordinatePlane::diagram()
{
    if ( d->diagrams.isEmpty() )
    {
        return 0;
    } else {
        return d->diagrams.first();
    }
}

AbstractDiagramList CoordinatePlane::diagrams()
{
    return d->diagrams;
}

QSize KDChart::CoordinatePlane::sizeHint() const
{
    return QSize( 1000, 1000 );
}

QSize KDChart::CoordinatePlane::minimumSizeHint() const
{
    return QSize( 200, 200 );
}


QSizePolicy KDChart::CoordinatePlane::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

void KDChart::CoordinatePlane::setGridAttributes( const GridAttributes& a )
{
    d->gridAttributes = a;
}

GridAttributes KDChart::CoordinatePlane::gridAttributes() const
{
    return d->gridAttributes;
}


