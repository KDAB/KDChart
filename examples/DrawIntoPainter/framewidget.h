#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>

namespace KDChart {
    class Chart;
}


class FrameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameWidget( QWidget * parent = 0, Qt::WFlags f = 0 );

    virtual void paintEvent( QPaintEvent* );

    void setChart( KDChart::Chart* chart ){
        mChart = chart;
    }

private:
    KDChart::Chart* mChart;
};


#endif /* FRAMEWIDGET_H */

