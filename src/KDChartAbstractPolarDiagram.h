#ifndef KDCHARTABSTRACTPOLARDIAGRAM_H
#define KDCHARTABSTRACTPOLARDIAGRAM_H

#include "KDChartPolarCoordinatePlane.h"
#include "KDChartAbstractDiagram.h"

namespace KDChart {

    class GridAttributes;

    /**
      * @brief Base class for diagrams based on a polar coordinate system.
      */
    class KDCHART_EXPORT AbstractPolarDiagram : public AbstractDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( AbstractPolarDiagram )
        KDCHART_DECLARE_DERIVED_DIAGRAM( AbstractPolarDiagram, PolarCoordinatePlane )

    public:
        explicit AbstractPolarDiagram (
            QWidget* parent = 0, PolarCoordinatePlane* plane = 0 );
        virtual ~AbstractPolarDiagram() {}

        virtual double valueTotals () const = 0;
        virtual double numberOfValuesPerDataset() const = 0;
        virtual double numberOfDatasets() const { return 1; };
        virtual double numberOfGridRings() const = 0;

        const PolarCoordinatePlane * polarCoordinatePlane() const;

        int columnCount() const;
        int rowCount() const;
    };

}

#endif
