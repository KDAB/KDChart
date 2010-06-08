#ifndef __KDCHARTATTRIBUTESSERIALIZER_P_H__
#define __KDCHARTATTRIBUTESSERIALIZER_P_H__

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

#include <KDChartAttributesSerializer.h>

/**
 * \internal
 */
class KDChart::AttributesSerializer::Private
{
    friend class ::KDChart::AttributesSerializer;
    AttributesSerializer * const q;
public:
    explicit Private( AttributesSerializer * qq );
    ~Private(); // non-virtual, since nothing inherits this
};


#endif // __KDCHARTATTRIBUTESSERIALIZER_P_H__
