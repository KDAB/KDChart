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

#include <QtGui>

#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartDatasetProxyModel>
#include <KDChartDataValueAttributes>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartThreeDLineAttributes>


#include <QTimer>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

    m_curColumn = -1;
    m_curOpacity = 0;

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );

    // Initialize the model, and fill it with data
    const int rowCount = 8;
    const int columnCount = 3;
    m_model = new QStandardItemModel(rowCount, columnCount, this);
    m_model->setHeaderData(0, Qt::Horizontal, tr("Product A"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Product B"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Product C"));
    openFile(":/Charts/qtdata.cht");

    // Set up the diagram
    m_lines = new LineDiagram();
    // Initialize and assign the proxy model to the diagram
    m_proxyModel = new KDChart::DatasetProxyModel(this);
    updateProxyModel();
    // Add axes to the diagram
    CartesianAxis *xAxis = new CartesianAxis( m_lines );
    CartesianAxis *yAxis = new CartesianAxis ( m_lines );
    xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
    yAxis->setPosition ( KDChart::CartesianAxis::Left );
    m_lines->addAxis( xAxis );
    m_lines->addAxis( yAxis );
    // Make the lines thicker
    for( int iColumn = 0; iColumn < columnCount; ++iColumn ){
        QPen linePen( m_lines->pen( iColumn ) );
        linePen.setWidth( 3 );
        m_lines->setPen( iColumn, linePen );
    }
    // Register the diagram at the coordinate plane
    m_chart->coordinatePlane()->replaceDiagram( m_lines );

    // Add a legend
    Legend* legend = new Legend( m_lines, m_chart );
    legend->setPosition( Position::South );
    legend->setAlignment( Qt::AlignCenter );
    legend->setShowLines( true );
    legend->setTitleText("");
    legend->setOrientation( Qt::Horizontal );
    m_chart->addLegend( legend );

    // Instantiate the timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_timerFired()));
    timer->start(40);
}

void MainWindow::on_showDataset1CB_toggled( bool checked )
{
    Q_UNUSED(checked);
    updateProxyModel();
}
void MainWindow::on_showDataset2CB_toggled( bool checked )
{
    Q_UNUSED(checked);
    updateProxyModel();
}
void MainWindow::on_showDataset3CB_toggled( bool checked )
{
    Q_UNUSED(checked);
    updateProxyModel();
}

void MainWindow::updateProxyModel()
{
    DatasetDescriptionVector columnConfig;
    if( showDataset1CB->isChecked() )
        columnConfig.append( 0 );
    if( showDataset2CB->isChecked() )
        columnConfig.append( 1 );
    if( showDataset3CB->isChecked() )
        columnConfig.append( 2 );

    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setDatasetColumnDescriptionVector( columnConfig );
    m_lines->setModel( m_proxyModel );

    m_chart->update();
}


void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if (path.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Choose a data file"),
            "", "*.cht");
    else
        fileName = path;

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QString line;

            m_model->removeRows(0, m_model->rowCount(QModelIndex()), QModelIndex());

            int row = 0;
            do {
                line = stream.readLine();
                if (!line.isEmpty()) {

                    m_model->insertRows(row, 1, QModelIndex());

                    QStringList pieces = line.split(",", QString::SkipEmptyParts);
                    m_model->setData(m_model->index(row, 0, QModelIndex()), pieces.value(0));
                    m_model->setData(m_model->index(row, 1, QModelIndex()), pieces.value(1));
                    m_model->setData(m_model->index(row, 2, QModelIndex()), pieces.value(2));
                    ++row;
                }
            } while (!line.isEmpty());

            file.close();
        }
    }
}

