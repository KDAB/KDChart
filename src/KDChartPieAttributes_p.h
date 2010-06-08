#ifndef KDCHART_PIE_ATTRIBUTES_P_H
#define KDCHART_PIE_ATTRIBUTES_P_H

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

#include <KDChartPieAttributes.h>

#include <KDABLibFakes>


namespace KDChart {

/**
 * \internal
 */
class PieAttributes::Private
{
    friend class PieAttributes;
public:
    Private();

    qreal explodeFactor;
    qreal tangentialGapFactor;
    qreal radialGapFactor;
};

}

#endif // KDCHART_PIE_ATTRIBUTES_P_H
