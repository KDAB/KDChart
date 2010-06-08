#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QStandardItemModel>

namespace KDChart {
    class Chart;
    class DatasetProxyModel;
    class PolarDiagram;
    class PolarCoordinatePlane;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );

private slots:
    // start position of circular grid
    void on_startPositionSB_valueChanged( double pos );
    void on_startPositionSL_valueChanged( int pos );

    // show grid
    void on_circularGridCB_toggled( bool toggle );
    void on_sagittalGridCB_toggled( bool toggle );

    // show axes
    void on_circularAxisCB_toggled( bool toggle );
    void on_sagittalAxisCB_toggled( bool toggle );

private:
    void initKDChartClasses();
    void wireUpKDChartClasses();
    void setItemModelData();

    // For a change we do not use our special TableModel here,
    // but we just use the standard model that comes with Qt.
    QStandardItemModel m_model;

    KDChart::Chart* m_chart;
    KDChart::PolarDiagram* m_diagram;
    KDChart::PolarCoordinatePlane* m_polarPlane;

    int m_currentFactor;
    int m_currentDirection;
    int m_currentSlice;
};


#endif /* MAINWINDOW_H */
