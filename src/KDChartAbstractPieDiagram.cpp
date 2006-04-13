#include <QMap>
#include <QtXml/QDomDocumentFragment>

#include "KDChartAbstractPieDiagram.h"
#include "KDChartAbstractPieDiagram_p.h"
#include "KDChartAttributesModel.h"

using namespace KDChart;

AbstractPieDiagram::Private::Private() :
    explode( false ),
    startPosition( 0.0 )
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

void AbstractPieDiagram::setExplodeFactor( double factor )
{
    d->attributesModel->setModelData( factor, ExplodeFactorRole );
}

double AbstractPieDiagram::explodeFactor() const
{
    return d->attributesModel->modelData( ExplodeFactorRole ).toDouble();
}

void AbstractPieDiagram::setExplodeFactor( int dataset, double factor )
{
    d->attributesModel->setHeaderData( dataset, Qt::Vertical, factor, ExplodeFactorRole );
}

double AbstractPieDiagram::explodeFactor( int dataset ) const
{
    return d->attributesModel->headerData( dataset, Qt::Vertical, ExplodeFactorRole ).toDouble();
}

void AbstractPieDiagram::setStartPosition( double degrees )
{
    d->startPosition = degrees;
}

double AbstractPieDiagram::startPosition() const
{
    return d->startPosition;
}


























