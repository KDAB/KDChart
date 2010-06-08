#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class QAbstractItemModel;

namespace KDChart {
    class Chart;
    class LineDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private slots:
    void on_showDataset1CB_toggled( bool checked );
    void on_showDataset2CB_toggled( bool checked );
    void on_showDataset3CB_toggled( bool checked );
    void setHidden( int dataset, bool hidden );
    void openFile(const QString &path);

private:
    KDChart::Chart             * m_chart;
    KDChart::LineDiagram       * m_lines;
    QAbstractItemModel         * m_model;
};


#endif /* MAINWINDOW_H */

