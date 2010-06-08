#ifndef KDCHARTNORMALLINEDIAGRAM_P_H
#define KDCHARTNORMALLINEDIAGRAM_P_H

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
#include "KDChartLineDiagram_p.h"

namespace KDChart {

    class NormalLineDiagram : public LineDiagram::LineDiagramType
    {
    public:
        explicit NormalLineDiagram( LineDiagram* );
        virtual ~NormalLineDiagram() {}
        virtual LineDiagram::LineType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}

#endif
