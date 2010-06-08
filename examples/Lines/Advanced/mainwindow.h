#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
    class Chart;
    class LineDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private:
    bool eventFilter(QObject* target, QEvent* event);

private slots:

    void on_lineTypeCB_currentIndexChanged( const QString & text );
    void on_paintValuesCB_toggled( bool checked );
    void on_centerDataPointsCB_toggled( bool checked );
    void on_threeDModeCB_toggled( bool checked );
    void on_depthSB_valueChanged( int i );
    void on_animateAreasCB_toggled( bool checked );
    void on_highlightAreaCB_toggled( bool checked );
    void on_highlightAreaSB_valueChanged( int i );
    void setHighlightArea( int row, int column, int opacity,
                           bool checked, bool doUpdate );
    void on_trackAreasCB_toggled( bool checked );
    void on_trackAreasSB_valueChanged( int i );
    void setTrackedArea( int column, bool checked, bool doUpdate );
    void slot_timerFired();
    void on_reverseHorizontalCB_toggled( bool checked );
    void on_reverseVerticalCB_toggled( bool checked );

private:
    KDChart::Chart* m_chart;
    KDChart::LineDiagram* m_lines;
    TableModel m_model;
    int m_curRow;
    int m_curColumn;
    int m_curOpacity;
};


#endif /* MAINWINDOW_H */

