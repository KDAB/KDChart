#include <QPainter>
#include <QDebug>
#include <QtXml/QDomDocumentFragment>

#include "KDChartPaintContext.h"
#include "KDChartPieDiagram.h"
#include "KDChartPieDiagram_p.h"

using namespace KDChart;

PieDiagram::Private::Private()
{
}

PieDiagram::Private::~Private() {}

#define d d_func()

PieDiagram::PieDiagram( PolarCoordinatePlane* parent ) :
    AbstractPieDiagram( new Private(), parent )
{
    init();
}

PieDiagram::~PieDiagram()
{
}

void PieDiagram::init()
{
}

PieDiagram * PieDiagram::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: PieDiagram * PieDiagram::clone() const" );
    return (PieDiagram*)0xdeadbeef;
}

QDomDocumentFragment PieDiagram::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment PieDiagram::toXML() const" );
    return QDomDocumentFragment();
}

const QPair<QPointF, QPointF> PieDiagram::dataBoundaries () const
{
    QPointF bottomLeft ( QPointF( 0, 0 ) );
    QPointF topRight ( QPointF( 1, 1 ) );
    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}

void PieDiagram::paintEvent( QPaintEvent* )
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

void PieDiagram::resizeEvent ( QResizeEvent*)
{
}

static QRectF buildReferenceRect( const PolarCoordinatePlane* plane )
{
    QRectF contentsRect;
    QPointF referencePointAtTop = plane->translate( QPointF( 1, 0 ) );
    QPointF temp = plane->translate( QPointF( 0, 0 ) ) - referencePointAtTop;
    const double offset = temp.y();
    referencePointAtTop.setX( referencePointAtTop.x() - offset );
    contentsRect.setTopLeft( referencePointAtTop );
    contentsRect.setBottomRight( referencePointAtTop + QPointF( 2*offset, 2*offset) );
    return contentsRect;
}

void PieDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants() ) return;
    const int colCount = model()->columnCount();
    QRectF contentsRect = buildReferenceRect( polarCoordinatePlane() );
    DataValueTextInfoList list;
    double startAngle = 0;
    for ( int j=0; j<colCount; ++j ) {
        const double nextValue = model()->data( model()->index( 0, j ) ).toDouble();
        double spanAngle = polarCoordinatePlane()->translatePolar( QPointF( nextValue, 1 ) ).x();
        if ( spanAngle == 0 ) continue;
        QBrush brush = model()->headerData( j, Qt::Vertical, KDChart::DatasetBrushRole ).value<QBrush>();
        ctx->painter()->save();
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( brush );
        QPen p( ctx->painter()->pen() );
        //p.setColor( brush.color() ); // By Default should not be set - To be defined as a property.
        // FIXME use DatasetPenRole, actually - till
        p.setWidth( 2 );// FIXME properties
        ctx->painter()->setPen( p );
        //qDebug() << "startAngle: " << startAngle << " spanAngle: " << spanAngle;
        ctx->painter()->drawPie( contentsRect, ( int ) ((-startAngle + 90 ) * 16), ( int ) (-spanAngle * 16) );
        ctx->painter()->restore();
        startAngle += spanAngle;
    }
    DataValueTextInfoListIterator it( list );
    while ( it.hasNext() ) {
        const DataValueTextInfo& info = it.next();
        paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
    }
}

void PieDiagram::resize ( const QSizeF& )
{
}

/*virtual*/
double PieDiagram::valueTotals () const
{
    double total = 0;
    const int colCount = model()->columnCount();
    for ( int j=0; j<colCount; ++j ) {
        total += model()->data( model()->index( 0, j ) ).toDouble();
    }
    return total;
}

/*virtual*/
double PieDiagram::numberOfValuesPerDataset() const
{
    return model()->columnCount();
}

/*virtual*/
double PieDiagram::numberOfGridRings() const
{
    return 1;
}







