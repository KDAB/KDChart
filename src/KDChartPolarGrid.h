#ifndef KDCHARTPOLARGrid_H
#define KDCHARTPOLARGrid_H

#include "KDChartPolarCoordinatePlane.h"
#include "KDChartAbstractGrid.h"

namespace KDChart {

    class PaintContext;
    class PolarCoordinatePlane;

    /**
     * \internal
     *
     * \brief Class for the grid in a polar plane.
     *
     * The PolarGrid interface is used
     * for calculating and for drawing
     * the sagittal grid lines, and the circular grid lines
     * of a polar coordinate plane.
     */
    class PolarGrid : public AbstractGrid
    {
    public:
        PolarGrid() : AbstractGrid(){}
        virtual ~PolarGrid(){}

        virtual void drawGrid( PaintContext* context );

    private:
        virtual DataDimensionsList calculateGrid(
            const DataDimensionsList& rawDataDimensions ) const;
    };

}

#endif
