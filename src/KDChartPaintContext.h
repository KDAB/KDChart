
#ifndef PAINTCONTEXT_H
#define PAINTCONTEXT_H

class QPainter;

#include <QRectF>
#include "KDChartGlobal.h"

namespace KDChart {

    class CoordinatePlane;

    // This class is internal (and mostly related to AbstractDiagram)
    // but it's exported for examples/LineChart/SineFunctionPlotter.
    class KDCHART_EXPORT PaintContext
    {
    public:
        PaintContext();

        const QRectF rectangle () const;
        void setRectangle( const QRectF& rect );

        QPainter* painter() const;
        void setPainter( QPainter* painter );

        CoordinatePlane* coordinatePlane() const;
        void setCoordinatePlane( CoordinatePlane* plane );


    private:
        class Private;
        Private * _d;
        Private * d_func() { return _d; }
        const Private * d_func() const { return _d; }
    };

}

#endif /* PAINTCONTEXT_H */

