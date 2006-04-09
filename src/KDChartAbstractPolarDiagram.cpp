#include "KDChartAbstractPolarDiagram.h"
#include "KDChartAbstractPolarDiagram_p.h"

using namespace KDChart;
AbstractPolarDiagram::Private::Private()
{
}

AbstractPolarDiagram::Private::~Private() {}

void AbstractPolarDiagram::init()
{
}

#define d d_func()

AbstractPolarDiagram::AbstractPolarDiagram ( PolarCoordinatePlane* parent )
    : AbstractDiagram ( new Private(), parent )
{
}


PolarCoordinatePlane * AbstractPolarDiagram::polarCoordinatePlane()
{
    return dynamic_cast<PolarCoordinatePlane*>( coordinatePlane() );
}
