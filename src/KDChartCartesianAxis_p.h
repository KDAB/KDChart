#ifndef KDCHARTCARTESIANAXIS_P_H
#define KDCHARTCARTESIANAXIS_P_H

#include "KDChartCartesianAxis.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAxis_p.h"

namespace KDChart {

    class CartesianAxis::Private : public Axis::Private
    {
        friend class CartesianAxis;

    public:
        Private() {};
        ~Private() {};

    private:
        Position position;
        QRectF geometry;
    };

    inline CartesianAxis::Private* CartesianAxis::d_func()
    {
        return static_cast<Private*> ( Axis::d_func() );
    }

    inline const CartesianAxis::Private* CartesianAxis::d_func() const
    {
        return static_cast<const Private*> ( Axis::d_func() );
    }

}

#endif
