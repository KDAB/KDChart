#ifndef KDCHARTPERCENTLYINGBARDIAGRAM_P_H
#define KDCHARTPERCENTLYINGBARDIAGRAM_P_H

#include "KDChartBarDiagram_p.h"

namespace KDChart {

    class PercentLyingBarDiagram : public BarDiagram::BarDiagramType
    {
    public:
        explicit PercentLyingBarDiagram( BarDiagram* );
        virtual ~PercentLyingBarDiagram() {}
        virtual BarDiagram::BarType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}

#endif
