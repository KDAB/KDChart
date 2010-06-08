#include <KDChartDiagramSerializerFactory.h>
#include <KDChartDiagramSerializerFactory_p.h>

#include <KDChartDiagramsSerializer.h>

#include <KDChartCoordPlanesSerializer.h>

#include <KDChartLineDiagram>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartRingDiagram>
#include <KDChartBarDiagram>
#include <KDChartStockDiagram>
#include <KDChartPlotter>

#define d d_func()

using namespace KDChart;

DiagramSerializerFactory::Private::Private( DiagramSerializerFactory* qq )
    : q( qq ),
      m_instance( 0 )
{
}

DiagramSerializerFactory::Private::~Private()
{
    if( m_instance != 0 )
        delete m_instance;
}

DiagramSerializerFactory::DiagramSerializerFactory( QObject* parent )
    : AbstractSerializerFactory( parent ),
      _d( new Private( this ) )
{
}

DiagramSerializerFactory::~DiagramSerializerFactory()
{
    delete _d; _d = 0;
}

void DiagramSerializerFactory::init()
{
}

AbstractSerializer* DiagramSerializerFactory::instance( const QString& className ) const
{
    Q_UNUSED( className );

    if( d->m_instance == 0 )
        d->m_instance = new DiagramsSerializer;

    return d->m_instance;
}

QObject* DiagramSerializerFactory::createNewObject( const QString& className ) const
{
    if( className == LineDiagram::staticMetaObject.className() )
        return new LineDiagram;
    if( className == PieDiagram::staticMetaObject.className() )
        return new PieDiagram;
    if( className == PolarDiagram::staticMetaObject.className() )
        return new PolarDiagram;
    if( className == RingDiagram::staticMetaObject.className() )
        return new RingDiagram;
    if( className == BarDiagram::staticMetaObject.className() )
        return new BarDiagram;
    if( className == StockDiagram::staticMetaObject.className() )
        return new StockDiagram;
    if( className == Plotter::staticMetaObject.className() )
        return new Plotter;
    return 0;
}
