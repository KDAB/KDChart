#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

namespace KDChart{
    class Widget;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = NULL );

    KDChart::Widget* widget;

public slots:
    void changeType();
    void changeLeading( int leading );
    void addDataset();

private:
    int datasetCount;
};

#endif
