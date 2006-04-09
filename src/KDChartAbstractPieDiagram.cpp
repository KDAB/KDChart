#include <QMap>
#include <QtXml/QDomDocumentFragment>

#include <KDChartThreeDAttributes.h>

#include "KDChartAbstractPieDiagram.h"
#include "KDChartAbstractPieDiagram_p.h"

using namespace KDChart;

AbstractPieDiagram::Private::Private() :
    explode( false ),
    explodeValues(),
    explodeFactors(),
    explodeFactor( 0.1 ),
    startPosition( 0 )
{
}

AbstractPieDiagram::Private::~Private() {}

AbstractPieDiagram::AbstractPieDiagram( PolarCoordinatePlane *parent ) :
    AbstractPolarDiagram( new Private(), parent )
{
    init();
}

AbstractPieDiagram::~AbstractPieDiagram()
{
}


void AbstractPieDiagram::init()
{
}


#define d d_func()


QDomDocumentFragment AbstractPieDiagram::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment AbstractPieDiagram::toXML() const" );
    return QDomDocumentFragment();
}

void AbstractPieDiagram::setExplode( bool explode )
{
    d->explode = explode;
}

bool AbstractPieDiagram::explode() const
{
    return d->explode;
}

void AbstractPieDiagram::setExplodeValues( const QList<int>& explodeList )
{
    d->explodeValues = explodeList;
}

QList<int> AbstractPieDiagram::explodeValues() const
{
    return d->explodeValues;
}

void AbstractPieDiagram::setExplodeFactors( AbstractPieDiagram::ExplodeFactorsMap factors )
{
    d->explodeFactors = factors;
}

AbstractPieDiagram::ExplodeFactorsMap AbstractPieDiagram::explodeFactors() const
{
    return d->explodeFactors;
}

void AbstractPieDiagram::setExplodeFactor( double factor )
{
    d->explodeFactor = factor;
}

double AbstractPieDiagram::explodeFactor() const
{
    return d->explodeFactor;
}

void AbstractPieDiagram::setStartPosition( int degrees )
{
    d->startPosition = degrees;
}

int AbstractPieDiagram::startPosition() const
{
    return d->startPosition;
}


























