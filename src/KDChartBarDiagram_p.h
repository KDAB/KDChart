#ifndef KDCHARTBARDIAGRAM_P_H
#define KDCHARTBARDIAGRAM_P_H

#include "KDChartAbstractCartesianDiagram_p.h"

namespace KDChart {

class BarDiagram::Private : public AbstractCartesianDiagram::Private
{
    friend class BarDiagram;
public:
    Private();
    ~Private();

    void calculateValueAndGapWidths( int rowCount, int colCount,         
                                     double groupWidth,
                                     double& barWidth,
                                     double& spaceBetweenBars,
                                     double& spaceBetweenGroups );

    BarType barType;
};

inline BarDiagram::BarDiagram( Private * p, CartesianCoordinatePlane* plane )
    : AbstractCartesianDiagram( p, plane ) { init(); }
inline BarDiagram::Private * BarDiagram::d_func()
{ return static_cast<Private*>( AbstractCartesianDiagram::d_func() ); }
inline const BarDiagram::Private * BarDiagram::d_func() const
{ return static_cast<const Private*>( AbstractCartesianDiagram::d_func() ); }

}

#endif /* KDCHARTBARDIAGRAM_P_H */

