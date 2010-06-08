#include <KDChartAxisSerializerFactory.h>
#include <KDChartAxisSerializerFactory_p.h>

#include <KDChartAxesSerializer.h>

#include <KDChartCartesianAxis>
//#include <KDChartPolarAxis>

#define d d_func()

using namespace KDChart;

AxisSerializerFactory::Private::Private( AxisSerializerFactory* qq )
    : q( qq ),
      m_instance( 0 )
{
}

AxisSerializerFactory::Private::~Private()
{
    if( m_instance != 0 )
        delete m_instance;
}

AxisSerializerFactory::AxisSerializerFactory( QObject* parent )
    : AbstractSerializerFactory( parent ),
      _d( new Private( this ) )
{
}

AxisSerializerFactory::~AxisSerializerFactory()
{
    delete _d; _d = 0;
}

void AxisSerializerFactory::init()
{
}

AbstractSerializer* AxisSerializerFactory::instance( const QString& className ) const
{
    Q_UNUSED( className );

    if( d->m_instance == 0 )
        d->m_instance = new AxesSerializer;

    return d->m_instance;
}

QObject* AxisSerializerFactory::createNewObject( const QString& className ) const
{
    if( className == CartesianAxis::staticMetaObject.className() )
        return new CartesianAxis;
//    if( className == PolarAxis::staticMetaObject.className() )
//        return new PolarAxis;
    return 0;
}
