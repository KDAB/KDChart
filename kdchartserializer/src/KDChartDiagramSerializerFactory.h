#ifndef __KDCHARTDIAGRAMSERIALIZERFACTORY_H__
#define __KDCHARTDIAGRAMSERIALIZERFACTORY_H__

/** \file KDChartDiagramsSerializerFactory.h
  */

#include <KDChartAbstractSerializerFactory>

namespace KDChart {

    class AbstractSerializer;

    class KDCHARTSERIALIZER_EXPORT DiagramSerializerFactory : public AbstractSerializerFactory
    {
        Q_OBJECT
        Q_DISABLE_COPY( DiagramSerializerFactory )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( DiagramSerializerFactory )

    public:
        DiagramSerializerFactory( QObject* parent = 0 );
        virtual ~DiagramSerializerFactory();

        AbstractSerializer* instance( const  QString& className ) const;
        QObject* createNewObject( const QString& className ) const;
    };

} // end of namespace

#endif
