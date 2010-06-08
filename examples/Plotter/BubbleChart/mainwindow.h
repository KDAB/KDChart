#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class QStandardItemModel;

namespace KDChart {
    class Chart;
    class Plotter;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private slots:
    void initializeDataModel();
    void setMarkerAttributes();

private:
    QStandardItemModel* m_model;
    KDChart::Chart* m_chart;
    KDChart::Plotter* m_plotter;
};


#endif /* MAINWINDOW_H */

