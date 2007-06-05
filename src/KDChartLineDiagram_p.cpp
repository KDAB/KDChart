#include "KDChartLineDiagram.h"
#include "KDChartLineDiagram_p.h"

using namespace KDChart;

LineDiagram::Private::Private( const Private& rhs )
    : AbstractCartesianDiagram::Private( rhs )
{
}

void LineDiagram::Private::paintPolyline(
    PaintContext* ctx,
    const QBrush& brush, const QPen& pen,
    const QPolygonF& points ) const
{
    ctx->painter()->setBrush( brush );
    ctx->painter()->setPen(
        QPen( pen.color(),
              pen.width(),
              pen.style(),
              Qt::FlatCap,
              Qt::MiterJoin ) );
#if QT_VERSION > 0x040299
    ctx->painter()->drawPolyline( points );
#else
    // FIXME (Mirko) verify, this sounds reverse-logical
    // For Qt versions older than 4.3 drawPolyline is VERY slow
    // so we use traditional line segments drawing instead then.
    for (int i = 0; i < points.size()-1; ++i)
        ctx->painter()->drawLine( points.at(i), points.at(i+1) );
#endif
}

/*!
  Projects a point in a space defined by its x, y, and z coordinates
  into a point onto a plane, given two rotation angles around the x
  resp. y axis.
*/
const QPointF LineDiagram::Private::project(
    LineDiagram* that, QPointF point, QPointF maxLimits,
    double z, const QModelIndex& index ) const
{
    ThreeDLineAttributes td = that->threeDLineAttributes( index );

    //Pending Michel FIXME - the rotation does not work as expected atm
    double xrad = DEGTORAD( td.lineXRotation() );
    double yrad = DEGTORAD( td.lineYRotation() );
    QPointF ret = QPointF(point.x()*cos( yrad ) + z * sin( yrad ) ,  point.y()*cos( xrad ) - z * sin( xrad ) );
    return ret;
}

void LineDiagram::Private::paintThreeDLines(
    LineDiagram* that, PaintContext* ctx, const QModelIndex& index,
    const QPointF& from, const QPointF& to, const double depth  )
{
    // retrieve the boundaries
    const QPair<QPointF, QPointF> boundaries = that->dataBoundaries ();
    QPointF maxLimits = boundaries.second;
    QVector <QPointF > segmentPoints;
    QPointF topLeft = project( that, from, maxLimits, depth, index  );
    QPointF topRight = project ( that, to, maxLimits, depth, index  );

    segmentPoints << from << topLeft << topRight << to;
    QPolygonF segment ( segmentPoints );
    QBrush indexBrush ( that->brush( index ) );
    PainterSaver painterSaver( ctx->painter() );
    if ( that->antiAliasing() )
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    ctx->painter()->setBrush( indexBrush );
    ctx->painter()->setPen( that->pen( index ) ) ;
    ctx->painter()->drawPolygon( segment );
}

// seems to be unused, praise pimpling:
//         void paintAreas( LineDiagram* that, PaintContext* ctx, const QModelIndex& index, const QPolygonF& area, const uint transparency )
//         {
//             QColor trans( that->brush(index).color() );
//             QPen indexPen( that->pen(index) );
//             trans.setAlpha( transparency );
//             indexPen.setColor( trans );
//             PainterSaver painterSaver( ctx->painter() );
//             if ( that->antiAliasing() )
//                 ctx->painter()->setRenderHint ( QPainter::Antialiasing );
//             ctx->painter()->setPen( indexPen );
//             ctx->painter()->setBrush( trans ) ;
//             ctx->painter()->drawPolygon( area );//pol );
//         }

void LineDiagram::Private::paintAreas(
    LineDiagram* that, PaintContext* ctx,
    const QModelIndex& index, const QList<QPolygonF>& areas,
    const uint transparency )
{
    QColor trans( that->brush(index).color() );
    trans.setAlpha( transparency );
    QPen indexPen( that->pen(index) );
    indexPen.setColor( trans );
    PainterSaver painterSaver( ctx->painter() );
    if ( that->antiAliasing() )
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    ctx->painter()->setPen( indexPen );
    ctx->painter()->setBrush( trans );
    QPainterPath path;
    for( int i=0; i<areas.count(); ++i ){
        path.addPolygon( areas[i] );
        path.closeSubpath();
        //qDebug() << "LineDiagram::paintAreas() adding path:"<<areas[i];
    }
    //qDebug() << endl;
    ctx->painter()->drawPath( path );
}

// publish attributes to the diagram type implementations:
// (some of those should have been in p in the first place)
QModelIndex LineDiagram::Private::attributesModelRootIndex() const
{
    return diagram->attributesModelRootIndex();
}

double LineDiagram::Private::valueForCellTesting(
    int row, int column,
    bool& bOK,
    bool showHiddenCellsAsInvalid) const
{
    return diagram->valueForCellTesting( row, column, bOK, showHiddenCellsAsInvalid );
}

LineAttributes::MissingValuesPolicy
LineDiagram::Private::getCellValues(
    int row, int column,
    bool shiftCountedXValuesByHalfSection,
    double& valueX, double& valueY ) const
{
    return diagram->getCellValues( row, column, shiftCountedXValuesByHalfSection,
                                   valueX, valueY );
}

double LineDiagram::Private::valueForCell( int row, int column ) const
{
    return diagram->valueForCell( row, column );
}


