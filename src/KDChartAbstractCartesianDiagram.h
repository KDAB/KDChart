#ifndef KDCHARTABSTRACTCARTESIANDIAGRAM_H
#define KDCHARTABSTRACTCARTESIANDIAGRAM_H

#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartCartesianAxis.h"

namespace KDChart {

    class KDCHART_EXPORT AbstractCartesianDiagram : public AbstractDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( AbstractCartesianDiagram )
        KDCHART_DECLARE_PRIVATE_DERIVED( AbstractCartesianDiagram )
    protected:
        explicit inline AbstractCartesianDiagram( Private *p, CartesianCoordinatePlane* parent );

    public:
        explicit AbstractCartesianDiagram ( CartesianCoordinatePlane* parent = 0 );
        virtual ~AbstractCartesianDiagram() {}

        virtual const int numberOfAbscissaSegments () const = 0;
        virtual const int numberOfOrdinateSegments () const = 0;
        virtual void addAxes( CartesianAxis * axis );
        virtual KDChart::CartesianAxisList axes () const;

        virtual void setReferenceDiagram( AbstractCartesianDiagram* diagarm, const QPointF& offset = QPointF() );
	virtual AbstractCartesianDiagram* referenceDiagram() const;
	virtual QPointF referenceDiagramOffset() const;
    };

}

#endif
