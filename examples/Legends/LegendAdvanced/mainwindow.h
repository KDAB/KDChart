#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMap>

#include "ui_mainwindow.h"
#include "derivedaddlegenddialog.h"
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

private slots:
    void on_addLegendPB_clicked();
    void on_editLegendPB_clicked();
    void on_removeLegendPB_clicked();
    void on_legendsTV_itemSelectionChanged();

private:
    void initAddLegendDialog( DerivedAddLegendDialog& conf,
                              Qt::Alignment alignment ) const;

    KDChart::Chart* m_chart;
    TableModel m_model;
    KDChart::LineDiagram* m_lines;
    QMap<Qt::Alignment, QString> alignmentMap;
};


#endif /* MAINWINDOW_H */

