#ifndef KDCHARTTERNARYDIAGRAM_H
#define KDCHARTTERNARYDIAGRAM_H

#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartAbstractTernaryDiagram.h"


namespace KDChart {

    class TernaryPointDiagram : public AbstractDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( TernaryPointDiagram )
        KDCHART_DECLARE_DERIVED_DIAGRAM( TernaryPointDiagram, TernaryCoordinatePlane )

        friend class TernaryCoordinatePlane;

    public:
        explicit TernaryPointDiagram ( QWidget* parent = 0, TernaryCoordinatePlane* plane = 0 );
        virtual ~TernaryPointDiagram();

        virtual void  resize (const QSizeF &area);
        virtual void  paint (PaintContext *paintContext);

    protected:
        virtual const QPair< QPointF, QPointF >  calculateDataBoundaries () const;

    };

};

#endif
