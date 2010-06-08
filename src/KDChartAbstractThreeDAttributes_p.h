#ifndef KDCHARTABSTRACTTHREEDATTRIBUTES_P_H
#define KDCHARTABSTRACTTHREEDATTRIBUTES_P_H

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

#include <KDChartAbstractThreeDAttributes.h>

#include <KDABLibFakes>


namespace KDChart {

/**
 * \internal
 */
class AbstractThreeDAttributes::Private
{
    friend class AbstractThreeDAttributes;
public:
    Private();

private:
    bool enabled;
    int height;
    double depth;
};

inline AbstractThreeDAttributes::AbstractThreeDAttributes( Private * p ) : _d( p ) { init(); }

}

#endif // KDCHARTABSTRACTTHREEDATTRIBUTES_P_H
