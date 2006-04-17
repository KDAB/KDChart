#include <QPainter>
#include <QDebug>
#include <QtXml/QDomDocumentFragment>

#include "KDChartPaintContext.h"
#include "KDChartPieDiagram.h"
#include "KDChartPieDiagram_p.h"
#include "KDChartPainterSaver_p.h"

#define DEGTORAD(d) (d)*M_PI/180.0

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
    QPointF topRight;
    // If we explode, we need extra space for the pie slice that has
    // the largest explosion distance.
    if( explode() ) {
        double maxExplode = 0.0;
        for( int j = 0; j < model()->columnCount(); j++ )
            maxExplode = qMax( maxExplode, explodeFactor( j ) );
        topRight = QPointF( 1.0+maxExplode, 1.0+maxExplode );
    } else
        topRight = QPointF( 1.0, 1.0 );
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
    double startAngle = startPosition();
    double startAngleValueSpace = startPosition();
    for ( int j=0; j<colCount; ++j ) {
        const double nextValue = qAbs( model()->data( model()->index( 0, j ) ).toDouble() );
        double spanAngle = polarCoordinatePlane()->translatePolar( QPointF( nextValue, 1 ) ).x();
        if ( spanAngle == 0 ) continue;
        QBrush brush = model()->headerData( j, Qt::Vertical, KDChart::DatasetBrushRole ).value<QBrush>();
        QPen pen = model()->headerData( j, Qt::Vertical, KDChart::DatasetPenRole ).value<QPen>();
        PainterSaver painterSaver( ctx->painter() );
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( brush );
        ctx->painter()->setPen( pen );
        //qDebug() << "startAngle: " << startAngle << " spanAngle: " << spanAngle;

        // Explosion support
        QRectF pieRect = contentsRect;
        if( explode() ) {
            QPointF oldCenter = contentsRect.center();
            QPointF newCenter = polarCoordinatePlane()->translate( QPointF( explodeFactor( j ),
                                                                            startAngleValueSpace + nextValue/2.0 ) );
            QPointF difference = newCenter - oldCenter;
            pieRect.translate( difference );
        }

        ctx->painter()->drawPie( pieRect, ( int ) ((-startAngle + 90 ) * 16), ( int ) (-spanAngle * 16) );
        startAngle += spanAngle;
        startAngleValueSpace += nextValue;
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







