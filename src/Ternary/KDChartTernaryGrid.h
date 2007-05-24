#ifndef KDCHARTTERNARYGRID_H
#define KDCHARTTERNARYGRID_H

#include <QList>

#include "KDChartAbstractGrid.h"
#include "PrerenderedElements/KDChartTextLabelCache.h"

namespace KDChart {

    struct TickInfo {
        TickInfo( double percentage = 0, int depth = 0 );
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
            the tick marks are placed outside of the trianges
            containing rectangle.
            The margins are returned in <em>diagram coordinates</em>,
            since the grid does not know about widget coordinates.
        */
        QPair<QSizeF, QSizeF> requiredMargins() const;
        /** Return the locations of the grid lines, so that axes can
            draw axis rulers at the correct positions.
            This information is valid after the grid has been
            painted (that is, the axes need to be painted after the
            grid. */
        const QVector<TickInfo>& tickInfo() const;
    private:
        QVector<TickInfo> m_tickInfo;
        // QList<PrerenderedLabel> m_labels;
    };

}

#endif
