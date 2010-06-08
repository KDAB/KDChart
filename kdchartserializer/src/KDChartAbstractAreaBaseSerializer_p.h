#ifndef __KDCHARTABSTRACTAREABASESERIALIZER_P_H__
#define __KDCHARTABSTRACTAREABASESERIALIZER_P_H__

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

#include <KDChartAbstractAreaBaseSerializer.h>

/**
 * \internal
 */
class KDChart::AbstractAreaBaseSerializer::Private
{
    friend class ::KDChart::AbstractAreaBaseSerializer;
    AbstractAreaBaseSerializer * const q;
public:
    explicit Private( AbstractAreaBaseSerializer * qq );
    ~Private(); // non-virtual, since nothing inherits this
};


#endif // KDChartAbstractAreaBaseSerializer_p_H
