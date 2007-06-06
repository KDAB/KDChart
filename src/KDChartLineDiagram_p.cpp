#include "KDChartLineDiagram.h"
#include "KDChartDataValueAttributes.h"

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
    QPointF point, QPointF maxLimits,
    double z, const QModelIndex& index ) const
{
    ThreeDLineAttributes td = diagram->threeDLineAttributes( index );

    //Pending Michel FIXME - the rotation does not work as expected atm
    double xrad = DEGTORAD( td.lineXRotation() );
    double yrad = DEGTORAD( td.lineYRotation() );
    QPointF ret = QPointF(point.x()*cos( yrad ) + z * sin( yrad ) ,  point.y()*cos( xrad ) - z * sin( xrad ) );
    return ret;
}

void LineDiagram::Private::paintThreeDLines(
    PaintContext* ctx, const QModelIndex& index,
    const QPointF& from, const QPointF& to, const double depth  )
{
    // retrieve the boundaries
    const QPair<QPointF, QPointF> boundaries = diagram->dataBoundaries ();
    QPointF maxLimits = boundaries.second;
    QVector <QPointF > segmentPoints;
    QPointF topLeft = project( from, maxLimits, depth, index  );
    QPointF topRight = project ( to, maxLimits, depth, index  );

    segmentPoints << from << topLeft << topRight << to;
    QPolygonF segment ( segmentPoints );
    QBrush indexBrush ( diagram->brush( index ) );
    PainterSaver painterSaver( ctx->painter() );
    if ( diagram->antiAliasing() )
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    ctx->painter()->setBrush( indexBrush );
    ctx->painter()->setPen( diagram->pen( index ) ) ;
    ctx->painter()->drawPolygon( segment );
}

// this method is factored out from LineDiagram::paint, and contains
// the common parts of the method that  previously implemented all
// chart types in one
void LineDiagram::LineDiagramType::paintElements(
    PaintContext* ctx,
    DataValueTextInfoList& list,
    LineAttributesInfoList& lineList,
    LineAttributes::MissingValuesPolicy policy )
{
    // paint all lines and their attributes
    PainterSaver painterSaver( ctx->painter() );
    if ( diagram()->antiAliasing() )
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    LineAttributesInfoListIterator itline ( lineList );

    QBrush curBrush;
    QPen curPen;
    QPolygonF points;
    while ( itline.hasNext() ) {
        const LineAttributesInfo& lineInfo = itline.next();
        const QModelIndex& index = lineInfo.index;
        const ThreeDLineAttributes td = diagram()->threeDLineAttributes( index );
        if( td.isEnabled() ){
            paintThreeDLines( ctx, index, lineInfo.value, lineInfo.nextValue, td.depth() );
        }else{
            const QBrush br( diagram()->brush( index ) );
            const QPen pn( diagram()->pen( index ) );
            if( points.count() && points.last() == lineInfo.value && curBrush == br && curPen == pn ){
                points << lineInfo.nextValue;
            }else{
                if( points.count() )
                    paintPolyline( ctx, curBrush, curPen, points );
                curBrush = br;
                curPen   = pn;
                points.clear();
                points << lineInfo.value << lineInfo.nextValue;
            }
        }
    }
    if( points.count() )
        paintPolyline( ctx, curBrush, curPen, points );
    // paint all data value texts and the point markers
    paintDataValueTextsAndMarkers( diagram(), ctx, list, true );
}

AttributesModel* LineDiagram::LineDiagramType::attributesModel() const
{
    return m_private->attributesModel;
}

QModelIndex LineDiagram::LineDiagramType::attributesModelRootIndex() const
{
    return m_private->diagram->attributesModelRootIndex();
}

int LineDiagram::LineDiagramType::datasetDimension() const
{
    return m_private->datasetDimension;
}

LineAttributes::MissingValuesPolicy LineDiagram::LineDiagramType::getCellValues(
    int row, int column,
    bool shiftCountedXValuesByHalfSection,
    double& valueX, double& valueY ) const
{
    return m_private->diagram->getCellValues( row, column, shiftCountedXValuesByHalfSection,
                                              valueX, valueY );
}

double LineDiagram::LineDiagramType::valueForCellTesting(
    int row, int column,
    bool& bOK,
    bool showHiddenCellsAsInvalid) const
{
    return m_private->diagram->valueForCellTesting( row, column, bOK, showHiddenCellsAsInvalid );
}

LineDiagram* LineDiagram::LineDiagramType::diagram()
{
    return m_private->diagram;
}

void LineDiagram::LineDiagramType::paintAreas(
    PaintContext* ctx,
    const QModelIndex& index, const QList<QPolygonF>& areas,
    const uint transparency )
{
    QColor trans( diagram()->brush(index).color() );
    trans.setAlpha( transparency );
    QPen indexPen( diagram()->pen(index) );
    indexPen.setColor( trans );
    PainterSaver painterSaver( ctx->painter() );
    if ( diagram()->antiAliasing() )
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    ctx->painter()->setPen( indexPen );
    ctx->painter()->setBrush( trans );
    QPainterPath path;
    for( int i=0; i<areas.count(); ++i ){
        path.addPolygon( areas[i] );
        path.closeSubpath();
    }
    ctx->painter()->drawPath( path );
}

double LineDiagram::LineDiagramType::valueForCell( int row, int column )
{
    return diagram()->valueForCell( row, column );
}

void LineDiagram::LineDiagramType::appendDataValueTextInfoToList(
            AbstractDiagram * diagram,
            DataValueTextInfoList & list,
            const QModelIndex & index,
            const PositionPoints& points,
            const Position& autoPositionPositive,
            const Position& autoPositionNegative,
            const qreal value )
{
    m_private->appendDataValueTextInfoToList( diagram, list, index, points,
                                              autoPositionPositive, autoPositionPositive, value );
}

