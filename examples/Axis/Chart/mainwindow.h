#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
    class Chart;
    class BarDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );


private:
    KDChart::Chart* m_chart;
    TableModel m_model;   
    KDChart::BarDiagram* m_lines;
};


#endif /* MAINWINDOW_H */

