/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartHeaderFooter>
#include <KDChartLegend>
#include <KDChartPosition>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartDatasetProxyModel>
#include <QComboBox>
#include <QLineEdit>

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
    legend->setPosition( KDChart::Position::NorthEast );
    legend->setAlignment( Qt::AlignCenter );
    legend->setShowLines( false );
    legend->setTitleText( tr( "Legend" ) );
    legend->setOrientation( Qt::Vertical );
    m_chart->addLegend( legend );
    legend->show();

    LegendItem* newItem = new LegendItem( legend, legendsTV );
    newItem->setText( 0, tr( "NorthEast" ) );
    newItem->setText( 1, tr( "no" ) );
    newItem->setText( 2, tr( "Legend" ) );
    newItem->setText( 3, tr( "Vertical" ) );
    newItem->setText( 4, tr("Center") );

    alignmentMap[ Qt::AlignTop     | Qt::AlignLeft]    = tr("Top + Left");
    alignmentMap[ Qt::AlignTop     | Qt::AlignHCenter] = tr("Top + HCenter");
    alignmentMap[ Qt::AlignTop     | Qt::AlignRight]   = tr("Top + Right");
    alignmentMap[ Qt::AlignVCenter | Qt::AlignRight]   = tr("VCenter + Right");
    alignmentMap[ Qt::AlignBottom  | Qt::AlignRight]   = tr("Bottom + Right");
    alignmentMap[ Qt::AlignBottom  | Qt::AlignHCenter] = tr("Bottom + HCenter");
    alignmentMap[ Qt::AlignBottom  | Qt::AlignLeft]    = tr("Bottom + Left");
    alignmentMap[ Qt::AlignVCenter | Qt::AlignLeft]    = tr("VCenter + Left");
    alignmentMap[ Qt::AlignCenter]                     = tr("Center");

    m_chart->update();
}


void MainWindow::setupAddHeaderDialog( QDialog* dlg, Ui::AddHeaderDialog& conf )const
{
    conf.setupUi( dlg );
    conf.textED->setFocus();
    conf.positionCO->addItems( KDChart::Position::printableNames(true) );
}


void MainWindow::on_addHeaderPB_clicked()
{
    QDialog dlg;
    Ui::AddHeaderDialog conf;
    setupAddHeaderDialog( &dlg, conf );
    conf.typeCO->setCurrentIndex( 0 ); // let us start with "Header"
    conf.positionCO->setCurrentIndex( 0 );
    if( dlg.exec() ) {
        KDChart::HeaderFooter* headerFooter = new KDChart::HeaderFooter( m_chart );
        m_chart->addHeaderFooter( headerFooter );
        headerFooter->setText( conf.textED->text() );
        KDChart::TextAttributes attrs;
        attrs.setColor( Qt::red );
        headerFooter->setTextAttributes( attrs );
        headerFooter->setType(
            conf.typeCO->currentText() == "Header"
            ? KDChart::HeaderFooter::Header
            : KDChart::HeaderFooter::Footer );
        headerFooter->setPosition(
            KDChart::Position::fromPrintableName( conf.positionCO->currentText() ) );
        headerFooter->show();
        HeaderItem* newItem = new HeaderItem( headerFooter, headersTV );
        newItem->setText( 0, conf.textED->text() );
        newItem->setText( 1, headerFooter->type() == KDChart::HeaderFooter::Header
                        ? tr("Header")
                        : tr("Footer") );
        newItem->setText( 2, conf.positionCO->currentText() );
        m_chart->update();
    }
}


void MainWindow::on_editHeaderPB_clicked()
{
    if ( headersTV->selectedItems().size() == 0 ) return;
    HeaderItem* item = static_cast<HeaderItem*>( headersTV->selectedItems().first() );
    KDChart::HeaderFooter* headerFooter = item->header();
    QDialog dlg;
    Ui::AddHeaderDialog conf;
    setupAddHeaderDialog( &dlg, conf );
    conf.textED->setText( headerFooter->text() );
    conf.typeCO->setCurrentIndex(
        headerFooter->type() == KDChart::HeaderFooter::Header
            ? 0 : 1 );
    conf.positionCO->setCurrentIndex(
        conf.positionCO->findText( headerFooter->position().printableName() ) );
    if( dlg.exec() ) {
        headerFooter->setText( conf.textED->text() );
        headerFooter->setType(
            conf.typeCO->currentText() == "Header"
            ? KDChart::HeaderFooter::Header
            : KDChart::HeaderFooter::Footer );
        headerFooter->setPosition(
            KDChart::Position::fromPrintableName( conf.positionCO->currentText() ) );
        item->setText( 0, conf.textED->text() );
        item->setText( 1, headerFooter->type() == KDChart::HeaderFooter::Header
            ? tr("Header")
            : tr("Footer") );
        item->setText( 2, conf.positionCO->currentText() );
        m_chart->update();
    }
}




void MainWindow::on_removeHeaderPB_clicked()
{
    if ( headersTV->selectedItems().size() == 0 ) return;
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

void MainWindow::initAddLegendDialog( DerivedAddLegendDialog& conf,
                                      Qt::Alignment alignment ) const
{
    conf.titleTextED->setFocus();
    conf.positionCO->addItems( KDChart::Position::printableNames(false) );
    QMap<Qt::Alignment, QString>::const_iterator i = alignmentMap.constBegin();
    while (i != alignmentMap.constEnd()) {
        conf.alignmentCO->addItem( i.value() );
        ++i;
    }
    const int idx = conf.alignmentCO->findText( alignmentMap[ alignment ] );
    if( idx > -1 )
        conf.alignmentCO->setCurrentIndex( idx );
}

void MainWindow::on_addLegendPB_clicked()
{
    DerivedAddLegendDialog conf;
    initAddLegendDialog( conf, Qt::AlignCenter );
    if( conf.exec() == QDialog::Accepted ) {
        KDChart::Legend* legend = new KDChart::Legend( m_lines, m_chart );
        m_chart->addLegend( legend );
        legend->setPosition(
            KDChart::Position::fromPrintableName( conf.positionCO->currentText() ) );
        // get the alignment
        Qt::Alignment alignment = Qt::AlignCenter;
        const QString selectedAlignment( conf.alignmentCO->currentText() );
        QMap<Qt::Alignment, QString>::const_iterator i = alignmentMap.constBegin();
        while (i != alignmentMap.constEnd()) {
            if (  i.value() == selectedAlignment ){
                alignment = i.key();
                break;
            }
            ++i;
        }
        legend->setAlignment( alignment );
        legend->setShowLines( conf.showLinesCB->isChecked() );
        legend->setTitleText( conf.titleTextED->text() );
        legend->setOrientation( ( conf.orientationCO->currentIndex() == 0 ) ? Qt::Vertical : Qt::Horizontal );
        legend->show();
        LegendItem* newItem = new LegendItem( legend, legendsTV );
        newItem->setText( 0, conf.positionCO->currentText() );
        newItem->setText( 1, conf.showLinesCB->isChecked() ? tr("yes") : tr("no") );
        newItem->setText( 2, conf.titleTextED->text() );
        newItem->setText( 3, conf.orientationCO->currentText() );
        newItem->setText( 4, selectedAlignment );
        m_chart->update();
    }
}


void MainWindow::on_editLegendPB_clicked()
{
    if ( legendsTV->selectedItems().size() == 0 ) return;
    LegendItem* item = static_cast<LegendItem*>( legendsTV->selectedItems().first() );
    KDChart::Legend* legend = item->legend();
    DerivedAddLegendDialog conf;
    initAddLegendDialog( conf, legend->alignment() );
    conf.showLinesCB->setChecked( legend->showLines() );
    conf.titleTextED->setText( legend->titleText() );

    // In this example we are using legend position names, that match
    // exactly the names in KDChart::Legend::LegendPosition,
    // so we can use a shortcut here, to set the respective name in
    // the dialog's list, and we need no error checking for findText():
    conf.positionCO->setCurrentIndex(
            conf.positionCO->findText( legend->position().printableName() ) );
    conf.orientationCO->setCurrentIndex( (legend->orientation()==Qt::Vertical)?0:1 );

    if( conf.exec() == QDialog::Accepted ) {
        //legend->setPosition( (KDChart::Legend::LegendPosition)conf.positionCO->currentIndex() );
        legend->setPosition(
            KDChart::Position::fromPrintableName( conf.positionCO->currentText() ) );
        // get the alignment
        Qt::Alignment alignment = Qt::AlignCenter;
        const QString selectedAlignment( conf.alignmentCO->currentText() );
        QMap<Qt::Alignment, QString>::const_iterator i = alignmentMap.constBegin();
        while (i != alignmentMap.constEnd()) {
            if (  i.value() == selectedAlignment ){
                alignment = i.key();
                break;
            }
            ++i;
        }
        legend->setAlignment( alignment );
        legend->setShowLines( conf.showLinesCB->isChecked() );
        legend->setTitleText( conf.titleTextED->text() );
        legend->setOrientation( ( conf.orientationCO->currentIndex() == 0 ) ? Qt::Vertical : Qt::Horizontal );
        item->setText( 0, conf.positionCO->currentText() );
        item->setText( 1, conf.showLinesCB->isChecked() ? tr("yes") : tr("no") );
        item->setText( 2, conf.titleTextED->text() );
        item->setText( 3, conf.orientationCO->currentText() );
        item->setText( 4, selectedAlignment );
        m_chart->update();
    }
}



void MainWindow::on_removeLegendPB_clicked()
{
    if ( legendsTV->selectedItems().size() == 0 ) return;
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
