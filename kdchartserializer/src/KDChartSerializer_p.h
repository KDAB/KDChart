#ifndef __KDCHARTSERIALIZER_P_H__
#define __KDCHARTSERIALIZER_P_H__

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

#include <KDChartSerializer.h>

#include <QMap>

class KDChart::AbstractSerializerFactory;

/**
 * \internal
 */
class KDChart::Serializer::Private
{
    friend class ::KDChart::Serializer;
    Serializer * const q;
public:
    explicit Private( Serializer * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    QString m_programName;
    Chart* m_chart;
    CoordPlanesSerializer* m_coordS;

    static void setupSerializerFactoriesMap();
    static QMap< QString, AbstractSerializerFactory* >* s_serializerFactories;
};


#endif // KDChartSerializer_p_H
