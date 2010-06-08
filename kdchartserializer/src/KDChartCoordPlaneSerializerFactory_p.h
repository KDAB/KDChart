#ifndef __KDCHARTCOORDPLANESERIALIZERFACTORY_P_H__
#define __KDCHARTCOORDPLANESERIALIZERFACTORY_P_H__

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

#include <KDChartCoordPlaneSerializerFactory.h>

#include <QMap>

namespace KDChart
{
    class CoordPlanesSerializer;
}

/**
 * \internal
 */
class KDChart::CoordPlaneSerializerFactory::Private
{
    friend class ::KDChart::CoordPlaneSerializerFactory;
    CoordPlaneSerializerFactory* const q;
public:
    explicit Private( CoordPlaneSerializerFactory * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    mutable KDChart::CoordPlanesSerializer* m_instance;
};


#endif // KDChartCoordPlaneSerializerFactory_p_H
