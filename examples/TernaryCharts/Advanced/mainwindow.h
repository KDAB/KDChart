#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

#include "ui_mainwindow.h"

namespace KDChart {
    class Chart;
    class TernaryDiagram;
    class TernaryCoordinatePlane;
}

class MainWindow : public QDialog,
                   private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );

private:
    KDChart::Chart* m_chart;
    KDChart::TernaryDiagram* m_diagram;
    KDChart::TernaryCoordinatePlane* m_ternaryPlane;
};

#endif
