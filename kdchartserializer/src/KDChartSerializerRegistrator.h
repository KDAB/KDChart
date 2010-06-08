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

#ifndef __KDCHARTSERIALIZERREGISTRATOR_H__
#define __KDCHARTSERIALIZERREGISTRATOR_H__

/** \file KDChartSerializerRegistrator.h
  */

#include <KDChartSerializer>
#include <KDChartAbstractSerializerFactory>

#include <QSet>

namespace KDChart {

    template< class S, class T >
    class SerializerRegistrator : public AbstractSerializerFactory
    {
    public:
        SerializerRegistrator( QObject* parent = 0 )
            : AbstractSerializerFactory( parent )
            , m_instance( new S )
        {
        }

        ~SerializerRegistrator()
        {
            if( m_instance != 0 )
                delete m_instance;
        }

        AbstractSerializer* instance( const QString& className ) const
        {
            if( className == QString::fromLatin1( T::staticMetaObject.className() ) )
                return m_instance;
            return 0;
        }

        QObject* createNewObject( const QString& className ) const
        {
            if( className == QString::fromLatin1( T::staticMetaObject.className() ) )
                return new T;
            return 0;
        }

    private:
        S* m_instance;
    };

    template< class S, class T >
    void registerElementSerializer( QObject* parent )
    {
        Serializer::registerElementSerializerFactory( T::staticMetaObject.className(), new SerializerRegistrator< S, T >( parent ) );
    }

    template< class S, class T >
    void unregisterElementSerializer()
    {
        AbstractSerializerFactory* f = Serializer::elementSerializerFactory( QString::fromLatin1( T::staticMetaObject.className() ) );
        Serializer::unregisterElementSerializerFactory( T::staticMetaObject.className() );
        delete f;
    }

} // end of namespace

#endif
