#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <KDChartWidget>

class ZoomWidget : public KDChart::Widget
{
    Q_OBJECT

public:
    explicit ZoomWidget( QWidget* parent );

protected:
    void wheelEvent( QWheelEvent* e );
};


#endif /* ZOOMWIDGET_H */

