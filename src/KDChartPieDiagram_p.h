#ifndef KDCHARTPIEDIAGRAM_P_H
#define KDCHARTPIEDIAGRAM_P_H

#include "KDChartAbstractPieDiagram_p.h"

namespace KDChart {

class PieDiagram::Private : public AbstractPieDiagram::Private
{
    friend class PieDiagram;
public:
    Private();
    ~Private();

private:
};

inline PieDiagram::PieDiagram( Private * p, PolarCoordinatePlane* plane )
    : AbstractPieDiagram( p, plane ) { init(); }
inline PieDiagram::Private * PieDiagram::d_func()
{ return static_cast<Private*>( AbstractPieDiagram::d_func() ); }
inline const PieDiagram::Private * PieDiagram::d_func() const
{ return static_cast<const Private*>( AbstractPieDiagram::d_func() ); }

}
#endif /* KDCHARTPIEDIAGRAM_P_H */

