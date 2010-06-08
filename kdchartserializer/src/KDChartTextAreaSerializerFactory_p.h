#ifndef __KDCHARTTEXTAREASERIALIZERFACTORY_P_H__
#define __KDCHARTTEXTAREASERIALIZERFACTORY_P_H__

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

#include <KDChartTextAreaSerializerFactory.h>

#include <QMap>

namespace KDChart
{
    class TextAreaSerializer;
}

/**
 * \internal
 */
class KDChart::TextAreaSerializerFactory::Private
{
    friend class ::KDChart::TextAreaSerializerFactory;
    TextAreaSerializerFactory* const q;
public:
    explicit Private( TextAreaSerializerFactory * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    mutable KDChart::TextAreaSerializer* m_instance;
};


#endif // KDChartLegendSerializerFactory_p_H
