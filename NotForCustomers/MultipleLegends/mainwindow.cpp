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
#include <KDChartAbstractCoordinatePlane>
#include <KDChartDatasetProxyModel>
#include <KDChartLineDiagram>
#include <KDChartLegend>
#include <KDChartPosition>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

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
    m_model2 = new QStandardItemModel(rowCount, columnCount, this);
    m_model2->setHeaderData(0, Qt::Horizontal, tr("Product A"));
    m_model2->setHeaderData(1, Qt::Horizontal, tr("Product B"));
    m_model2->setHeaderData(2, Qt::Horizontal, tr("Product C"));
    openFile();

    // Set up the diagrams
    m_lines = new LineDiagram();
    m_lines->setModel( m_model );
    m_lines2 = new LineDiagram();
    m_lines2->setModel( m_model2 );

    // We call this "plane2" just for remembering, that we use it
    // in addition to the plane, that's built-in by default.
    plane2 = new CartesianCoordinatePlane( m_chart );

    CartesianAxis *xAxis = new CartesianAxis( m_lines );
    CartesianAxis *x2Axis = new CartesianAxis( m_lines2 );
    CartesianAxis *yAxis = new CartesianAxis ( m_lines );
    CartesianAxis *yAxis2 = new CartesianAxis ( m_lines2 );

    xAxis->setPosition ( CartesianAxis::Bottom );
    x2Axis->setPosition( CartesianAxis::Bottom );
    yAxis->setPosition ( CartesianAxis::Left );
    yAxis2->setPosition (CartesianAxis::Left );

    m_lines->addAxis( xAxis );
    m_lines2->addAxis( x2Axis );
    m_lines->addAxis( yAxis );
    m_lines2->addAxis( yAxis2 );

    // Make one of the diagram lines thicker
    for( int iColumn = 0; iColumn < columnCount; ++iColumn ){
        QPen linePen( m_lines->pen( iColumn ) );
        linePen.setWidth( 3 );
        m_lines->setPen( iColumn, linePen );
    }
    m_chart->coordinatePlane()->replaceDiagram( m_lines );

    plane2->replaceDiagram( m_lines2 );
    plane2->setReferenceCoordinatePlane( m_chart->coordinatePlane() );
    m_chart->addCoordinatePlane( plane2 );

    Legend* legend = new Legend( m_lines, m_chart );
    legend->setTitleText( "February - big Lines" );
    legend->setPosition( Position::South );
    legend->setAlignment( Qt::AlignCenter );
    legend->setShowLines( true );
    legend->setOrientation( Qt::Horizontal );
    m_chart->addLegend( legend );

    Legend* legend2 = new Legend( m_lines2, m_chart );
    legend2->setTitleText( "January - small lines" );
    legend2->setPosition( Position::North );
    legend2->setAlignment( Qt::AlignCenter );
    legend2->setShowLines( true );
    legend2->setOrientation( Qt::Horizontal );
    m_chart->addLegend( legend2 );

}


void MainWindow::openFile()
{
        QFile data1(":/Charts/qtdata.cht");

        if (data1.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&data1);
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

            data1.close();
        }
        QFile data2  ( ":/Charts/qtdatabis.cht" );
        if (data2.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&data2);
            QString line;

            m_model2->removeRows(0, m_model2->rowCount(QModelIndex()), QModelIndex());

            int row = 0;
            do {
                line = stream.readLine();
                if (!line.isEmpty()) {

                    m_model2->insertRows(row, 1, QModelIndex());

                    QStringList pieces = line.split(",", QString::SkipEmptyParts);
                    m_model2->setData(m_model2->index(row, 0, QModelIndex()), pieces.value(0));
                    m_model2->setData(m_model2->index(row, 1, QModelIndex()), pieces.value(1));
                    m_model2->setData(m_model2->index(row, 2, QModelIndex()), pieces.value(2));
                    ++row;
                }
            } while (!line.isEmpty());

            data2.close();
        }
}


void MainWindow::on_showDataset1CB_toggled( bool checked )
{
  setHidden( 0, ! checked );
}
void MainWindow::on_showDataset2CB_toggled( bool checked )
{
    setHidden( 1, ! checked );
}
void MainWindow::on_showDataset3CB_toggled( bool checked )
{
   setHidden( 2, ! checked );
}


void MainWindow::setHidden( int dataset, bool hidden )
{
    m_lines->setHidden( dataset, hidden );
    m_lines2->setHidden( dataset, hidden );
    m_chart->update();
}
