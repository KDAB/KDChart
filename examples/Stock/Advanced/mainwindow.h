#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>
#include <KDChartChart>
#include <KDChartStockDiagram>
#include <KDChartThreeDBarAttributes>

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = 0 );

private slots:
    void on_threeDProperties_toggled( bool checked );
    void on_perspectiveDepth_valueChanged( int value );
    void on_perspectiveAngle_valueChanged( int value );
    void on_useShadowColors_toggled( bool checked );
    void on_stockTypeCB_currentIndexChanged( const QString &text );

private:
    void initValues();

    KDChart::Chart *m_chart;
    TableModel m_HLCModel;
    TableModel m_OHLCModel;

    KDChart::StockDiagram m_diagram;
    KDChart::ThreeDBarAttributes m_threeDBarAttributes;
};

#endif // MAINWINDOW_H
