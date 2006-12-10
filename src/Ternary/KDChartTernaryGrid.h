#ifndef KDCHARTTERNARYGRID_H
#define KDCHARTTERNARYGRID_H

#include <QList>

#include "KDChartAbstractGrid.h"
#include "PrerenderedElements/KDChartTextLabelCache.h"

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

        /** Returns two QSizeF objects specifying the dimension of the
            margins needed between each corner of the diagram and the
            border of the drawing area. Margins are required because
            the axis inscriptions are placed adjacent to the corners
            of the plot, outside of the plots rectangle. */
        QPair<QSizeF, QSizeF> requiredMargins() const;
    private:
        QVector<TickInfo> m_tickInfo;
        QList<PrerenderedLabel> m_labels;
        QString m_labelA;
        QString m_labelB;
        QString m_labelC;
    };

}

#endif
