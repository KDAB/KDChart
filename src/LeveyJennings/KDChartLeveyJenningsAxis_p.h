#ifndef KDCHARTLEVEYJENNINGSAXIS_P_H
#define KDCHARTLEVEYJENNINGSAXIS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartLeveyJenningsAxis.h"
#include "KDChartLeveyJenningsDiagram.h"
#include "KDChartCartesianAxis_p.h"

#include <KDABLibFakes>


namespace KDChart {

/**
  * \internal
  */
class LeveyJenningsAxis::Private : public CartesianAxis::Private
{
    friend class LeveyJenningsAxis;

public:
    Private( LeveyJenningsDiagram* diagram, LeveyJenningsAxis* axis )
        : CartesianAxis::Private( diagram, axis )
    {}
    ~Private() {}

private:
    LeveyJenningsGridAttributes::GridType type;
    Qt::DateFormat format;
};

inline LeveyJenningsAxis::LeveyJenningsAxis( Private * p, AbstractDiagram* diagram )
    : CartesianAxis( p, diagram )
{
    init();
}
inline LeveyJenningsAxis::Private * LeveyJenningsAxis::d_func()
{ return static_cast<Private*>( CartesianAxis::d_func() ); }
inline const LeveyJenningsAxis::Private * LeveyJenningsAxis::d_func() const
{ return static_cast<const Private*>( CartesianAxis::d_func() ); }

}

#endif
