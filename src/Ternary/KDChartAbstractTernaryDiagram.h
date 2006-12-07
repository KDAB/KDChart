#ifndef KDCHARTABSTRACTTERNARYDIAGRAM_H
#define KDCHARTABSTRACTTERNARYDIAGRAM_H

#include "KDChartAbstractDiagram.h"

namespace KDChart {

    class TernaryCoordinatePlane;

    class AbstractTernaryDiagram : public AbstractDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( AbstractTernaryDiagram )
        KDCHART_DECLARE_DERIVED_DIAGRAM( AbstractTernaryDiagram,
                                         TernaryCoordinatePlane )

    public:
        explicit AbstractTernaryDiagram ( QWidget* parent = 0,
                                          TernaryCoordinatePlane* plane = 0 );
        virtual ~AbstractTernaryDiagram();

        virtual void resize (const QSizeF &area) = 0;
        virtual void paint (PaintContext *paintContext) = 0;

    protected:
        virtual const QPair< QPointF, QPointF >  calculateDataBoundaries () const = 0;

    };

}

#endif
