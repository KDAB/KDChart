#include "KDChartAbstractCoordinatePlane.h"
#include "KDChartGridAttributes.h"

using namespace KDChart;

class AbstractCoordinatePlane::Private
{
public:
    AbstractDiagramList diagrams;
    GridAttributes gridAttributes;
};

AbstractCoordinatePlane::AbstractCoordinatePlane ( QWidget* parent )
    : QWidget ( parent )
    , d ( new Private() )
{
}

AbstractCoordinatePlane::~AbstractCoordinatePlane()
{
    emit destroyedCoordinatePlane( this );
}

void AbstractCoordinatePlane::addDiagram ( AbstractDiagram* diagram )
{
    // diagrams are invisible and paint through their paint() method
    diagram->hide();

    d->diagrams.append ( diagram );
    diagram->setParent ( this );
    diagram->setCoordinatePlane( this );
    layoutDiagrams();
}


/*virtual*/
void AbstractCoordinatePlane::replaceDiagram ( AbstractDiagram* diagram, int position )
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
void AbstractCoordinatePlane::removeDiagram( int position/* = 0 */ )
{
    if ( position >= 0 && d->diagrams.size() > position ) {
        d->diagrams.removeAt( position );
        layoutDiagrams();
    }
}

AbstractDiagram* AbstractCoordinatePlane::diagram()
{
    if ( d->diagrams.isEmpty() )
    {
        return 0;
    } else {
        return d->diagrams.first();
    }
}

AbstractDiagramList AbstractCoordinatePlane::diagrams()
{
    return d->diagrams;
}

QSize KDChart::AbstractCoordinatePlane::sizeHint() const
{
    return QSize( 1000, 1000 );
}

QSize KDChart::AbstractCoordinatePlane::minimumSizeHint() const
{
    return QSize( 200, 200 );
}


QSizePolicy KDChart::AbstractCoordinatePlane::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

void KDChart::AbstractCoordinatePlane::setGridAttributes( const GridAttributes& a )
{
    d->gridAttributes = a;
}

GridAttributes KDChart::AbstractCoordinatePlane::gridAttributes() const
{
    return d->gridAttributes;
}


