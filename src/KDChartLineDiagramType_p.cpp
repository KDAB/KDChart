#include "KDChartLineDiagram.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartLineDiagramType_p.h"

using namespace KDChart;

NormalLineDiagram::NormalLineDiagram()
{
}

LineDiagram::LineType NormalLineDiagram::type() const
{
    return LineDiagram::Normal;
}

const QPair<QPointF, QPointF> NormalLineDiagram::calculateDataBoundaries() const
{
    return QPair<QPointF, QPointF>();
}

void NormalLineDiagram::paint(  PaintContext* ctx )
{
}



