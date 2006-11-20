#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QStandardItemModel>

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
    void setupModel();

    KDChart::Chart* m_chart;
    KDChart::TernaryDiagram* m_diagram;
    KDChart::TernaryCoordinatePlane* m_ternaryPlane;

    QStandardItemModel m_model;
};

#endif
