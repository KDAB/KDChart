#ifndef __KDCHARTSERIALIZECOLLECTOR_P_H__
#define __KDCHARTSERIALIZECOLLECTOR_P_H__

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

#include <KDChartSerializeCollector.h>

/**
 * \internal
 */
class KDChart::SerializeCollector::Private
{
    friend class ::KDChart::SerializeCollector;
    SerializeCollector * const q;
public:
    explicit Private( SerializeCollector * qq );
    ~Private(); // non-virtual, since nothing inherits this
protected:
    static QString unresolvedTagName();
    static QString unresolvedMapName();

    QMap< QString, QDomElement* > m_map;
    InitializedPointersMap m_initializedPointersMap;
};


#endif // __KDCHARTSERIALIZECOLLECTOR_P_H__
