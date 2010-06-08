#ifndef __KDCHARTLEGENDSSERIALIZER_P_H__
#define __KDCHARTLEGENDSSERIALIZER_P_H__

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

#include <KDChartLegendsSerializer.h>

/**
 * \internal
 */
class KDChart::LegendsSerializer::Private
{
    friend class ::KDChart::LegendsSerializer;
    LegendsSerializer * const q;
public:
    explicit Private( LegendsSerializer * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    static void saveLegend( QDomDocument& doc, QDomElement& element, const Legend& legend );
    static bool doParseLegend( const QDomElement& container, Legend* ptr );
};


#endif // KDChartLegendsSerializer_p_H
