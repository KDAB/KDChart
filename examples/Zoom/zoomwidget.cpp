#include "zoomwidget.h"
#include <KDChartAbstractCoordinatePlane>
#include <QWheelEvent>

ZoomWidget::ZoomWidget( QWidget* parent ) :
    KDChart::Widget( parent )
{

}

void ZoomWidget::wheelEvent( QWheelEvent* e )
{
    coordinatePlane()->setZoomFactorX( coordinatePlane()->zoomFactorX()+e->delta() );
    coordinatePlane()->setZoomFactorY( coordinatePlane()->zoomFactorX()+e->delta() );
    coordinatePlane()->setZoomCenter( e->pos() );
}
