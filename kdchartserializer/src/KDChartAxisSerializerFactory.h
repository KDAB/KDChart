#ifndef __KDCHARTAXISSERIALIZERFACTORY_H__
#define __KDCHARTAXISSERIALIZERFACTORY_H__

/** \file KDChartAxisSerializerFactory.h
  */

#include <KDChartAbstractSerializerFactory>

namespace KDChart {

    class AbstractSerializer;

    class KDCHARTSERIALIZER_EXPORT AxisSerializerFactory : public AbstractSerializerFactory
    {
        Q_OBJECT
        Q_DISABLE_COPY( AxisSerializerFactory )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( AxisSerializerFactory )

    public:
        AxisSerializerFactory( QObject* parent = 0 );
        virtual ~AxisSerializerFactory();

        AbstractSerializer* instance( const  QString& className ) const;
        QObject* createNewObject( const QString& className ) const;
    };

} // end of namespace

#endif
