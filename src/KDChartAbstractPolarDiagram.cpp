#include "KDChartAbstractPolarDiagram.h"
#include "KDChartAbstractPolarDiagram_p.h"

#include <KDABLibFakes>


using namespace KDChart;

AbstractPolarDiagram::Private::Private()
{
}

AbstractPolarDiagram::Private::~Private()
{
}

void AbstractPolarDiagram::init()
{
}

#define d d_func()

AbstractPolarDiagram::AbstractPolarDiagram (
    QWidget* parent, PolarCoordinatePlane* plane )
    : AbstractDiagram ( new Private(), parent, plane )
{
}


const PolarCoordinatePlane * AbstractPolarDiagram::polarCoordinatePlane() const
{
    return dynamic_cast<const PolarCoordinatePlane*>( coordinatePlane() );
}

int AbstractPolarDiagram::columnCount() const
{
    return static_cast<int>( numberOfValuesPerDataset() );
}

int AbstractPolarDiagram::rowCount() const
{
    return static_cast<int>( numberOfDatasets() );
}
