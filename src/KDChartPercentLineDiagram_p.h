#ifndef KDCHARTPERCENTLINEDIAGRAM_P_H
#define KDCHARTPERCENTLINEDIAGRAM_P_H

#include "KDChartLineDiagram_p.h"

namespace KDChart {

    class PercentLineDiagram : public LineDiagram::LineDiagramType
    {
    public:
        PercentLineDiagram();
        virtual ~PercentLineDiagram() {}
        virtual LineDiagram::LineType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}

#endif
