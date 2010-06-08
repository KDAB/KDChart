#include <KDChartLegendSerializerFactory.h>
#include <KDChartLegendSerializerFactory_p.h>

#include <KDChartLegendsSerializer.h>

#include <KDChartLegend>

#define d d_func()

using namespace KDChart;

LegendSerializerFactory::Private::Private( LegendSerializerFactory* qq )
    : q( qq ),
      m_instance( 0 )
{
}

LegendSerializerFactory::Private::~Private()
{
    if( m_instance != 0 )
        delete m_instance;
}

LegendSerializerFactory::LegendSerializerFactory( QObject* parent )
    : AbstractSerializerFactory( parent ),
      _d( new Private( this ) )
{
}

LegendSerializerFactory::~LegendSerializerFactory()
{
    delete _d; _d = 0;
}

void LegendSerializerFactory::init()
{
}

AbstractSerializer* LegendSerializerFactory::instance( const QString& className ) const
{
    Q_UNUSED( className );

    if( d->m_instance == 0 )
        d->m_instance = new LegendsSerializer;

    return d->m_instance;
}

QObject* LegendSerializerFactory::createNewObject( const QString& className ) const
{
    if( className == Legend::staticMetaObject.className() )
        return new Legend;
    return 0;
}
