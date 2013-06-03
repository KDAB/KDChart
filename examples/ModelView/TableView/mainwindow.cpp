/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "mainwindow.h"

#include <QtGui>

#include <KDChartChart>
#include <KDChartHeaderFooter>
#include <KDChartDatasetProxyModel>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBarDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <QMenuBar>
#include <QMenu>
#include <QTableView>
#include <QSplitter>
#include <QFileDialog>
#include <QStatusBar>
#include <QApplication>


using namespace KDChart;


MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu( tr( "&File" ), this );
    QAction *openAction = fileMenu->addAction( tr( "&Open..." ) );
    openAction->setShortcut( QKeySequence( tr( "Ctrl+O" ) ) );
    QAction *saveAction = fileMenu->addAction( tr( "&Save As..." ) );
    saveAction->setShortcut( QKeySequence( tr( "Ctrl+S" ) ) );
    QAction *quitAction = fileMenu->addAction( tr( "E&xit" ) );
    quitAction->setShortcut( QKeySequence( tr( "Ctrl+Q" ) ) );

    setupModel();
    initializeData();
    setupViews();


    connect( openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect( saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect( quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect( m_selectionModel, SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
             this,               SLOT( selectionChanged( const QItemSelection &, const QItemSelection & ) ) );

    connect( m_diagramView, SIGNAL( clicked( const QModelIndex& ) ), SLOT( itemClicked( const QModelIndex& ) ) );

    menuBar()->addMenu( fileMenu );
    statusBar();

    setWindowTitle( tr( "KD Chart used as item viewer together with a QTableView" ) );
    resize( 740, 480 );
}

void MainWindow::setupModel()
{
    const int rowCount = 8;
    const int columnCount = 3;
    m_model = new QStandardItemModel( rowCount, columnCount, this );
    m_model->setHeaderData( 0, Qt::Horizontal, tr( "Label" ) );
    m_model->setHeaderData( 1, Qt::Horizontal, tr( "Quantity" ) );
    m_model->setHeaderData( 2, Qt::Horizontal, tr( "Product A") );

}

void MainWindow::initializeData()
{
    openFile(":/Charts/qtdata.cht");
}

void MainWindow::setupViews()
{
    m_chart = new Chart();

    m_tableView = new QTableView;

    QSplitter *splitter = new QSplitter( Qt::Vertical );
    splitter->addWidget( m_tableView );
    splitter->addWidget( m_chart );
    splitter->setStretchFactor( 0, 0 );
    splitter->setStretchFactor( 1, 1 );

    // Set up the diagram
    m_diagramView = new BarDiagram();

    DatasetDescriptionVector columnConfig( m_model->columnCount() - 1 );
    for ( int iC = 1; iC <= columnConfig.size(); ++iC )
        columnConfig[ iC - 1 ] = iC;

    qDebug() << "(" << m_model->rowCount() << "," << m_model->columnCount() << ")";

    KDChart::DatasetProxyModel* dproxy = new KDChart::DatasetProxyModel( this );
    dproxy->setSourceModel( m_model );
    dproxy->setDatasetColumnDescriptionVector( columnConfig );
    m_diagramView->setModel( dproxy );

    KDChart::HeaderFooter* headerFooter = new KDChart::HeaderFooter( m_chart );
    headerFooter->setText( tr( "You can edit the table data, or select table cells with keyboard/mouse." ) );
    headerFooter->setType( HeaderFooter::Header );
    headerFooter->setPosition( Position::North );
    m_chart->addHeaderFooter( headerFooter );
    m_chart->coordinatePlane()->replaceDiagram( m_diagramView );

    m_tableView->setModel( m_model );

    m_selectionModel = new QItemSelectionModel( m_model );
    m_tableView->setSelectionModel( m_selectionModel );

    setCentralWidget( splitter );
}

void MainWindow::itemClicked( const QModelIndex& index )
{
    QItemSelectionModel::SelectionFlags command = QItemSelectionModel::Clear | QItemSelectionModel::Select;
    if ( QApplication::keyboardModifiers() & Qt::ControlModifier )
        command = QItemSelectionModel::Toggle;
    m_selectionModel->setCurrentIndex( static_cast< const QAbstractProxyModel* >( index.model() )
                                           ->mapToSource( index ), command );
}

void MainWindow::selectionChanged( const QItemSelection & selected, const QItemSelection & deselected )
{
    if ( deselected != selected ) {

        // First we set pItemSelection to the de-selected bars
        // and we use an invisible pen, then we set it to the selected ones
        // using a dark blue pen:
        const QItemSelection * pItemSelection = &deselected;
        QPen pen( Qt::NoPen );

        for ( int iItemSelection = 0; iItemSelection < 2; ++iItemSelection ) {
            for ( int i = 0; i < pItemSelection->size(); ++i ) {
                QItemSelectionRange range( pItemSelection->at( i ) );
                for ( int iRow = range.topLeft().row(); iRow <= range.bottomRight().row(); ++iRow ) {
                    for ( int iColumn = range.topLeft().column(); iColumn <= range.bottomRight().column(); ++iColumn ) {
                        // ignore the first column: that's just the label texts to be shown in the table view
                        if ( iColumn )
                            // enable (or disable, resp.) the surrounding line around this bar
                            m_diagramView->setPen( m_diagramView->model()
                                ->index( iRow, iColumn - 1, m_diagramView->rootIndex()), pen );
                    }
                }
            }
            pItemSelection = &selected;
            pen.setColor( Qt::darkBlue );
            pen.setStyle( Qt::DashLine );
            pen.setWidth( 2 );
        }

        m_chart->update();
    }
}

void MainWindow::openFile(const QString &path)
{
    QString fileName;
    if ( path.isNull() )
        fileName = QFileDialog::getOpenFileName( this, tr( "Choose a data file" ), "", "*.cht" );
    else
        fileName = path;

    if ( !fileName.isEmpty() ) {
        QFile file(fileName);

        if ( file.open( QFile::ReadOnly | QFile::Text) ) {
            QTextStream stream( &file );
            QString line;

            m_model->removeRows( 0, m_model->rowCount( QModelIndex() ), QModelIndex() );

            int row = 0;
            do {
                line = stream.readLine();
                if (!line.isEmpty()) {

                    m_model->insertRows(row, 1, QModelIndex());

                    QStringList pieces = line.split( ',', QString::SkipEmptyParts );
                    m_model->setData( m_model->index( row, 0, QModelIndex() ),
                                      pieces.value( 0 ) );
                    m_model->setData( m_model->index(row, 1, QModelIndex() ),
                                      pieces.value( 1 ) );
                    m_model->setData( m_model->index( row, 2, QModelIndex() ),
                                      pieces.value( 2 ) );
                    m_model->setData( m_model->index(row, 0, QModelIndex()),
                                      QColor( pieces.value( 3 ) ), Qt::DecorationRole );
                    ++row;
                }
            } while ( !line.isEmpty() );

            file.close();
            statusBar()->showMessage( tr( "Loaded %1" ).arg( fileName ), 2000 );
        }
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr( "Save file as" ), "", "*.cht" );

    if ( !fileName.isEmpty() ) {
        QFile file( fileName );
        QTextStream stream( &file );

        if ( file.open(QFile::WriteOnly | QFile::Text) ) {
            for ( int row = 0; row < m_model->rowCount( QModelIndex() ); ++row ) {

                QStringList pieces;

                pieces.append( m_model->data( m_model->index( row, 0, QModelIndex() ),
                                              Qt::DisplayRole).toString());
                pieces.append( m_model->data( m_model->index( row, 1, QModelIndex() ),
                                              Qt::DisplayRole).toString());
                pieces.append( m_model->data( m_model->index( row, 2, QModelIndex() ),
                                              Qt::DisplayRole).toString());
                pieces.append( m_model->data( m_model->index( row, 0, QModelIndex() ),
                                              Qt::DecorationRole).toString());

                stream << pieces.join( "," ) << "\n";
            }
        }

        file.close();
        statusBar()->showMessage( tr( "Saved %1" ).arg( fileName ), 2000 );
    }
}
