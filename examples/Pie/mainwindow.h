#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

class QTimer;
namespace KDChart {
    class Chart;
    class DatasetProxyModel;
    class PieDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private slots:
    // start position
    void on_startPositionSB_valueChanged( double pos );
    void on_startPositionSL_valueChanged( int pos );

    // explode
    void on_explodeGB_toggled( bool toggle );
    void on_explodeSubmitPB_clicked();
    void on_animateExplosionCB_toggled( bool toggle );

    // animation
    void slotNextFrame();

private:
    KDChart::Chart* m_chart;
    TableModel m_model;
    KDChart::DatasetProxyModel* m_datasetProxy;
    KDChart::PieDiagram* m_pie;
    QTimer* m_timer;

    int m_currentFactor;
    int m_currentDirection;
    int m_currentSlice;
};


#endif /* MAINWINDOW_H */

