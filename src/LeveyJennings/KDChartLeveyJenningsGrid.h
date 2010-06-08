#ifndef KDCHARTLEVEYJENNINGSGRID_H
#define KDCHARTLEVEYJENNINGSGRID_H

#include "KDChartCartesianGrid.h"

namespace KDChart {

    class PaintContext;

    /**
     * \internal
     *
     * \brief Class for the grid in a Levey Jennings plane.
     *
     * The LeveyJenningsGrid interface is used
     * for calculating and for drawing
     * the horizonal grid lines, and the vertical grid lines
     * of a Levey Jennings coordinate plane.
     */
    class LeveyJenningsGrid : public CartesianGrid
    {
    public:
        LeveyJenningsGrid() : CartesianGrid(){}
        virtual ~LeveyJenningsGrid(){}

        void drawGrid( PaintContext* context );

    private:
        DataDimensionsList calculateGrid( const DataDimensionsList& rawDataDimensions ) const;
        DataDimension calculateGridXY( const DataDimension& rawDataDimension, 
                          Qt::Orientation orientation, bool adjustLower, bool adjustUpper ) const;
        void calculateStepWidth( qreal start_, qreal end_, const QList<qreal>& granularities, Qt::Orientation orientation,
                                 qreal& stepWidth, qreal& subStepWidth, bool adjustLower, bool adjustUpper ) const;
    };

}

#endif
