#ifndef KDCHARTABSTRACTPOLARDIAGRAM_H
#define KDCHARTABSTRACTPOLARDIAGRAM_H

#include "KDChartPolarCoordinatePlane.h"
#include "KDChartAbstractDiagram.h"

namespace KDChart {

    class AbstractPolarDiagram : public AbstractDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( AbstractPolarDiagram )
        KDCHART_DECLARE_PRIVATE_DERIVED( AbstractPolarDiagram )
    protected:
        explicit inline AbstractPolarDiagram( Private *p, PolarCoordinatePlane* parent );

    public:
        explicit AbstractPolarDiagram ( PolarCoordinatePlane* parent = 0 );
        virtual ~AbstractPolarDiagram() {}

        virtual double valueTotals () const = 0;
        virtual double numberOfValuesPerDataset() const = 0;
        virtual double numberOfGridRings() const = 0;

        PolarCoordinatePlane * polarCoordinatePlane();

    };

}

#endif
