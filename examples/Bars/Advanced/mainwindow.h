#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
    class Chart;
    class BarDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private slots:

    void on_barTypeCB_currentIndexChanged( const QString & text );
    void on_barOrientationCB_currentIndexChanged( const QString & text );
    void on_paintValuesCB_toggled( bool checked );
    void on_paintThreeDBarsCB_toggled( bool checked );
    void on_markColumnCB_toggled( bool checked );
    void on_markColumnSB_valueChanged( int i );
    void on_threeDDepthCB_toggled( bool checked );
    void on_depthSB_valueChanged( int i );
    void on_widthCB_toggled( bool checked );
    void on_widthSB_valueChanged( int i );
    void on_fixPlaneSizeCB_toggled( bool checked );

private:
    KDChart::Chart* m_chart;
    KDChart::BarDiagram* m_bars;
    TableModel m_model;
};


#endif /* MAINWINDOW_H */

