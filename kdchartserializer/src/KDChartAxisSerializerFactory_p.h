#ifndef __KDCHARTAXISSERIALIZERFACTORY_P_H__
#define __KDCHARTAXISSERIALIZERFACTORY_P_H__

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

#include <KDChartAxisSerializerFactory.h>

#include <QMap>

namespace KDChart
{
    class AxesSerializer;
}

/**
 * \internal
 */
class KDChart::AxisSerializerFactory::Private
{
    friend class ::KDChart::AxisSerializerFactory;
    AxisSerializerFactory* const q;
public:
    explicit Private( AxisSerializerFactory * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    mutable KDChart::AxesSerializer* m_instance;
};


#endif // KDChartAxisSerializerFactory_p_H
