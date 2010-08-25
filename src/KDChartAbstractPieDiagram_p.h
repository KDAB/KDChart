#ifndef KDCHARTABSTRACTPIEDIAGRAM_P_H
#define KDCHARTABSTRACTPIEDIAGRAM_P_H

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

#include "KDChartAbstractPolarDiagram_p.h"
#include <KDChartAbstractThreeDAttributes.h>

#include <KDABLibFakes>


namespace KDChart {

class PolarCoordinatePlane;

/**
 * \internal
 */
class AbstractPieDiagram::Private : public AbstractPolarDiagram::Private
{
    friend class AbstractPieDiagram;
public:
    Private();
    ~Private();

    Private( const Private& rhs ) :
        AbstractPolarDiagram::Private( rhs ),
        granularity( rhs.granularity )
        {
        }

private:
    double granularity;
    bool autoRotateLabels;
};

KDCHART_IMPL_DERIVED_DIAGRAM( AbstractPieDiagram, AbstractPolarDiagram, PolarCoordinatePlane )

}
#endif /* KDCHARTABSTRACTPIEDIAGRAM_P_H */

