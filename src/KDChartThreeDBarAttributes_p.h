#ifndef KDCHARTTHREEDBARATTRIBUTES_P_H
#define KDCHARTTHREEDBARATTRIBUTES_P_H

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
class ThreeDBarAttributes::Private : public AbstractThreeDAttributes::Private
{
    friend class ThreeDBarAttributes;
public:
    Private();

private:
    bool useShadowColors;
    uint angle;
};

KDCHART_DERIVED_PRIVATE_FOOTER(ThreeDBarAttributes, AbstractThreeDAttributes)

}


#endif // KDCHARTTHREEDBARATTRIBUTES_P_H
