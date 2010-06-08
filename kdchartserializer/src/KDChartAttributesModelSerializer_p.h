#ifndef __KDCHARTATTRIBUTESMODELSERIALIZER_P_H__
#define __KDCHARTATTRIBUTESMODELSERIALIZER_P_H__

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

#include <KDChartAttributesModelSerializer.h>

/**
 * \internal
 */
class KDChart::AttributesModelSerializer::Private
{
    friend class ::KDChart::AttributesModelSerializer;
    AttributesModelSerializer * const q;
public:
    explicit Private( AttributesModelSerializer * qq );
    ~Private(); // non-virtual, since nothing inherits this
};


#endif // KDChartAttributesModelSerializer_p_H
