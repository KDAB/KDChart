#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"

using namespace KDChart;

AbstractTernaryDiagram::AbstractTernaryDiagram( QWidget* parent,
                                                TernaryCoordinatePlane* plane )
    : AbstractDiagram( parent, plane )
{
}

AbstractTernaryDiagram::~AbstractTernaryDiagram()
{
}


