#ifndef KDCHARTLEVEYJENNINGSCOORDINATEPLANE_H
#define KDCHARTLEVEYJENNINGSCOORDINATEPLANE_H

#include "../KDChartCartesianCoordinatePlane.h"

#include "KDChartLeveyJenningsGridAttributes.h"

namespace KDChart {

    class LeveyJenningsGrid;

    /**
      * @brief Levey Jennings coordinate plane
      * This is actually nothing real more than a plain cartesian
      * coordinate plane. The difference is, that only Levey Jennings
      * Diagrams can be added to it.
      */
    class KDCHART_EXPORT LeveyJenningsCoordinatePlane
        : public CartesianCoordinatePlane
    {
        Q_OBJECT
        Q_DISABLE_COPY( LeveyJenningsCoordinatePlane )
        KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( LeveyJenningsCoordinatePlane, Chart* )

        friend class LeveyJenningsGrid;

    public:
        explicit LeveyJenningsCoordinatePlane( Chart* parent = 0 );
        ~LeveyJenningsCoordinatePlane();

        void addDiagram( AbstractDiagram* diagram );

        LeveyJenningsGridAttributes gridAttributes() const;
        void setGridAttributes( const LeveyJenningsGridAttributes& attr );
   
    protected: 
        const QPointF translateBack( const QPointF& screenPoint ) const;

    private:
        LeveyJenningsGrid* grid() const;
    };

}

#endif
