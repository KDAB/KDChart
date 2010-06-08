#ifndef KDCHARTRINGDIAGRAM_P_H
#define KDCHARTRINGDIAGRAM_P_H

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
class RingDiagram::Private : public AbstractPieDiagram::Private
{
    friend class RingDiagram;
public:
    Private();
    ~Private();

    Private( const Private& rhs ) :
        AbstractPieDiagram::Private( rhs )
        {
            relativeThickness = rhs.relativeThickness;
            expandWhenExploded = rhs.expandWhenExploded;
        }

protected:
    // this information needed temporarily at drawing time
    QVector< QVector < qreal > > startAngles;
    QVector< QVector < qreal > > angleLens;
    QRectF position;
    qreal size;
    bool relativeThickness;
    bool expandWhenExploded;
    // polygons associated to their 3d depth
    QMap<qreal, QPolygon> polygonsToRender;
};

KDCHART_IMPL_DERIVED_DIAGRAM( RingDiagram, AbstractPieDiagram, PolarCoordinatePlane )

}

#endif /* KDCHARTRINGDIAGRAM_P_H */
