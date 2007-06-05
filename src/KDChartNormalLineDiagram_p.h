#ifndef KDCHARTNORMALLINEDIAGRAM_P_H
#define KDCHARTNORMALLINEDIAGRAM_P_H

#include "KDChartLineDiagram_p.h"

namespace KDChart {

    class NormalLineDiagram : public LineDiagram::LineDiagramType
    {
    public:
        explicit NormalLineDiagram( LineDiagram* );
        virtual ~NormalLineDiagram() {}
        virtual LineDiagram::LineType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}

#endif
