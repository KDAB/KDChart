#ifndef KDCHARTSTACKEDLINEDIAGRAM_P_H
#define KDCHARTSTACKEDLINEDIAGRAM_P_H

#include "KDChartLineDiagram_p.h"

namespace KDChart {

    class StackedLineDiagram : public LineDiagram::LineDiagramType
    {
    public:
        StackedLineDiagram();
        virtual ~StackedLineDiagram() {}
        virtual LineDiagram::LineType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}

#endif
