#ifndef KDCHARTTERNARYLINEDIAGRAM_H
#define KDCHARTTERNARYLINEDIAGRAM_H

#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartAbstractTernaryDiagram.h"


namespace KDChart {

    class KDCHART_EXPORT TernaryLineDiagram : public AbstractTernaryDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( TernaryLineDiagram )
        KDCHART_DECLARE_DERIVED_DIAGRAM( TernaryLineDiagram, TernaryCoordinatePlane )

    public:
        explicit TernaryLineDiagram ( QWidget* parent = 0, TernaryCoordinatePlane* plane = 0 );
        virtual ~TernaryLineDiagram();

        void resize (const QSizeF &area);
        void paint (PaintContext *paintContext);

    protected:
        const QPair< QPointF, QPointF > calculateDataBoundaries () const;

    };
}

#endif
