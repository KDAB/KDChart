#include "KDChartLineDiagram_p.h"

namespace KDChart {

    class NormalLineDiagram : public LineDiagram::LineDiagramType
    {
    public:
        NormalLineDiagram();
        virtual ~NormalLineDiagram() {}
        virtual LineDiagram::LineType type() const;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const;
        virtual void paint(  PaintContext* ctx );
    };

}
