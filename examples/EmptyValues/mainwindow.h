#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <TableModel.h>

namespace KDChart {
    class Chart;
    class BarDiagram;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );
    virtual ~MainWindow() { }

private:
    KDChart::Chart* m_chart;
    KDChart::BarDiagram* m_bars;
    TableModel m_model;
};


#endif /* MAINWINDOW_H */

