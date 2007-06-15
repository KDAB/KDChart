#ifndef KDCHARTPERCENTBARDIAGRAM_P_H
#define KDCHARTPERCENTBARDIAGRAM_P_H

#include "KDChartBarDiagram_p.h"

namespace KDChart {

    class PercentBarDiagram : public BarDiagram::BarDiagramType
    {
    public:
        explicit PercentBarDiagram( BarDiagram* );
        virtual ~PercentBarDiagram() {}
        virtual BarDiagram::BarType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}

#endif
