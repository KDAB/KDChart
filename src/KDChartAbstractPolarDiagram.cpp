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


const PolarCoordinatePlane * AbstractPolarDiagram::polarCoordinatePlane() const
{
    return dynamic_cast<const PolarCoordinatePlane*>( coordinatePlane() );
}
