#ifndef __KDCHARTDIAGRAMSERIALIZERFACTORY_P_H__
#define __KDCHARTDIAGRAMSERIALIZERFACTORY_P_H__

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

#include <KDChartDiagramSerializerFactory.h>

#include <QMap>

namespace KDChart
{
    class DiagramsSerializer;
}

/**
 * \internal
 */
class KDChart::DiagramSerializerFactory::Private
{
    friend class ::KDChart::DiagramSerializerFactory;
    DiagramSerializerFactory* const q;
public:
    explicit Private( DiagramSerializerFactory * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    mutable KDChart::DiagramsSerializer* m_instance;
};


#endif // KDChartDiagramSerializerFactory_p_H
