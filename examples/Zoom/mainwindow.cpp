#include "mainwindow.h"
#include "zoomwidget.h"
#include <KDChartWidget>
#include <QDebug>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent )
 : QWidget( parent )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    widget = new ZoomWidget( chartFrame );
    chartLayout->addWidget( widget );
}

