#include <KDChartTextAreaSerializerFactory.h>
#include <KDChartTextAreaSerializerFactory_p.h>

#include <KDChartTextAreaSerializer.h>

#include <KDChartHeaderFooter>

#define d d_func()

using namespace KDChart;

TextAreaSerializerFactory::Private::Private( TextAreaSerializerFactory* qq )
    : q( qq ),
      m_instance( 0 )
{
}

TextAreaSerializerFactory::Private::~Private()
{
    if( m_instance != 0 )
        delete m_instance;
}

TextAreaSerializerFactory::TextAreaSerializerFactory( QObject* parent )
    : AbstractSerializerFactory( parent ),
      _d( new Private( this ) )
{
}

TextAreaSerializerFactory::~TextAreaSerializerFactory()
{
    delete _d; _d = 0;
}

void TextAreaSerializerFactory::init()
{
}

AbstractSerializer* TextAreaSerializerFactory::instance( const QString& className ) const
{
    Q_UNUSED( className );

    if( d->m_instance == 0 )
        d->m_instance = new TextAreaSerializer;

    return d->m_instance;
}

QObject* TextAreaSerializerFactory::createNewObject( const QString& className ) const
{
    if( className == HeaderFooter::staticMetaObject.className() )
        return new HeaderFooter;
    return 0;
}
