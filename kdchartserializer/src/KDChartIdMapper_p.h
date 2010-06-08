#ifndef __KDCHARTIDMAPPER_P_H__
#define __KDCHARTIDMAPPER_P_H__

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

#include <KDChartIdMapper.h>

/**
 * \internal
 */
class KDChart::IdMapper::Private
{
    friend class ::KDChart::IdMapper;
    IdMapper * const q;
public:
    explicit Private( IdMapper * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    QMap< const void*, QString > m_mapOfKnownElements;
    QMap< const void*, QString > m_unresolvedMap;
    QString m_counterTag;
};


#endif // KDChartIdMapper_p_H
