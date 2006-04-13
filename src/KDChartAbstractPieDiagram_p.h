#ifndef KDCHARTABSTRACTPIEDIAGRAM_P_H
#define KDCHARTABSTRACTPIEDIAGRAM_P_H

#include "KDChartAbstractPolarDiagram_p.h"
#include <KDChartThreeDAttributes.h>

namespace KDChart {

class PolarCoordinatePlane;

class AbstractPieDiagram::Private : public AbstractPolarDiagram::Private
{
    friend class AbstractPieDiagram;
public:
    Private();
    ~Private();

private:
    bool explode;
    double startPosition;
};

inline AbstractPieDiagram::AbstractPieDiagram( Private * p, PolarCoordinatePlane* plane )
    : AbstractPolarDiagram( p, plane ) { init(); }
inline AbstractPieDiagram::Private * AbstractPieDiagram::d_func()
{ return static_cast<Private*>( AbstractPolarDiagram::d_func() ); }
inline const AbstractPieDiagram::Private * AbstractPieDiagram::d_func() const
{ return static_cast<const Private*>( AbstractPolarDiagram::d_func() ); }

}
#endif /* KDCHARTABSTRACTPIEDIAGRAM_P_H */

