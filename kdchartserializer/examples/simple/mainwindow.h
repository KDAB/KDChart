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

private slots:

    void initializeDataModel();

    void on_lineTypeCB_currentIndexChanged( const QString & text );
    void on_paintValuesCB_toggled( bool checked );
    void on_threeDModeCB_toggled( bool checked );
    void on_depthSB_valueChanged( int i );
    void on_highlightAreaCB_toggled( bool checked );
    void on_highlightAreaSB_valueChanged( int i );
    void setHighlightArea( int row, int column, int opacity, bool checked, bool doUpdate );

    void on_pushButtonLoad_clicked();
    void load();
    void on_pushButtonSave_clicked();
    void saveAs();

private:
    QHBoxLayout* m_chartLayout;

    KDChart::Chart* m_chart;
    KDChart::LineDiagram* m_lines;

    QStandardItemModel* m_model;
    int m_curRow;
    int m_curColumn;
    int m_curOpacity;
};


#endif /* MAINWINDOW_H */

