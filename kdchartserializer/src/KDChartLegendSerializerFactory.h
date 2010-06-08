#ifndef __KDCHARTLEGENDSERIALIZERFACTORY_H__
#define __KDCHARTLEGENDSERIALIZERFACTORY_H__

/** \file KDChartLegendSerializerFactory.h
  */

#include <KDChartAbstractSerializerFactory>

namespace KDChart {

    class AbstractSerializer;

    class KDCHARTSERIALIZER_EXPORT LegendSerializerFactory : public AbstractSerializerFactory
    {
        Q_OBJECT
        Q_DISABLE_COPY( LegendSerializerFactory )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( LegendSerializerFactory )

    public:
        LegendSerializerFactory( QObject* parent = 0 );
        virtual ~LegendSerializerFactory();

        AbstractSerializer* instance( const  QString& className ) const;
        QObject* createNewObject( const QString& className ) const;
    };

} // end of namespace

#endif
