#ifndef KDCHARTNORMALBARDIAGRAM_P_H
#define KDCHARTNORMALBARDIAGRAM_P_H

#include "KDChartBarDiagram_p.h"

namespace KDChart {

    class NormalBarDiagram : public BarDiagram::BarDiagramType
    {
    public:
        explicit NormalBarDiagram( BarDiagram* );
        virtual ~NormalBarDiagram() {}
        virtual BarDiagram::BarType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
/*
        virtual void calculateValueAndGapWidths( int rowCount,int colCount,
				    double groupWidth,
				    double& outBarWidth,
				    double& outSpaceBetweenBars,
				    double& outSpaceBetweenGroups );
*/
    };

}

#endif
