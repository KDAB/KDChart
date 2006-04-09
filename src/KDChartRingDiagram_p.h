#ifndef KDCHARTRINGDIAGRAM_P_H
#define KDCHARTRINGDIAGRAM_P_H

#include "KDChartAbstractPieDiagram_p.h"

namespace KDChart {

class RingDiagram::Private : public AbstractPieDiagram::Private
{
    friend class RingDiagram;
public:
    Private();
    ~Private();

private:
    bool relativeThickness;
};

inline RingDiagram::RingDiagram( Private * p, PolarCoordinatePlane* plane )
    : AbstractPieDiagram( p, plane ) { init(); }
inline RingDiagram::Private * RingDiagram::d_func()
{ return static_cast<Private*>( AbstractPieDiagram::d_func() ); }
inline const RingDiagram::Private * RingDiagram::d_func() const
{ return static_cast<const Private*>( AbstractPieDiagram::d_func() ); }

}

#endif /* KDCHARTRINGDIAGRAM_P_H */

