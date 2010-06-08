#ifndef __KDCHARTTEXTAREASERIALIZERFACTORY_H__
#define __KDCHARTTEXTAREASERIALIZERFACTORY_H__

/** \file KDChartTextAreaSerializerFactory.h
  */

#include <KDChartAbstractSerializerFactory>

namespace KDChart {

    class AbstractSerializer;

    class KDCHARTSERIALIZER_EXPORT TextAreaSerializerFactory : public AbstractSerializerFactory
    {
        Q_OBJECT
        Q_DISABLE_COPY( TextAreaSerializerFactory )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( TextAreaSerializerFactory )

    public:
        TextAreaSerializerFactory( QObject* parent = 0 );
        virtual ~TextAreaSerializerFactory();

        AbstractSerializer* instance( const  QString& className ) const;
        QObject* createNewObject( const QString& className ) const;
    };

} // end of namespace

#endif
