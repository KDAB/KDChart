#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
    class Chart;
    class DatasetProxyModel;
    class LineDiagram;
    class LineAttributes;
    class CartesianAxis;
    class CartesianCoordinatePlane;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private slots:

    void on_displayGrid1CB_toggled( bool checked );
    void on_displayGrid2CB_toggled( bool checked );

private:
    void init();
    KDChart::Chart* m_chart;
    TableModel m_model;
    TableModel m_model2;
    KDChart::DatasetProxyModel* m_datasetProxy;
    KDChart::LineDiagram* m_lines;
    KDChart::LineDiagram* m_lines2;
    KDChart::CartesianCoordinatePlane* plane2;


};


#endif /* MAINWINDOW_H */

