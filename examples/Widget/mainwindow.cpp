#include "mainwindow.h"

#include <KDChartWidget.h>
#include <QDebug>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent )
 : QWidget( parent ), datasetCount( 3 )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    widget = new Widget( chartFrame );
    chartLayout->addWidget( widget );

    connect( typeSelector, SIGNAL( activated( int )), SLOT( changeType() ));

    connect( btnAddDataset, SIGNAL( clicked()), SLOT( addDataset() ));
    
/*    connect( leadingSelector, SIGNAL( valueChanged( int )),
		    this, SLOT( changeLeading( int )));*/
}

void MainWindow::changeType()
{
    QString text = typeSelector->currentText();
    if ( text == "Widget::Bar" )
        widget->setType( Widget::Bar );
    else if ( text == "Widget::Line" )
        widget->setType( Widget::Line );
    else if ( text == "Widget::Pie" )
        widget->setType( Widget::Pie );
    else if ( text == "Widget::Polar" )
        widget->setType( Widget::Polar );
    else if ( text == "Widget::Ring" )
        widget->setType( Widget::Ring );
    else
        widget->setType( Widget::NoType );
}

void MainWindow::changeLeading( int leading )
{
    widget->setGlobalLeading( leading, leading, leading, leading );
}

void MainWindow::addDataset()
{
    QStringList parts = lineAddDataset->text().split(";");
    bool ok;
    QVector< double > vec;
    foreach (QString str, parts ) {
        vec.append( str.toDouble(&ok) );
    }
    widget->setDataset( datasetCount++, vec );
}
