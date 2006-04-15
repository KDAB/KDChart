#ifndef KDCHARTCARTESIANAXIS_P_H
#define KDCHARTCARTESIANAXIS_P_H

#include "KDChartCartesianAxis.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractAxis_p.h"

namespace KDChart {

    class CartesianAxis::Private : public AbstractAxis::Private
    {
        friend class CartesianAxis;

    public:
        Private() {};
        ~Private() {};

    private:
        Position position;
        QRectF geometry;
    };

    inline CartesianAxis::CartesianAxis( Private * p, AbstractCartesianDiagram* parent )
    : AbstractAxis( p, parent ) { init(); }
    inline CartesianAxis::Private * CartesianAxis::d_func()
    { return static_cast<Private*>( AbstractAxis::d_func() ); }
    inline const CartesianAxis::Private * CartesianAxis::d_func() const
    { return static_cast<const Private*>( AbstractAxis::d_func() ); }

}

#endif
