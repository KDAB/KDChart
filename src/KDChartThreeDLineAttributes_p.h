#ifndef KDCHARTTHREEDLINESATTRIBUTES_P_H
#define KDCHARTTHREEDLINESATTRIBUTES_P_H

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

#include <KDChartAbstractThreeDAttributes_p.h>

#include <KDABLibFakes>


namespace KDChart {

/**
 * \internal
 */
class ThreeDLineAttributes::Private : public AbstractThreeDAttributes::Private
{
    friend class ThreeDLineAttributes;
public:
    Private();

private:
    //ThreeDLines
    uint lineXRotation;
    uint lineYRotation;
};

KDCHART_DERIVED_PRIVATE_FOOTER(ThreeDLineAttributes, AbstractThreeDAttributes)

}


#endif // KDCHARTTHREEDLINESATTRIBUTES_P_H
