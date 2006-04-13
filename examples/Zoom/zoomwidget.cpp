#include "zoomwidget.h"
#include <KDChartAbstractCoordinatePlane>
#include <QWheelEvent>
#include <QDebug>

ZoomWidget::ZoomWidget( QWidget* parent ) :
    KDChart::Widget( parent )
{
    setFocusPolicy( Qt::WheelFocus );
}

void ZoomWidget::wheelEvent( QWheelEvent* e )
{
    qreal delta = static_cast<qreal>( e->delta() ) / 120.0 / 10.0;
    coordinatePlane()->setZoomFactorX( coordinatePlane()->zoomFactorX() + delta );
    coordinatePlane()->setZoomFactorY( coordinatePlane()->zoomFactorY() + delta );
    coordinatePlane()->setZoomCenter( e->pos() );
    update();
}

void ZoomWidget::keyPressEvent( QKeyEvent* e )
{
    int dZoom = 0;
    qreal dX = 0;
    qreal dY = 0;
    switch ( e->key() ){
        case Qt::Key_PageDown:
            dZoom = 1;
            break;
        case Qt::Key_Down:
            dY = 0.1;
            break;
        case Qt::Key_Right:
            dX = 0.1;
            break;
        case Qt::Key_PageUp:
            dZoom = -1;
            break;
        case Qt::Key_Up:
            dY = -0.1;
            break;
        case Qt::Key_Left:
            dX = -0.1;
            break;
        default:
            e->ignore();
    }
    if( dZoom || dX || dY ){
        const double factor = qMax( 1.0, coordinatePlane()->zoomFactorX() + dZoom );
        const qreal x = coordinatePlane()->zoomCenter().x() + dX;
        const qreal y = coordinatePlane()->zoomCenter().y() + dY;
qDebug("new zoom factor: %f   new center: (%f, %f)", factor, x, y);
        coordinatePlane()->setZoomFactorX( factor );
        coordinatePlane()->setZoomFactorY( factor );
        coordinatePlane()->setZoomCenter( QPointF(x,y) );
        update();
    }
}
