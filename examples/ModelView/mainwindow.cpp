/****************************************************************************
**
** Copyright (C) 2004-2006 Trolltech AS. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartDatasetProxyModel>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBarDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>


using namespace KDChart;


MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));
    QAction *saveAction = fileMenu->addAction(tr("&Save As..."));
    saveAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));

    setupModel();
    setupViews();

    openFile(":/Charts/qtdata.cht");

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    menuBar()->addMenu(fileMenu);
    statusBar();

    setWindowTitle(tr("KD Chart used as item viewer together with a QTableView"));
    resize(740, 480);
    //QTimer::singleShot(200, this, SLOT(initializeData()));
}

void MainWindow::setupModel()
{
    m_model = new QStandardItemModel(8, 3, this);
    m_model->setHeaderData(0, Qt::Horizontal, tr("Label"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Quantity"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Product A"));

    // you might want to add some dummy data:
    /*
    for( int row=0; row<8; ++row){
        m_model->setData(m_model->index(row, 0, QModelIndex()), 1);
        m_model->setData(m_model->index(row, 1, QModelIndex()), 2);
        m_model->setData(m_model->index(row, 2, QModelIndex()), 3);
    }
    */
}

void MainWindow::initializeData()
{
    openFile(":/Charts/qtdata.cht");
}

void MainWindow::setupViews()
{
    m_chart = new Chart();

    m_tableView = new QTableView;

    QSplitter *splitter = new QSplitter;
    splitter->addWidget(m_tableView);
    splitter->addWidget(m_chart);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    // Set up the diagram
    m_diagramView = new BarDiagram();
    m_diagramView->setModel( m_model );
    m_chart->coordinatePlane()->replaceDiagram( static_cast<BarDiagram *>(m_diagramView) );

    m_tableView->setModel( m_model );

    QItemSelectionModel *selectionModel = new QItemSelectionModel( m_model );
    m_tableView->setSelectionModel(   selectionModel );
    m_diagramView->setSelectionModel( selectionModel );

    setCentralWidget(splitter);
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
                    m_model->setData(m_model->index(row, 0, QModelIndex()),
                                   pieces.value(0));
                    m_model->setData(m_model->index(row, 1, QModelIndex()),
                                   pieces.value(1));
                    m_model->setData(m_model->index(row, 2, QModelIndex()),
                                   pieces.value(2));
                    m_model->setData(m_model->index(row, 0, QModelIndex()),
                                   QColor(pieces.value(3)), Qt::DecorationRole);
                    ++row;
                }
            } while (!line.isEmpty());

            file.close();
            statusBar()->showMessage(tr("Loaded %1").arg(fileName), 2000);
        }
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save file as"), "", "*.cht");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        QTextStream stream(&file);

        if (file.open(QFile::WriteOnly | QFile::Text)) {
            for (int row = 0; row < m_model->rowCount(QModelIndex()); ++row) {

                QStringList pieces;

                pieces.append(m_model->data(m_model->index(row, 0, QModelIndex()),
                                          Qt::DisplayRole).toString());
                pieces.append(m_model->data(m_model->index(row, 1, QModelIndex()),
                                          Qt::DisplayRole).toString());
                pieces.append(m_model->data(m_model->index(row, 2, QModelIndex()),
                                          Qt::DisplayRole).toString());
                pieces.append(m_model->data(m_model->index(row, 0, QModelIndex()),
                                          Qt::DecorationRole).toString());

                stream << pieces.join(",") << "\n";
            }
        }

        file.close();
        statusBar()->showMessage(tr("Saved %1").arg(fileName), 2000);
    }
}
