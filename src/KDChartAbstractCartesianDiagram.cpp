#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractCartesianDiagram_p.h"
#include <QDebug>

using namespace KDChart;
AbstractCartesianDiagram::Private::Private()
    : referenceDiagram(NULL)
{
}

AbstractCartesianDiagram::Private::~Private() 
{
 
}

void AbstractCartesianDiagram::init()
{
}

#define d d_func()

AbstractCartesianDiagram::AbstractCartesianDiagram ( CartesianCoordinatePlane* parent )
    : AbstractDiagram ( new Private(), parent )
{
}

void AbstractCartesianDiagram::addAxes( CartesianAxis *axis )
{
   
    d->axesList.append( axis );
}

KDChart::CartesianAxisList AbstractCartesianDiagram::axes( ) const 
{
    return d->axesList;
}

void AbstractCartesianDiagram::setReferenceDiagram( AbstractCartesianDiagram* diagram, const QPointF& offset )
{
    d->referenceDiagram = diagram;
    d->referenceDiagramOffset = offset;
}

AbstractCartesianDiagram* AbstractCartesianDiagram::referenceDiagram() const
{
    return d->referenceDiagram;
}

QPointF AbstractCartesianDiagram::referenceDiagramOffset() const
{
    return d->referenceDiagramOffset;
}
