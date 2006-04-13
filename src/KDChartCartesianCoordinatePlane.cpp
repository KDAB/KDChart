#include <QFont>
#include <QList>
#include <QtDebug>
#include <QPainter>

#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartCartesianCoordinatePlane.h"
#include "CartesianCoordinateTransformation.h"
#include "KDChartGridAttributes.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"

using namespace KDChart;

class CartesianCoordinatePlane::Private
{
public:
    // the coordinate plane will calculate the coordinate transformation:
    CoordinateTransformation coordinateTransformation;
    // the whole drawing area, includes diagrams and axes, but maybe smaller
    // than (width, height):
    QRectF drawingArea;
    // the reactangle occupied by the diagrams, in plane coordinates
    QRectF diagramArea;
    // true after the first resize event came in
    bool initialResizeEventReceived;
    // true if the coordinate plane scales isometrically
    bool isometricScaling;

    Private()
        : initialResizeEventReceived ( false )
        , isometricScaling ( false )
    {}
};


CartesianCoordinatePlane::CartesianCoordinatePlane ( QWidget* parent )
    : CoordinatePlane ( parent )
    , d ( new Private() )
{
}

CartesianCoordinatePlane::~CartesianCoordinatePlane()
{
}

void CartesianCoordinatePlane::addDiagram ( AbstractDiagram* diagram )
{
    Q_ASSERT_X ( dynamic_cast<AbstractCartesianDiagram*> ( diagram ),
                 "CartesianCoordinatePlane::addDiagram", "Only cartesian "
                 "diagrams can be added to a cartesian coordinate plane!" );
    CoordinatePlane::addDiagram ( diagram );
    connect ( diagram,  SIGNAL ( layoutChanged ( AbstractDiagram* ) ),
              SLOT ( slotLayoutChanged ( AbstractDiagram* ) ) );
}

#define DEBUG_PAINTING 0
void CartesianCoordinatePlane::paintEvent ( QPaintEvent* )
{
    QPainter painter ( this );
#ifdef DEBUG_PAINTING
    if ( true ) // this should be a little helper method somewhere:
    {
        QString text = tr( "CartesianCoordinatePlane" );
        if ( diagrams().isEmpty() ) text += tr ( " (no diagrams)" );
        PainterSaver painterSaver( &painter );
        painter.setRenderHint ( QPainter::TextAntialiasing );
        painter.setPen ( "gold" );
        painter.setBrush (  Qt::NoBrush );
        painter.drawRect ( 0, 0, width() - 1,  height() - 1);
        QFont font = painter.font();
        font.setPointSize ( 7 );
        painter.setFont( font );
        painter.setPen ( Qt::lightGray );
        painter.drawText ( 0, 0, width() - 3 , height() - 3, Qt::AlignRight | Qt::AlignTop, text );
    }
#endif
    AbstractDiagramList diags = diagrams();
    if ( !diags.isEmpty() )
    {
        // paint the coordinate system rulers:
        PaintContext ctx;
        ctx.setRectangle ( d->drawingArea );
        ctx.setPainter ( &painter );
        ctx.setCoordinatePlane ( this );
        //paintGrid( &ctx );

        // paint the diagrams:
        for ( int i = 0; i < diags.size(); i++ )
        {
            PainterSaver painterSaver( &painter );
            diags[i]->paint ( &ctx );
        }
    }
}

void CartesianCoordinatePlane::paintGrid( PaintContext* ctx )
{
    // FIXME accumulate over all diagrams
    const GridAttributes a = gridAttributes();
    AbstractCartesianDiagram* dgr = dynamic_cast<AbstractCartesianDiagram*> (diagrams().first() );
    Q_ASSERT ( dgr ); // only cartesian diagrams are allowed here

    const int numberOfAbscissaSegments = dgr->numberOfAbscissaSegments();
    const int numberOfOrdinateSegments = dgr->numberOfOrdinateSegments();
    QRectF diagramRect = d->coordinateTransformation.diagramRect;
    bool hasVisibleAbscissa = ( d->coordinateTransformation.unitVectorX
                                * ( diagramRect.left() - diagramRect.right() ) ) <= 0;
    bool hasVisibleOrdinate = ( d->coordinateTransformation.unitVectorY
                                * ( diagramRect.top() - diagramRect.bottom () ) ) <= 0;


    // the axes and rulers to draw, this can be cached:
    QVector<QLineF> axes;
    QVector<QLineF> rulers;

    // draw the abscissa and ordinate, if they are in the visible range:
    if ( hasVisibleAbscissa )
    {
        QLineF l2r ( translate ( QPointF ( diagramRect.left(), 0 ) ),
                     translate ( QPointF ( diagramRect.right(), 0 ) ) );
        axes.append ( l2r );
    }

    if ( hasVisibleOrdinate )
    {
        QLineF t2b ( translate ( QPointF ( 0, diagramRect.top() ) ),
                     translate ( QPointF ( 0, diagramRect.bottom() ) ) );
        axes.append ( t2b );
    }

    double step = diagramRect.width() / ( numberOfAbscissaSegments - 1 );
    double x = diagramRect.left();
    for ( int i = 0; i < numberOfAbscissaSegments; ++i )
    {
        QLineF l ( translate ( QPointF ( x, diagramRect.top() ) ),
                   translate ( QPointF ( x,  diagramRect.bottom() ) ) );
        rulers.append (l );
        x += step;
    }

    step = diagramRect.height() / ( numberOfOrdinateSegments - 1 );
    double y = diagramRect.top();
    for ( int i = 0; i < numberOfOrdinateSegments; ++i )
    {
        QLineF l ( translate ( QPointF ( diagramRect.left(), y ) ),
                   translate ( QPointF ( diagramRect.right(), y ) ) );
        rulers.append (l );
        y += step;
    }

    if ( a.isSubGridVisible() ) {
        ctx->painter()->setPen ( a.subGridPen() );
        ctx->painter()->drawLines ( rulers );
    }
    if ( a.isGridVisible() ) {
        ctx->painter()->setPen ( a.gridPen() );
        ctx->painter()->drawLines ( axes );
    }
}

void CartesianCoordinatePlane::resizeEvent ( QResizeEvent* )
{
    d->initialResizeEventReceived = true;
    layoutDiagrams();
}

void CartesianCoordinatePlane::slotLayoutChanged ( AbstractDiagram* )
{
    if ( d->initialResizeEventReceived ) layoutDiagrams();
}

void CartesianCoordinatePlane::layoutDiagrams()
{
    if ( diagrams().isEmpty() )
    {   // FIXME evaluate what can still be prepared
        return;
    }
    // the rectangle the diagrams cover in the *plane*:
    // (Why -3? We save 1px on each side for the antialiased drawing, and
    // respect the way QPainter calculates the width of a painted rect (the
    // size is the rectangle size plus the pen width). This way, most clipping
    // for regular pens should be avoided. When pens with a penWidth or larger
    // than 1 are used, this may not be sufficient.
    d->drawingArea = QRectF ( 1, 1, width() - 3, height() - 3 );

    // determine unit of the rectangles of all involved diagrams:
    QPointF smallestPoint;
    QPointF largestPoint;
    foreach ( AbstractDiagram* diagram, diagrams() )
        {
            QPair<QPointF, QPointF> dataBoundariesPair = diagram->dataBoundaries();
            if ( dataBoundariesPair.first.x() < smallestPoint.x() )
              smallestPoint.setX( dataBoundariesPair.first.x() );
            if ( dataBoundariesPair.first.y() < smallestPoint.y() )
              smallestPoint.setY( dataBoundariesPair.first.y() );
            if ( dataBoundariesPair.second.x() > largestPoint.x() )
              largestPoint.setX( dataBoundariesPair.second.x() );
            if ( dataBoundariesPair.second.y() > largestPoint.y() )
              largestPoint.setY( dataBoundariesPair.second.y() );
        }
    QRectF dataBoundingRect;
    dataBoundingRect.setBottomLeft ( smallestPoint );
    dataBoundingRect.setTopRight ( largestPoint );

    // calculate the remaining rectangle, and use it as the diagram area:
    d->diagramArea = d->drawingArea;
    d->diagramArea.setTopLeft ( QPointF ( d->drawingArea.left(), d->drawingArea.top() ) );
    d->diagramArea.setBottomRight ( QPointF ( d->drawingArea.right(), d->drawingArea.bottom() ) );

    // determine coordinate transformation:
    QPointF diagramTopLeft = dataBoundingRect.topLeft();
    QPointF diagramBottomRight = dataBoundingRect.bottomRight();
    double diagramWidth = dataBoundingRect.width();
    double diagramHeight = dataBoundingRect.height();
    double planeWidth = d->diagramArea.width();
    double planeHeight = d->diagramArea.height();
    double scaleX;
    double scaleY;

    double diagramXUnitInCoordinatePlane = planeWidth / diagramWidth;
    double diagramYUnitInCoordinatePlane = planeHeight / diagramHeight;
    // calculate diagram origin in plane coordinates:
    QPointF coordinateOrigin = QPointF (
        diagramTopLeft.x() * -diagramXUnitInCoordinatePlane,
        diagramTopLeft.y() * -diagramYUnitInCoordinatePlane );
    coordinateOrigin += d->diagramArea.topLeft();
    // calculate isometric scaling factor to maxscale the diagram into
    // the coordinate system:
    if ( d->isometricScaling )
    {
        double scale = qMin ( qAbs ( diagramXUnitInCoordinatePlane ),
                              qAbs ( diagramYUnitInCoordinatePlane ) );

        scaleX = qAbs( scale / diagramXUnitInCoordinatePlane );
        scaleY = qAbs( scale / diagramYUnitInCoordinatePlane );
    } else {
        scaleX = 1.0;
        scaleY = 1.0;
    }
    d->coordinateTransformation.diagramRect = dataBoundingRect;
    d->coordinateTransformation.originTranslation = coordinateOrigin;
    d->coordinateTransformation.unitVectorX = diagramXUnitInCoordinatePlane;
    d->coordinateTransformation.unitVectorY = diagramYUnitInCoordinatePlane;
    d->coordinateTransformation.isoScaleX = scaleX;
    d->coordinateTransformation.isoScaleY = scaleY;
    //      adapt diagram area to effect of isometric scaling:
    d->diagramArea.setTopLeft( translate ( dataBoundingRect.topLeft() ) );
    d->diagramArea.setBottomRight ( translate ( dataBoundingRect.bottomRight() ) );
}

const QPointF CartesianCoordinatePlane::translate( const QPointF& diagramPoint ) const
{
    // i had to disable the assertion, because it fails for diagramPoint's
    // directly on the edge of the diagramRect. i guess it can be replaced by
    // manually comparing he coordinates, but be aware of the fact that the
    // user coordinate system may be directed in any way the user choses!
//     Q_ASSERT_X ( d->coordinateTransformation.diagramRect.contains ( diagramPoint ),
//                  "CartesianCoordinatePlane::translate", "Diagram point has to be inside "
//                  "user-supplied diagram rectangle." );
    return  d->coordinateTransformation.translate ( diagramPoint );
}

void CartesianCoordinatePlane::setIsometricScaling ( bool onOff )
{
    if ( d->isometricScaling != onOff )
    {
        d->isometricScaling = onOff;
        layoutDiagrams();
    }
}

bool CartesianCoordinatePlane::doesIsometricScaling ()
{
    return d->isometricScaling;
}

void CartesianCoordinatePlane::setZoomFactorX( double factor )
{
    d->coordinateTransformation.zoom.xFactor = factor;
}

void CartesianCoordinatePlane::setZoomFactorY( double factor )
{
    d->coordinateTransformation.zoom.yFactor = factor;
}

void CartesianCoordinatePlane::setZoomCenter( QPointF center )
{
    d->coordinateTransformation.zoom.xCenter = center.x();
    d->coordinateTransformation.zoom.yCenter = center.y();
}

QPointF CartesianCoordinatePlane::zoomCenter() const
{
    return QPointF( d->coordinateTransformation.zoom.xCenter,
                    d->coordinateTransformation.zoom.yCenter );
}

double CartesianCoordinatePlane::zoomFactorX() const
{
    return d->coordinateTransformation.zoom.xFactor;
}

double CartesianCoordinatePlane::zoomFactorY() const
{
    return d->coordinateTransformation.zoom.yFactor;
}
