#ifndef KDCHARTSTACKEDBARDIAGRAM_P_H
#define KDCHARTSTACKEDBARDIAGRAM_P_H

#include "KDChartBarDiagram_p.h"

namespace KDChart {

    class StackedBarDiagram : public BarDiagram::BarDiagramType
    {
    public:
        explicit StackedBarDiagram( BarDiagram* );
        virtual ~StackedBarDiagram() {}
        virtual BarDiagram::BarType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}

#endif
