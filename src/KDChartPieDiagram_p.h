#ifndef KDCHARTPIEDIAGRAM_P_H
#define KDCHARTPIEDIAGRAM_P_H

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

#include "KDChartAbstractPieDiagram_p.h"

#include <KDABLibFakes>


namespace KDChart {

/**
 * \internal
 */
class PieDiagram::Private : public AbstractPieDiagram::Private
{
    friend class PieDiagram;
public:
    Private();
    ~Private();

    Private( const Private& rhs ) :
        AbstractPieDiagram::Private( rhs )
        {
            // just for consistency
        }

protected:
    // this information needed temporarily at drawing time
    QVector < qreal > startAngles;
    QVector < qreal > angleLens;
    QRectF position;
    qreal size;
};

KDCHART_IMPL_DERIVED_DIAGRAM( PieDiagram, AbstractPieDiagram, PolarCoordinatePlane )

}

#endif /* KDCHARTPIEDIAGRAM_P_H */
