#include <KDChartCoordPlaneSerializerFactory.h>
#include <KDChartCoordPlaneSerializerFactory_p.h>

#include <KDChartCoordPlanesSerializer.h>

#include <KDChartCartesianCoordinatePlane>
#include <KDChartPolarCoordinatePlane>

#define d d_func()

using namespace KDChart;

CoordPlaneSerializerFactory::Private::Private( CoordPlaneSerializerFactory* qq )
    : q( qq ),
      m_instance( 0 )
{
}

CoordPlaneSerializerFactory::Private::~Private()
{
    if( m_instance != 0 )
        delete m_instance;
}

CoordPlaneSerializerFactory::CoordPlaneSerializerFactory( QObject* parent )
    : AbstractSerializerFactory( parent ),
      _d( new Private( this ) )
{
}

CoordPlaneSerializerFactory::~CoordPlaneSerializerFactory()
{
    delete _d; _d = 0;
}

void CoordPlaneSerializerFactory::init()
{
}

AbstractSerializer* CoordPlaneSerializerFactory::instance( const QString& className ) const
{
    Q_UNUSED( className );

    if( d->m_instance == 0 )
        d->m_instance = new CoordPlanesSerializer;

    return d->m_instance;
}

QObject* CoordPlaneSerializerFactory::createNewObject( const QString& className ) const
{
    if( className == CartesianCoordinatePlane::staticMetaObject.className() )
        return new CartesianCoordinatePlane;
    else if( className == PolarCoordinatePlane::staticMetaObject.className() )
        return new PolarCoordinatePlane;
    return 0;
}
