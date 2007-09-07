#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtGui/QMainWindow>
#include "ui_chartwidget.h"
#include "TableModel.h"


namespace KDChart{
    class Widget;
    class Chart;
    class DatasetProxyModel;
    class LineDiagram;
    class LineAttributes;
    class CartesianAxis;
    class Legend;
    class ChartParams;
}

class ChartWidget : public QMainWindow
{
    Q_OBJECT

public:
    ChartWidget(QWidget *parent = 0);
    ~ChartWidget();

    KDChart::Widget* widget;

protected slots:

    virtual void onCellChanged(int row, int column);



private:
    Ui::ChartWidgetClass m_ui;

    //KDChart::Chart* m_chart;
    TableModel m_model;
    //KDChart::LineDiagram* lnDiagram;
    KDChart::Legend* m_legend;
    KDChart::CartesianAxis* xAxis;
    KDChart::CartesianAxis* yAxis;

};

#endif // CHARTWIDGET_H
