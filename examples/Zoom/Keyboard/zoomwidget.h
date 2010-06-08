#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <KDChartWidget>

class ZoomWidget : public KDChart::Widget
{
    Q_OBJECT

public:
    explicit ZoomWidget( QWidget* parent );

protected:
    QPointF findNewZoomCenter( const QPoint & pos );
    void mousePressEvent( QMouseEvent * e );
    void wheelEvent( QWheelEvent* e );
    void keyPressEvent( QKeyEvent* e );
};


#endif /* ZOOMWIDGET_H */

