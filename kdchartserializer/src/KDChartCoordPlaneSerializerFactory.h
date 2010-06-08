#ifndef __KDCHARTCOORDPLANESERIALIZERFACTORY_H__
#define __KDCHARTCOORDPLANESERIALIZERFACTORY_H__

/** \file KDChartCoordPlaneSerializerFactory.h
  */

#include <KDChartAbstractSerializerFactory.h>

namespace KDChart {

    class AbstractSerializer;

    class KDCHARTSERIALIZER_EXPORT CoordPlaneSerializerFactory : public AbstractSerializerFactory
    {
        Q_OBJECT
        Q_DISABLE_COPY( CoordPlaneSerializerFactory )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( CoordPlaneSerializerFactory )

    public:
        CoordPlaneSerializerFactory( QObject* parent = 0 );
        virtual ~CoordPlaneSerializerFactory();

        AbstractSerializer* instance( const  QString& className ) const;
        QObject* createNewObject( const QString& className ) const;
    };

} // end of namespace

#endif
