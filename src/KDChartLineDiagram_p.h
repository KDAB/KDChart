#ifndef KDCHARTLINEDIAGRAM_P_H
#define KDCHARTLINEDIAGRAM_P_H


#include "KDChartAbstractCartesianDiagram_p.h"

namespace KDChart {

  //class CartesianCoordinatePlane;

class LineDiagram::Private : public AbstractCartesianDiagram::Private
{
    friend class LineDiagram;
public:
    Private();
    ~Private();
    
    LineType lineType;
};


inline LineDiagram::LineDiagram( Private * p, CartesianCoordinatePlane* plane )
  : AbstractCartesianDiagram( p, plane ) { init(); }
inline LineDiagram::Private * LineDiagram::d_func()
{ return static_cast<Private*>( AbstractCartesianDiagram::d_func() ); }
inline const LineDiagram::Private * LineDiagram::d_func() const
{ return static_cast<const Private*>( AbstractCartesianDiagram::d_func() ); }

}

#endif /* KDCHARTLINEDIAGRAM_P_H */

