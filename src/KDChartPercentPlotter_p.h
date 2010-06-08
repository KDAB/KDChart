#ifndef KDCHARTPERCENTPLOTTER_P_H
#define KDCHARTPRECENTPLOTTER_P_H

#include "KDChartPlotter_p.h"

namespace KDChart {

    class PercentPlotter : public Plotter::PlotterType
    {
    public:
        explicit PercentPlotter( Plotter* );
        virtual ~PercentPlotter() {}
        virtual Plotter::PlotType type() const;
        virtual const QPair< QPointF,  QPointF > calculateDataBoundaries() const;
        virtual void paint( PaintContext* ctx );
    };
}

#endif
