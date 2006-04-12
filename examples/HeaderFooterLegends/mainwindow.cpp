#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartHeaderFooter>
#include <KDChartLegend>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartDatasetProxyModel>
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include "ui_addheaderdialog.h"
#include "ui_addlegenddialog.h"

class HeaderItem : public QTreeWidgetItem
{
public:
    HeaderItem( KDChart::HeaderFooter* header, QTreeWidget* parent ) :
        QTreeWidgetItem( parent ), m_header( header ) {}

    KDChart::HeaderFooter* header() const { return m_header; }

private:
    KDChart::HeaderFooter* m_header;
};

class LegendItem : public QTreeWidgetItem
{
public:
    LegendItem( KDChart::Legend* legend, QTreeWidget* parent ) :
        QTreeWidgetItem( parent ), m_legend( legend ) {}

    KDChart::Legend* legend() const { return m_legend; }

private:
    KDChart::Legend* m_legend;
};


MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new KDChart::Chart();
    chartLayout->addWidget( m_chart );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_lines = new KDChart::LineDiagram();
    m_lines->setModel( &m_model );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );

    // Add at least one legend for starters
    KDChart::Legend* legend = new KDChart::Legend( m_lines, m_chart );
    legend->setPosition( KDChart::Legend::North );
    legend->setShowLines( false );
    legend->setTitleText( tr( "Legend" ) );
    legend->setOrientation( Qt::Vertical );
    m_chart->addLegend( legend );
    legend->show();

    LegendItem* newItem = new LegendItem( legend, legendsTV );
    newItem->setText( 0, tr( "Top" ) );
    newItem->setText( 1, tr( "no" ) );
    newItem->setText( 2, tr( "Legend" ) );
    newItem->setText( 3, tr( "Vertical" ) );
    m_chart->update();
}

void MainWindow::on_addHeaderPB_clicked()
{
    QDialog dlg;
    Ui::AddHeaderDialog conf;
    conf.setupUi( &dlg );
    conf.textED->setFocus();
    if( dlg.exec() ) {
        KDChart::HeaderFooter* headerFooter = new KDChart::HeaderFooter( m_chart );
        m_chart->addHeaderFooter( headerFooter );
        headerFooter->setText( conf.textED->text() );
        KDChart::TextAttributes attrs;
        attrs.setColor( Qt::red );
        headerFooter->setTextAttributes( attrs );
        headerFooter->setPosition( (KDChart::HeaderFooter::HeaderFooterPosition)conf.positionCO->currentIndex() );
        headerFooter->show();
        HeaderItem* newItem = new HeaderItem( headerFooter, headersTV );
        newItem->setText( 0, conf.textED->text() );
        newItem->setText( 1, conf.positionCO->currentText() );
        m_chart->update();
    }
}


void MainWindow::on_editHeaderPB_clicked()
{
    HeaderItem* item = static_cast<HeaderItem*>( headersTV->selectedItems().first() );
    KDChart::HeaderFooter* headerFooter = item->header();
    QDialog dlg;
    Ui::AddHeaderDialog conf;
    conf.setupUi( &dlg );
    conf.textED->setText( headerFooter->text() );
    conf.textED->setFocus();
    conf.positionCO->setCurrentIndex( headerFooter->position() );
    if( dlg.exec() ) {
        headerFooter->setText( conf.textED->text() );
        headerFooter->setPosition( (KDChart::HeaderFooter::HeaderFooterPosition)conf.positionCO->currentIndex() );
        item->setText( 0, conf.textED->text() );
        item->setText( 1, conf.positionCO->currentText() );
        m_chart->update();
    }
}




void MainWindow::on_removeHeaderPB_clicked()
{
    QList<QTreeWidgetItem*> items = headersTV->selectedItems();
    for( QList<QTreeWidgetItem*>::const_iterator it = items.begin();
         it != items.end(); ++it ) {
        delete static_cast<HeaderItem*>( (*it) )->header();
        delete (*it);
    }
    m_chart->update();
}


void MainWindow::on_headersTV_itemSelectionChanged()
{
    removeHeaderPB->setEnabled( headersTV->selectedItems().count() > 0 );
    removeHeaderPB->setEnabled( headersTV->selectedItems().count() == 1 );
}


void MainWindow::on_addLegendPB_clicked()
{
    QDialog dlg;
    Ui::AddLegendDialog conf;
    conf.setupUi( &dlg );
    conf.titleTextED->setFocus();
    if( dlg.exec() ) {
        KDChart::Legend* legend = new KDChart::Legend( m_lines, m_chart );
        m_chart->addLegend( legend );
        legend->setPosition( (KDChart::Legend::LegendPosition)conf.positionCO->currentIndex() );
        legend->setShowLines( conf.showLinesCB->isChecked() );
        legend->setTitleText( conf.titleTextED->text() );
        legend->setOrientation( ( conf.orientationCO->currentIndex() == 0 ) ? Qt::Vertical : Qt::Horizontal );
        legend->show();
        LegendItem* newItem = new LegendItem( legend, legendsTV );
        newItem->setText( 0, conf.positionCO->currentText() );
        newItem->setText( 1, conf.showLinesCB->isChecked() ? "yes" : "no" );
        newItem->setText( 2, conf.titleTextED->text() );
        newItem->setText( 3, conf.orientationCO->currentText() );
        m_chart->update();
    }
}


void MainWindow::on_editLegendPB_clicked()
{
    LegendItem* item = static_cast<LegendItem*>( legendsTV->selectedItems().first() );
    KDChart::Legend* legend = item->legend();
    QDialog dlg;
    Ui::AddLegendDialog conf;
    conf.setupUi( &dlg );
    conf.titleTextED->setFocus();
    conf.positionCO->setCurrentIndex( legend->position() );
    conf.showLinesCB->setChecked( legend->showLines() );
    conf.titleTextED->setText( legend->titleText() );
    if( dlg.exec() ) {
        legend->setPosition( (KDChart::Legend::LegendPosition)conf.positionCO->currentIndex() );
        legend->setShowLines( conf.showLinesCB->isChecked() );
        legend->setTitleText( conf.titleTextED->text() );
        legend->setOrientation( ( conf.orientationCO->currentIndex() == 0 ) ? Qt::Vertical : Qt::Horizontal );
        item->setText( 0, conf.positionCO->currentText() );
        item->setText( 1, conf.showLinesCB->isChecked() ? "yes" : "no" );
        item->setText( 2, conf.titleTextED->text() );
        item->setText( 3, conf.orientationCO->currentText() );
        m_chart->update();
    }
}



void MainWindow::on_removeLegendPB_clicked()
{
    QList<QTreeWidgetItem*> items = legendsTV->selectedItems();
    for( QList<QTreeWidgetItem*>::const_iterator it = items.begin();
         it != items.end(); ++it ) {
        delete static_cast<LegendItem*>( (*it) )->legend();
        delete (*it);
    }
    m_chart->update();
}

void MainWindow::on_legendsTV_itemSelectionChanged()
{
    removeLegendPB->setEnabled( legendsTV->selectedItems().count() > 0 );
    removeLegendPB->setEnabled( legendsTV->selectedItems().count() == 1 );
}
