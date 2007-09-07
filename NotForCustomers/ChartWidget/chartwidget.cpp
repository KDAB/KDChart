#include "chartwidget.h"
#include <KDChartWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidgetItem>

#include <KDChartChart>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartLineDiagram>
#include <KDChartLineAttributes>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartThreeDLineAttributes>
#include <KDChartMarkerAttributes>
#include <KDChartLegend>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

ChartWidget::ChartWidget(QWidget *parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);

    connect(m_ui.tableWidget, SIGNAL (cellChanged(int, int)),
            this, SLOT (onCellChanged(int, int)));

    m_ui.tableWidget->setColumnCount(2);
    m_ui.tableWidget->setRowCount(11);
    QStringList lstHeaders;
    lstHeaders.append( tr("Y line 1") );
    lstHeaders.append( tr("Y line 2") );
    m_ui.tableWidget->setHorizontalHeaderLabels( lstHeaders );
    QStringList lstHeaders2;
    lstHeaders2.append( tr("X=0") );
    lstHeaders2.append( tr("X=1") );
    lstHeaders2.append( tr("X=2") );
    lstHeaders2.append( tr("X=3") );
    lstHeaders2.append( tr("X=4") );
    lstHeaders2.append( tr("X=5") );
    lstHeaders2.append( tr("X=6") );
    lstHeaders2.append( tr("X=7") );
    lstHeaders2.append( tr("X=8") );
    lstHeaders2.append( tr("X=9") );
    lstHeaders2.append( tr("X=10") );
    m_ui.tableWidget->setVerticalHeaderLabels( lstHeaders2 );

    QHBoxLayout* chartLayout = new QHBoxLayout( m_ui.frame );
    widget = new Widget( m_ui.frame );

    KDChart::LineDiagram* lnDiagram = dynamic_cast<KDChart::LineDiagram*>(widget->diagram());
    xAxis = new CartesianAxis( lnDiagram );
    yAxis = new CartesianAxis (lnDiagram );
    xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
    yAxis->setPosition ( KDChart::CartesianAxis::Left );
    lnDiagram->addAxis( xAxis );
    lnDiagram->addAxis( yAxis );
    //Pending Michel: test this example without calling
    //replaceDiagram() but layoutDiagram()
    //widget->coordinatePlane()->layoutDiagrams();
    widget->coordinatePlane()->replaceDiagram( lnDiagram );
    chartLayout->addWidget( widget );

}

ChartWidget::~ChartWidget()
{

}


void ChartWidget::onCellChanged(int row, int column)
{
    QString text;
    QVector<double> vec;
    for (int i = 0; i < m_ui.tableWidget->rowCount(); i++)
    {
        QTableWidgetItem* pTableItem = m_ui.tableWidget->item( i, column);
        if ( pTableItem == NULL) text = "0";
        else text = pTableItem->text().trimmed();
        vec.append(text.toDouble());
    }

// Note: there seems to be a bug here as only the first two X Labels
// are changed - we will investigate this issue and fix it for the beta3
// version

     if ( column == 0 || column == 1 )
    {
        widget->setDataset( column, vec, QString("Line%1").arg( column + 1 ) );
        //refresh to display the axis
        widget->update();
        return;
    }

}

