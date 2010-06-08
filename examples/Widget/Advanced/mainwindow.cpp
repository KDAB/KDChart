#include "mainwindow.h"

#include <KDChartWidget>
#include <KDChartAbstractDiagram>
#include <QDebug>
#include <QMessageBox>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent )
 : QWidget( parent ), datasetCount( 3 )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    widget = new Widget( chartFrame );
    chartLayout->addWidget( widget );

    typeSelector->setCurrentIndex(1); // we start by LineDiagram

    connect( typeSelector, SIGNAL( activated( int )), SLOT( changeType() ));

    connect( btnAddDataset, SIGNAL( clicked()), SLOT( addDataset() ));

    connect( leadingSelector, SIGNAL( valueChanged( int )),
		    this, SLOT( changeLeading( int )));
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
    else
        widget->setType( Widget::NoType );
}

void MainWindow::changeLeading( int leading )
{
    widget->setGlobalLeading( leading, leading, leading, leading );
}

void MainWindow::addDataset()
{
    QStringList parts = lineAddDataset->text().split(';');
    bool ok;
    QVector< double > vec;
    foreach ( const QString &str, parts ) {
        const double val = str.toDouble(&ok);
        if(ok)
            vec.append( val );
    }
    const int rows = widget->diagram()->model()->rowCount();
    if( vec.count() != rows )
        QMessageBox::warning( this, "Wrong number of values entered!",
                              QString("You have entered %1 values,<br>but the data model needs %2 ones.<br><br>Note: Use <b>;</b> to separate the values!")
                                      .arg( vec.count() )
                                      .arg( rows ) );
    else
        widget->setDataset( datasetCount++, vec, "user data" );
}
