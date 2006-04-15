#ifndef KDCHARTAXIS_P_H
#define KDCHARTAXIS_P_H

#include "KDChartAbstractArea_p.h"
#include <KDChartTextAttributes.h>
#include <KDChartAbstractDiagram.h>


namespace KDChart {

class AbstractAxis::Private : public AbstractArea::Private
{
    friend class AbstractAxis;

public:
    Private();
    ~Private();

    AbstractDiagram* diagram() const;

private:
    AbstractDiagram* parent;
};


inline AbstractAxis::AbstractAxis( Private * p, AbstractDiagram* parent_ )
    :  AbstractArea( p, parent_ )
{
    p->parent = parent_;
    init();
}

inline AbstractDiagram* AbstractAxis::Private::diagram() const
{
    return parent;
}

inline AbstractAxis::Private * AbstractAxis::d_func()
{ return static_cast<Private*>( AbstractArea::d_func() ); }
inline const AbstractAxis::Private * AbstractAxis::d_func() const
{ return static_cast<const Private*>( AbstractArea::d_func() ); }

}
#endif /* KDCHARTAREA_P_H */

