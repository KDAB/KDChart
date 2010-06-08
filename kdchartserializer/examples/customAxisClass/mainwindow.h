#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"


class QStandardItemModel;

namespace KDChart {
    class Chart;
    class LineDiagram;
}


class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );
    virtual ~MainWindow();

private slots:

    void initializeDataModel();

    void on_pushButtonSave_clicked();
    void saveAs();
    void on_pushButtonClear_clicked();
    void removeTheChart();
    void on_pushButtonLoad_clicked();
    void load();

private:
    QHBoxLayout* m_chartLayout;

    KDChart::Chart* m_chart;

    QStandardItemModel* m_model;
};


#endif /* MAINWINDOW_H */

