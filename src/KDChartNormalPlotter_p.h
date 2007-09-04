#ifndef KDCHARTNORMALPLOTTER_P_H
#define KDCHARTNORMALPLOTTER_P_H

#include "KDChartPlotter_p.h"

namespace KDChart {

    class NormalPlotter : public Plotter::PlotterType
    {
    public:
        explicit NormalPlotter( Plotter* );
        virtual ~NormalPlotter() {}
        virtual Plotter::PlotType type() const;
        virtual const QPair< QPointF,  QPointF > calculateDataBoundaries() const;
        virtual void paint( PaintContext* ctx );
    };
}

#endif
