
#include <QPainter>
#include <QtXml/QDomDocumentFragment>
#include "KDChartPaintContext.h"
#include "KDChartPolarDiagram.h"
#include "KDChartPolarDiagram_p.h"

using namespace KDChart;

PolarDiagram::Private::Private() :
    zeroDegreePosition( 0 ),
    rotateCircularLabels( false )
{
}

PolarDiagram::Private::~Private() {}

#define d d_func()

PolarDiagram::PolarDiagram( PolarCoordinatePlane* plane ) :
    AbstractPolarDiagram( new Private( ), plane )
{
}

PolarDiagram::~PolarDiagram()
{
}


void PolarDiagram::init()
{
    setShowDelimitersAtPosition( KDChartArea::TopLeft, false );
    setShowDelimitersAtPosition( KDChartArea::Top, true );
    setShowDelimitersAtPosition( KDChartArea::TopRight, false );
    setShowDelimitersAtPosition( KDChartArea::Left, false );
    setShowDelimitersAtPosition( KDChartArea::Right, false );
    setShowDelimitersAtPosition( KDChartArea::BottomLeft, false );
    setShowDelimitersAtPosition( KDChartArea::Bottom, true );
    setShowDelimitersAtPosition( KDChartArea::BottomRight, false );
    setShowLabelsAtPosition( KDChartArea::TopLeft, false );
    setShowLabelsAtPosition( KDChartArea::Top, true );
    setShowLabelsAtPosition( KDChartArea::TopRight, false );
    setShowLabelsAtPosition( KDChartArea::Left, false );
    setShowLabelsAtPosition( KDChartArea::Right, false );
    setShowLabelsAtPosition( KDChartArea::BottomLeft, false );
    setShowLabelsAtPosition( KDChartArea::Bottom, true );
    setShowLabelsAtPosition( KDChartArea::BottomRight, false );
}




PolarDiagram * PolarDiagram::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: PolarDiagram * PolarDiagram::clone() const" );
    return (PolarDiagram*)0xdeadbeef;
}

QDomDocumentFragment PolarDiagram::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment PolarDiagram::toXML() const" );
    return QDomDocumentFragment();
}

const QPair<QPointF, QPointF> PolarDiagram::dataBoundaries () const
{
    if ( !checkInvariants() ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );
    const int rowCount = model()->rowCount();
    const int colCount = model()->columnCount();
    double xMin = 0.0;
    double xMax = colCount;
    double yMin = 0, yMax = 0;
    for ( int j=0; j<colCount; ++j ) {
        for ( int i=0; i< rowCount; ++i ) {
            double value = model()->data( model()->index( i, j, QModelIndex() ) ).toDouble();
            yMax = qMax( yMax, value );
        }
    }
    QPointF bottomLeft ( QPointF( xMin, yMin ) );
    QPointF topRight ( QPointF( xMax, yMax ) );
    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}



void PolarDiagram::paintEvent ( QPaintEvent*)
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

void PolarDiagram::resizeEvent ( QResizeEvent*)
{
}

void PolarDiagram::paintMarkers( PaintContext* ctx, const QPolygonF& polygon )
{
    const double markerSize = 4; // FIXME use real markers
    for ( int i=0; i<polygon.size(); ++i ) {
        QPointF p = polygon.at( i );
        p.setX( p.x() - markerSize/2 );
        p.setY( p.y() - markerSize/2 );
        ctx->painter()->drawRect( QRectF( p, QSizeF( markerSize, markerSize ) ) );
    }
}

void PolarDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants() ) return;
    const int rowCount = model()->rowCount();
    const int colCount = model()->columnCount();
    DataValueTextInfoList list;
    for ( int j=0; j<colCount; ++j ) {
        QBrush brush = model()->headerData( j, Qt::Vertical, KDChart::DatasetBrushRole ).value<QBrush>();
        QPolygonF polygon;
        for ( int i=0; i<rowCount; ++i ) {
            QModelIndex index = model()->index( i, j, QModelIndex() );
            const double value = model()->data( index ).toDouble();
            QPointF point = coordinatePlane()->translate( QPointF( value, i ) );
            polygon.append( point );
            list.append( DataValueTextInfo( index, point, value ) );
        }
        ctx->painter()->save();
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( brush );
        QPen p( ctx->painter()->pen() );
        p.setColor( brush.color() ); // FIXME use DatasetPenRole
        p.setWidth( 2 );// FIXME properties
        ctx->painter()->setPen( p );
        ctx->painter()->drawPolyline( polygon );
        paintMarkers( ctx, polygon );
        ctx->painter()->restore();
    }
    DataValueTextInfoListIterator it( list );
    while ( it.hasNext() ) {
        const DataValueTextInfo& info = it.next();
        paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
    }
}

void PolarDiagram::resize ( const QSizeF& )
{
}

/*virtual*/ 
double PolarDiagram::valueTotals () const
{
    return model()->rowCount();
}

/*virtual*/
double PolarDiagram::numberOfValuesPerDataset() const
{
    return model()->rowCount();
}

/*virtual*/ 
double PolarDiagram::numberOfGridRings() const
{
    return 5; // FIXME
}

void PolarDiagram::setZeroDegreePosition( int degrees )
{
    d->zeroDegreePosition = degrees;
}

int PolarDiagram::zeroDegreePosition() const
{
    return d->zeroDegreePosition;
}

void PolarDiagram::setRotateCircularLabels( bool rotateCircularLabels )
{
    d->rotateCircularLabels = rotateCircularLabels;
}

bool PolarDiagram::rotateCircularLabels() const
{
    return d->rotateCircularLabels;
}

void PolarDiagram::setShowDelimitersAtPosition( KDChartArea::DockingPointType position,
                                                       bool showDelimiters )
{
    d->showDelimitersAtPosition[position] = showDelimiters;
}

void PolarDiagram::setShowLabelsAtPosition( KDChartArea::DockingPointType position,
                                                   bool showLabels )
{
    d->showLabelsAtPosition[position] = showLabels;
}

bool PolarDiagram::showDelimitersAtPosition( KDChartArea::DockingPointType position ) const
{
    return d->showDelimitersAtPosition[position];
}

bool PolarDiagram::showLabelsAtPosition( KDChartArea::DockingPointType position ) const
{
    return d->showLabelsAtPosition[position];
}


























