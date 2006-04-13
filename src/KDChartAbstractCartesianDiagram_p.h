#ifndef KDCHARTABSTRACTCARTESIANDIAGRAM_P_H
#define KDCHARTABSTRACTCARTESIANDIAGRAM_P_H

#include <KDChartAbstractDiagram_p.h>
#include <KDChartThreeDAttributes.h>
#include <KDChartGridAttributes.h>

namespace KDChart {

  class CartesianCoordinatePlane;

class AbstractCartesianDiagram::Private : public AbstractDiagram::Private
{
    friend class AbstractCartesianDiagram;
public:
    Private();
   ~Private();

   CartesianAxisList axesList;

   AbstractCartesianDiagram* referenceDiagram;
   QPointF referenceDiagramOffset;

   GridAttributes gridAttributes;
};

inline AbstractCartesianDiagram::AbstractCartesianDiagram( Private * p )
    : AbstractDiagram( p ) { init(); }
inline AbstractCartesianDiagram::AbstractCartesianDiagram( Private * p, CartesianCoordinatePlane* parent )
    : AbstractDiagram( p, parent ) { init(); }
inline AbstractCartesianDiagram::Private * AbstractCartesianDiagram::d_func()
{ return static_cast<Private*>( AbstractDiagram::d_func() ); }
inline const AbstractCartesianDiagram::Private * AbstractCartesianDiagram::d_func() const
{ return static_cast<const Private*>( AbstractDiagram::d_func() ); }

}
#endif /* KDCHARTABSTRACTCARTESIANDIAGRAM_P_H */

