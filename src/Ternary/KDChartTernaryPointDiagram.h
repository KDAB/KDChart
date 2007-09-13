#ifndef KDCHARTTERNARYDIAGRAM_H
#define KDCHARTTERNARYDIAGRAM_H

#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartAbstractTernaryDiagram.h"

namespace KDChart {

    /**
      * @brief A TernaryPointDiagram is a point diagram within a ternary coordinate plane
      */
    class KDCHART_EXPORT TernaryPointDiagram : public AbstractTernaryDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( TernaryPointDiagram )
        KDCHART_DECLARE_DERIVED_DIAGRAM( TernaryPointDiagram, TernaryCoordinatePlane )

    public:
        explicit TernaryPointDiagram ( QWidget* parent = 0, TernaryCoordinatePlane* plane = 0 );
        virtual ~TernaryPointDiagram();

        virtual void resize (const QSizeF &area);
        virtual void paint (PaintContext *paintContext);

    protected:
        virtual const QPair< QPointF, QPointF >  calculateDataBoundaries () const;
    };

}

#endif
