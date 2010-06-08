#ifndef __KDCHARTLEGENDSERIALIZERFACTORY_P_H__
#define __KDCHARTLEGENDSERIALIZERFACTORY_P_H__

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

#include <KDChartLegendSerializerFactory.h>

#include <QMap>

namespace KDChart
{
    class LegendsSerializer;
}

/**
 * \internal
 */
class KDChart::LegendSerializerFactory::Private
{
    friend class ::KDChart::LegendSerializerFactory;
    LegendSerializerFactory* const q;
public:
    explicit Private( LegendSerializerFactory * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    mutable KDChart::LegendsSerializer* m_instance;
};


#endif // KDChartLegendSerializerFactory_p_H
