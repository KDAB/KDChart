#include <QFont>
#include <QList>
#include <QtDebug>
#include <QPainter>

#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartCartesianCoordinatePlane.h"
#include "CartesianCoordinateTransformation.h"

#include "KDChartPaintContext.h"

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
        painter.save();
        painter.setRenderHint ( QPainter::TextAntialiasing );
        painter.setPen ( "gold" );
        painter.setBrush (  Qt::NoBrush );
        painter.drawRect ( 0, 0, width() - 1,  height() - 1);
        QFont font = painter.font();
        font.setPointSize ( 7 );
        painter.setFont( font );
        painter.setPen ( Qt::lightGray );
        painter.drawText ( 0, 0, width() - 3 , height() - 3, Qt::AlignRight | Qt::AlignTop, text );
        painter.restore();
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
        paintRulers ( &ctx );

        // paint the axes:
        foreach ( AbstractDiagram* abstractDiagram, diagrams() )
            {
                AbstractCartesianDiagram* diagram = dynamic_cast<AbstractCartesianDiagram*> ( abstractDiagram );
                Q_ASSERT ( diagram );
                if ( diagram )
                    foreach ( CartesianAxis* axis, diagram->axes() )
                    {
                        axis->paint ( &ctx );
                    }
            }

        // paint the diagrams:
        for ( int i = 0; i < diags.size(); i++ )
        {
            painter.save();
            diags[i]->paint ( &ctx );
            painter.restore();
        }
    }
}

void CartesianCoordinatePlane::paintRulers( PaintContext* ctx )
{
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

    ctx->painter()->setPen ( QColor ( Qt::lightGray ) );
    ctx->painter()->drawLines ( rulers );
    ctx->painter()->setPen ( QColor ( Qt::blue ) );
    ctx->painter()->drawLines ( axes );

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

    // lay out the diagrams and the axes:
    //
    // query and lay out the axes (make sure to keep this list stable, since
    // it already holds the order in which the axes are supposed to be painted):
    CartesianAxisList axes;
    foreach ( AbstractDiagram* abstractDiagram, diagrams() )
        {
            AbstractCartesianDiagram* diagram =
                dynamic_cast<AbstractCartesianDiagram*> ( abstractDiagram );
            Q_ASSERT ( diagram );
            if ( diagram )
            {
                axes << diagram->axes();
            }
        }

    double bottomAxisHeight = 0.0;
    double leftAxisWidth = 0.0;
    double rightAxisWidth = 0.0;
    double topAxisHeight = 0.0;
    foreach ( CartesianAxis* axis, axes )
        {
            switch ( axis->position() )
            {
            case CartesianAxis::Top:
                topAxisHeight += axis->sizeHint().height();
                break;
            case CartesianAxis::Bottom:
                bottomAxisHeight += axis->sizeHint().height();
                break;
            case CartesianAxis::Left:
                leftAxisWidth += axis->sizeHint().width();
                break;
            case CartesianAxis::Right:
                rightAxisWidth += axis->sizeHint().width();
                break;
            default:
                Q_ASSERT_X( false, "CartesianCoordinatePlane::paintEvent",
                            "unknown axis position" );
                break;
            };
        }

    // calculate the remaining rectangle, and use it as the diagram area:
    d->diagramArea = d->drawingArea;
    d->diagramArea.setTopLeft ( QPointF ( d->drawingArea.left() + leftAxisWidth ,
                                          d->drawingArea.top() + topAxisHeight ) );
    d->diagramArea.setBottomRight ( QPointF ( d->drawingArea.right() - rightAxisWidth,
                                              d->drawingArea.bottom() - bottomAxisHeight ) );

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

    // calculate the rectangles, lay out the axes and the grid area, and draw
    // the axes:
    QRectF leftAxisRect;
    leftAxisRect.setTopLeft ( QPointF( d->diagramArea.x() - leftAxisWidth,
                                       d->diagramArea.y() ) );
    leftAxisRect.setWidth ( leftAxisWidth );
    leftAxisRect.setHeight ( d->diagramArea.height() );
    QRectF rightAxisRect;
    rightAxisRect.setTopLeft( QPointF ( d->diagramArea.right(),d->diagramArea.top() ) );
    rightAxisRect.setWidth ( rightAxisWidth );
    rightAxisRect.setHeight( d->diagramArea.height() );
    QRectF topAxisRect;
    topAxisRect.setTopLeft ( QPointF( d->diagramArea.left(), d->diagramArea.top() - topAxisHeight ) );
    topAxisRect.setWidth( d->diagramArea.width() );
    topAxisRect.setHeight( topAxisHeight );
    QRectF bottomAxisRect;
    bottomAxisRect.setTopLeft ( QPointF( d->diagramArea.left(), d->diagramArea.bottom() ) );
    bottomAxisRect.setHeight( bottomAxisHeight );
    bottomAxisRect.setWidth( d->diagramArea.width() );

    // FIXME: calculate every axes rectangle (try with more than one)
    QPointF leftTopLeft ( leftAxisRect.topRight() ),
        leftBottomRight ( leftAxisRect.bottomRight() ),
        topTopLeft ( topAxisRect.bottomLeft() ),
        topBottomRight ( topAxisRect.bottomRight() ),
        rightTopLeft ( rightAxisRect.topLeft() ),
        rightBottomRight ( rightAxisRect.bottomLeft() ),
        bottomTopLeft ( bottomAxisRect.topLeft() ),
        bottomBottomRight ( bottomAxisRect.topRight() );

    foreach ( CartesianAxis* axis, axes )
        {
            QSizeF sizehint = axis->sizeHint();
            QRectF rect;
            switch ( axis->position() ) {
            case CartesianAxis::Left:
                leftTopLeft.setX ( leftTopLeft.x() - sizehint.width() );
                rect.setTopLeft ( leftTopLeft );
                rect.setBottomRight( leftBottomRight );
                axis->setGeometry ( rect );
                leftBottomRight.setX ( leftBottomRight.x() - sizehint.width() );
                break;
            case CartesianAxis::Bottom:
                bottomBottomRight.setY( bottomBottomRight.y() + sizehint.height() );
                rect.setTopLeft ( bottomTopLeft );
                rect.setBottomRight ( bottomBottomRight );
                axis->setGeometry ( rect );
                bottomTopLeft.setY ( bottomTopLeft.y() + sizehint.height() );
                break;
            case CartesianAxis::Top:
                topTopLeft.setY( topTopLeft.y() - sizehint.height() );
                rect.setTopLeft ( topTopLeft );
                rect.setBottomRight ( topBottomRight );
                axis->setGeometry ( rect );
                topBottomRight.setY ( topBottomRight.y() - sizehint.height() );
                break;
            case CartesianAxis::Right:
                rightBottomRight.setX( rightBottomRight.x() + sizehint.width() );
                rect.setTopLeft ( rightTopLeft );
                rect.setBottomRight ( rightBottomRight );
                axis->setGeometry ( rect );
                rightTopLeft.setX ( rightTopLeft.x() + sizehint.width() );
                break;
            }
        }

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
