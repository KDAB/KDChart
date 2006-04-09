#ifndef KDCHARTABSTRACTPOLARDIAGRAM_P_H
#define KDCHARTABSTRACTPOLARDIAGRAM_P_H

#include "KDChartAbstractDiagram_p.h"

namespace KDChart {

  class PolarCoordinatePlane;

class AbstractPolarDiagram::Private : public AbstractDiagram::Private
{
    friend class AbstractPolarDiagram;
public:
    Private();
    ~Private();

};

inline AbstractPolarDiagram::AbstractPolarDiagram( Private * p )
    : AbstractDiagram( p ) { init(); }
inline AbstractPolarDiagram::AbstractPolarDiagram( Private * p, PolarCoordinatePlane* parent )
    : AbstractDiagram( p, parent ) { init(); }
inline AbstractPolarDiagram::Private * AbstractPolarDiagram::d_func()
{ return static_cast<Private*>( AbstractDiagram::d_func() ); }
inline const AbstractPolarDiagram::Private * AbstractPolarDiagram::d_func() const
{ return static_cast<const Private*>( AbstractDiagram::d_func() ); }

}
#endif /* KDCHARTABSTRACTCARTESIANDIAGRAM_P_H */

