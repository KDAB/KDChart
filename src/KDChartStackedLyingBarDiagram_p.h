#ifndef KDCHARTSTACKEDLYINGBARDIAGRAM_P_H
#define KDCHARTSTACKEDLYINGBARDIAGRAM_P_H

#include "KDChartBarDiagram_p.h"

namespace KDChart {

    class StackedLyingBarDiagram : public BarDiagram::BarDiagramType
    {
    public:
        explicit StackedLyingBarDiagram( BarDiagram* );
        virtual ~StackedLyingBarDiagram() {}
        virtual BarDiagram::BarType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}

#endif
