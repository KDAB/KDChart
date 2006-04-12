#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QStandardItemModel>

namespace KDChart {
    class Chart;
    class DatasetProxyModel;
    class LineDiagram;
    class BarDiagram;
    class LineAttributes;
    class CartesianAxis;
    class CartesianCoordinatePlane;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private:
    KDChart::Chart* m_chart;
    QStandardItemModel m_model;
    QStandardItemModel m_model2;
    KDChart::DatasetProxyModel* m_datasetProxy;
    KDChart::LineDiagram* m_lines;
    KDChart::BarDiagram* m_bars;
    KDChart::CartesianCoordinatePlane* plane;
    // mutable KDChart::CartesianAxis xAxis;
    //mutable KDChart::CartesianAxis yAxis;
    
};


#endif /* MAINWINDOW_H */

