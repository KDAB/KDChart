#ifndef KDCHARTPOLARDIAGRAM_P_H
#define KDCHARTPOLARDIAGRAM_P_H

#include "KDChartAbstractPolarDiagram_p.h"

namespace KDChart {

class PolarDiagram::Private : public AbstractPolarDiagram::Private
{
    friend class PolarDiagram;
public:
    Private();
    ~Private();

private:
    int zeroDegreePosition;
    QMap<AbstractArea::DockingPointType,bool> showDelimitersAtPosition;
    QMap<AbstractArea::DockingPointType,bool> showLabelsAtPosition;
    bool rotateCircularLabels;
};

inline PolarDiagram::PolarDiagram( Private * p )
    : AbstractPolarDiagram( p ) { init(); }
inline PolarDiagram::PolarDiagram( Private * p, PolarCoordinatePlane* plane )
    : AbstractPolarDiagram( p, plane ) { init(); }
inline PolarDiagram::Private * PolarDiagram::d_func()
{ return static_cast<Private*>( AbstractPolarDiagram::d_func() ); }
inline const PolarDiagram::Private * PolarDiagram::d_func() const
{ return static_cast<const Private*>( AbstractPolarDiagram::d_func() ); }

}

#endif /* KDCHARTPOLARDIAGRAM_P_H */

