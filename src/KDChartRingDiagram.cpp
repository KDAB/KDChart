#include <QtXml/QDomDocumentFragment>
#include <QPainter>

#include "KDChartPaintContext.h"
#include "KDChartRingDiagram.h"
#include "KDChartRingDiagram_p.h"

using namespace KDChart;

RingDiagram::Private::Private() :
    relativeThickness( false )
{
}

RingDiagram::Private::~Private() {}

#define d d_func()

RingDiagram::RingDiagram( PolarCoordinatePlane* parent ) :
    AbstractPieDiagram( new Private(), parent )
{
    init();
}

RingDiagram::~RingDiagram()
{
}

void RingDiagram::init()
{
}

RingDiagram * RingDiagram::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: RingDiagram * RingDiagram::clone() const" );
    return (RingDiagram*)0xdeadbeef;
}

QDomDocumentFragment RingDiagram::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment RingDiagram::toXML() const" );
    return QDomDocumentFragment();
}

void RingDiagram::setRelativeThickness( bool relativeThickness )
{
    d->relativeThickness = relativeThickness;
}

bool RingDiagram::relativeThickness() const
{
    return d->relativeThickness;
}

const QPair<QPointF, QPointF> RingDiagram::dataBoundaries () const
{
    if ( !checkInvariants() ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );

    QPointF bottomLeft ( QPointF( 0, 0 ) );
    QPointF topRight ( QPointF( 1, 1 ) );
    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}

void RingDiagram::paintEvent( QPaintEvent* )
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

void RingDiagram::resizeEvent ( QResizeEvent*)
{
}

void RingDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants() ) return;
    const int colCount = model()->columnCount();
    DataValueTextInfoList list;
    for ( int j=0; j<colCount; ++j ) {
        QBrush brush = model()->headerData( j, Qt::Vertical, KDChart::DatasetBrushRole ).value<QBrush>();
        ctx->painter()->save();
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( brush );
        QPen p( ctx->painter()->pen() );
        p.setColor( brush.color() );
        p.setWidth( 2 );// FIXME properties, use DatasetPenRole
        ctx->painter()->setPen( p );
        //ctx->painter()->drawPolyline( polygon );
        ctx->painter()->restore();
    }
    DataValueTextInfoListIterator it( list );
    while ( it.hasNext() ) {
        const DataValueTextInfo& info = it.next();
        paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
    }
}

void RingDiagram::resize ( const QSizeF& )
{
}

/*virtual*/
double RingDiagram::valueTotals () const
{
    double total = 0;
    const int colCount = model()->columnCount();
    for ( int j=0; j<colCount; ++j ) {
        total += model()->data( model()->index( 0, j ) ).toDouble();
    }
    return total;
}

/*virtual*/
double RingDiagram::numberOfValuesPerDataset() const
{
    return model()->columnCount();
}

/*virtual*/
double RingDiagram::numberOfGridRings() const
{
    return 1;
}

