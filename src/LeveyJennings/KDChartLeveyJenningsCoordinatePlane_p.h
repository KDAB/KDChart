#ifndef KDCHARTLEVEYJENNINGSCOORDINATEPLANE_P_H
#define KDCHARTLEVEYJENNINGSCOORDINATEPLANE_P_H

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

#include <QRectF>

#include "KDChartLeveyJenningsGrid.h"
#include "KDChartCartesianCoordinatePlane_p.h"

#include <KDABLibFakes>

namespace KDChart {

    class CartesianAxis;

    /**
     * \internal
     */

    class LeveyJenningsCoordinatePlane::Private : public CartesianCoordinatePlane::Private
    {
        friend class LeveyJenningsCoordinatePlane;

    public:
        explicit Private();

        virtual ~Private() {
            // grid is delete in base class dtor
        }

        virtual void initialize()
        {
            grid = new LeveyJenningsGrid();
        }

        LeveyJenningsGridAttributes gridAttributes;
    };

    KDCHART_IMPL_DERIVED_PLANE(LeveyJenningsCoordinatePlane, CartesianCoordinatePlane)
}

#endif /* KDCHARTTERNARYCOORDINATEPLANE_P_H */
