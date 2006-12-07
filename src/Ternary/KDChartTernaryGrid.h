#ifndef KDCHARTTERNARYGRID_H
#define KDCHARTTERNARYGRID_H

#include "KDChartAbstractGrid.h"

namespace KDChart {

    class PaintContext;

    class TernaryGrid : public AbstractGrid
    {
    public:
        TernaryGrid();

        virtual ~TernaryGrid();

        void drawGrid( PaintContext* context );
    };

}

#endif
