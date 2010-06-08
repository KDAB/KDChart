#ifndef KDCHARTPOLARDIAGRAM_P_H
#define KDCHARTPOLARDIAGRAM_P_H

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

#include <KDABLibFakes>


namespace KDChart {

/**
 * \internal
 */
class PolarDiagram::Private : public AbstractPolarDiagram::Private
{
    friend class PolarDiagram;
public:
    Private();
    ~Private();

    Private( const Private& rhs ) :
        AbstractPolarDiagram::Private( rhs ),
        showDelimitersAtPosition( rhs.showDelimitersAtPosition ),
        showLabelsAtPosition( rhs.showLabelsAtPosition ),
        rotateCircularLabels( rhs.rotateCircularLabels ),
        closeDatasets( rhs.closeDatasets )
        {
        }

private:
    QMap<int,bool> showDelimitersAtPosition;
    QMap<int,bool> showLabelsAtPosition;
    bool rotateCircularLabels;
    bool closeDatasets;
    DataValueTextInfoList dataValueInfoList;
};

KDCHART_IMPL_DERIVED_DIAGRAM( PolarDiagram, AbstractPolarDiagram, PolarCoordinatePlane )

}

#endif /* KDCHARTPOLARDIAGRAM_P_H */
