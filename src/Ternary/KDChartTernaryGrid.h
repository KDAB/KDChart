#ifndef KDCHARTTERNARYGRID_H
#define KDCHARTTERNARYGRID_H

#include "KDChartAbstractGrid.h"

namespace KDChart {

    struct TickInfo {
        double percentage;
        int depth;
    };

    bool operator==(const TickInfo&, const TickInfo& );

    class PaintContext;

    // VERIFY: Grids are not public API, are they?
    class TernaryGrid : public AbstractGrid
    {
    public:
        TernaryGrid();

        virtual ~TernaryGrid();

        void drawGrid( PaintContext* context );
        DataDimensionsList calculateGrid( const DataDimensionsList& rawDataDimensions ) const;
    private:
        QVector<TickInfo> m_tickInfo;
    };

}

#endif
