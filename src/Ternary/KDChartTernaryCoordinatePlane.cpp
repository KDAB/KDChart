#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartTernaryCoordinatePlane_p.h"

using namespace KDChart;

TernaryCoordinatePlane::TernaryCoordinatePlane( Chart* parent )
    : AbstractCoordinatePlane( parent )
{

}

TernaryCoordinatePlane::~TernaryCoordinatePlane()
{
}

void TernaryCoordinatePlane::addDiagram( AbstractDiagram* diagram )
{
}

void TernaryCoordinatePlane::replaceDiagram( AbstractDiagram* diagram,
                                             AbstractDiagram* replacement )
{
}

void TernaryCoordinatePlane::takeDiagram( AbstractDiagram* diagram )
{
}

AbstractDiagram* TernaryCoordinatePlane::diagram()
{
}

AbstractDiagramList TernaryCoordinatePlane::diagrams()
{
}

ConstAbstractDiagramList TernaryCoordinatePlane::diagrams() const
{
}

void TernaryCoordinatePlane::layoutDiagrams()
{
}

const QPointF TernaryCoordinatePlane::translate( const QPointF& diagramPoint ) const
{
}

QSize TernaryCoordinatePlane::minimumSizeHint() const
{
}

QSizePolicy TernaryCoordinatePlane::sizePolicy() const
{
}


