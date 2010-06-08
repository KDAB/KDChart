#ifndef KDCHARTNORMALLYINGBARDIAGRAM_P_H
#define KDCHARTNORMALLYINGBARDIAGRAM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
#include "KDChartBarDiagram_p.h"

namespace KDChart {

    class NormalLyingBarDiagram : public BarDiagram::BarDiagramType
    {
    public:
        explicit NormalLyingBarDiagram( BarDiagram* );
        virtual ~NormalLyingBarDiagram() {}
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
