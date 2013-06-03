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

#include "ui_mainwindow.h"
#include "entrydelegate.h"
#include "entrydialog.h"

#include <algorithm>

#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttGraphicsView>
#include <KDGanttLegend>
#include <QAbstractItemView>
#include <QDebug>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QTreeView>
#include <QCloseEvent>
#include <QPointer>
#include <QScrollBar>

class MyStandardItem : public QStandardItem {
public:
  MyStandardItem( const QVariant& v ) : QStandardItem()
  {
    setData( v, Qt::DisplayRole );
  }
  MyStandardItem( const QString& v ) : QStandardItem()
  {
    setData( v, Qt::DisplayRole );
  }
};

MainWindow::MainWindow( QWidget* parent, Qt::WindowFlags flags )
    : QMainWindow( parent, flags ),
      smallLegend( 0 ),
      detailedLegend( 0 ),
      ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    initModel();
    initActions();
    initItemDelegate();
    initGrid();

    QTreeView* leftView = qobject_cast<QTreeView*>( ui->ganttView->leftView() );
    Q_ASSERT( leftView );
    leftView->setColumnHidden( 1, true );
    leftView->setColumnHidden( 2, true );
    leftView->setColumnHidden( 3, true );
    leftView->setColumnHidden( 4, true );
    leftView->setColumnHidden( 5, true );
    leftView->header()->setStretchLastSection( true );

    connect( ui->ganttView->leftView(), SIGNAL( customContextMenuRequested( const QPoint& ) ),
             this, SLOT( showContextMenu( const QPoint& ) ) );
    connect( ui->ganttView->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
             this, SLOT( enableActions( const QItemSelection& ) ) );
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    delete smallLegend;
    delete detailedLegend;
    event->accept();
}

void MainWindow::initModel()
{
    model = new QStandardItemModel( 0, 6, this );
    model->setHeaderData( 0, Qt::Horizontal, tr( "Tree View of Entries" ) );
    ui->ganttView->setModel( model );

    QStandardItemModel* lmodel = new QStandardItemModel;
    lmodel->appendRow( QList<QStandardItem*>()
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( KDGantt::TypeEvent )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QString::fromLatin1("Event") ) );
    lmodel->appendRow( QList<QStandardItem*>()
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( KDGantt::TypeTask )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QString::fromLatin1("Task") ) );
    lmodel->appendRow( QList<QStandardItem*>()
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( KDGantt::TypeSummary )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QVariant() )
        << new MyStandardItem( QString::fromLatin1("Summary") ) );

    smallLegend = new KDGantt::Legend();
    smallLegend->setWindowTitle( tr( "Legend" ) );
    smallLegend->show();
    smallLegend->setModel( lmodel );

    detailedLegend = new KDGantt::Legend();
    detailedLegend->setWindowTitle( tr( "List" ) );
    detailedLegend->show();
    detailedLegend->setModel( model );

    constraintModel = new KDGantt::ConstraintModel( this );
    ui->ganttView->setConstraintModel( constraintModel );
}

void MainWindow::initActions()
{
    newEntryAction = new QAction( tr( "New entry" ), this );
    newEntryAction->setShortcut( QKeySequence::New );
    connect( newEntryAction, SIGNAL( triggered() ), this, SLOT( addNewEntry() ) );

    removeEntryAction = new QAction( tr( "Remove entry" ), this );
    removeEntryAction->setShortcut( QKeySequence::Delete );
    connect( removeEntryAction, SIGNAL( triggered() ), this, SLOT( removeEntry() ) );

    zoomInAction = new QAction( tr( "Zoom In" ), this );
    zoomInAction->setShortcut( QKeySequence::ZoomIn );
    connect( zoomInAction, SIGNAL( triggered() ), this, SLOT( zoomIn() ) );

    zoomOutAction = new QAction( tr( "Zoom Out" ), this );
    zoomOutAction->setShortcut( QKeySequence::ZoomOut );
    connect( zoomOutAction, SIGNAL( triggered() ), this, SLOT( zoomOut() ) );

    zoomFitAction = new QAction( tr( "Zoom to Fit" ), this );
    connect( zoomFitAction, SIGNAL( triggered() ), this, SLOT( zoomFit() ) );

    ui->ganttView->leftView()->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->ganttView->leftView()->addAction( newEntryAction );
    ui->ganttView->leftView()->addAction( removeEntryAction );

    QMenu* entryMenu = menuBar()->addMenu( tr( "Entry" ) );
    entryMenu->addAction( newEntryAction );
    entryMenu->addAction( removeEntryAction );

    QMenu* zoomMenu = menuBar()->addMenu( tr( "Zoom" ) );
    zoomMenu->addAction( zoomInAction );
    zoomMenu->addAction( zoomOutAction );
    zoomMenu->addAction( zoomFitAction );
    
    QMenu* scaleMenu = menuBar()->addMenu( tr( "Scale" ) );
    
    scaleMenu->addAction( tr( "Auto" ), this, SLOT(scaleAuto()) );
    scaleMenu->addAction( tr( "Hour" ), this, SLOT(scaleHour()) );
    scaleMenu->addAction( tr( "Day" ), this, SLOT(scaleDay()) );
    scaleMenu->addAction( tr( "Week" ), this, SLOT(scaleWeek()) );
    scaleMenu->addAction( tr( "Month" ), this, SLOT(scaleMonth()) );

    enableActions( QItemSelection() );
}

void MainWindow::initItemDelegate()
{
    EntryDelegate* delegate = new EntryDelegate( constraintModel, this );
    ui->ganttView->leftView()->setItemDelegate( delegate );
}

void MainWindow::initGrid()
{
    grid = new KDGantt::DateTimeGrid();
    grid->setDayWidth( 70 );
    ui->ganttView->setGrid( grid );
}

void MainWindow::showContextMenu( const QPoint& pos )
{
    if ( !ui->ganttView->leftView()->indexAt( pos ).isValid() )
        ui->ganttView->selectionModel()->clearSelection();

    QMenu menu( ui->ganttView->leftView() );
    menu.addAction( newEntryAction );
    menu.addAction( removeEntryAction );
    menu.exec( ui->ganttView->leftView()->viewport()->mapToGlobal( pos ) );
}

void MainWindow::enableActions(const QItemSelection & selected)
{
    if ( selected.indexes().isEmpty() ) {
        newEntryAction->setEnabled( true );
        removeEntryAction->setEnabled( false );
        return;
    }

    QModelIndex selectedIndex = selected.indexes()[0];

    if ( model->data( model->index( selectedIndex.row(), 1 ) ) == KDGantt::TypeEvent ||
        model->data( model->index( selectedIndex.row(), 1 ) ) == KDGantt::TypeTask ) {
        newEntryAction->setEnabled( false );
        removeEntryAction->setEnabled( true );
        return;
    }

    newEntryAction->setEnabled( true );
    removeEntryAction->setEnabled( true );
}

void MainWindow::addNewEntry()
{
    QPointer<EntryDialog> dialog = new EntryDialog( model );
    dialog->setWindowTitle( tr( "New Entry") );
    if ( dialog->exec() == QDialog::Rejected || !dialog ) {
        delete dialog;
        return;
    }

    QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();
    const QModelIndex parent = selectedIndexes.value( 0 );

    if ( !model->insertRow( model->rowCount( parent ), parent ) )
        return;

    int row = model->rowCount( parent ) - 1;
    if ( row == 0 && parent.isValid() )
        model->insertColumns( model->columnCount( parent ), 5, parent );

    model->setData( model->index( row, 0, parent ), dialog->name() );
    model->setData( model->index( row, 1, parent ), dialog->type() );
    if ( dialog->type() != KDGantt::TypeSummary ) {
        model->setData( model->index( row, 2, parent ), dialog->startDate(), KDGantt::StartTimeRole );
        model->setData( model->index( row, 3, parent ), dialog->endDate(), KDGantt::EndTimeRole );
    }
    model->setData( model->index( row, 4, parent ), dialog->completion() );
    const QString legend( dialog->legend() );
    if ( ! legend.isEmpty() )
        model->setData( model->index( row, 5, parent ), legend );

    addConstraint( dialog->depends(), model->index( row, 0, parent ) );
    setReadOnly( model->index( row, 0, parent ), dialog->readOnly() );

    delete dialog;
}

void MainWindow::setReadOnly(const QModelIndex & index, bool readOnly)
{
    int row = index.row();
    const QModelIndex parent = index.parent();
    QStandardItem* item;

    item = model->itemFromIndex( model->index( row, 0, parent ) );
    item->setFlags( readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable );

    item = model->itemFromIndex( model->index( row, 1, parent ) );
    item->setFlags( readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable );

    item = model->itemFromIndex( model->index( row, 2, parent ) );
    item->setFlags( readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable );

    item = model->itemFromIndex( model->index( row, 3, parent ) );
    item->setFlags( readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable );

    item = model->itemFromIndex( model->index( row, 4, parent ) );
    item->setFlags( readOnly ? item->flags() & ~Qt::ItemIsEditable : item->flags() | Qt::ItemIsEditable );
}

void MainWindow::addConstraint(const QModelIndex & index1, const QModelIndex & index2)
{
    if ( !index1.isValid() || !index2.isValid() )
        return;

    KDGantt::Constraint c( index1, index2 );
    ui->ganttView->constraintModel()->addConstraint( c );
}

void MainWindow::removeEntry()
{
    QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();
    QModelIndex index = selectedIndexes.value( 0 );

    if ( !index.isValid() )
        return;

    model->removeRow( index.row(), index.parent() );
}

void MainWindow::zoomIn()
{
    qreal dayWidth = grid->dayWidth() + 10;
    if ( dayWidth > 400 )
        grid->setScale( KDGantt::DateTimeGrid::ScaleHour );

    grid->setDayWidth( dayWidth );
}

void MainWindow::zoomOut()
{
    qreal dayWidth = grid->dayWidth() - 10;
    if ( dayWidth < 10 )
        dayWidth = 10;

    if ( dayWidth <= 400 )
        grid->setScale( KDGantt::DateTimeGrid::ScaleDay );

    grid->setDayWidth( dayWidth );
}

void MainWindow::zoomFit()
{
    QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();

    if ( selectedIndexes.isEmpty() ) {
        return;
    }

    KDGantt::Span span;
    Q_FOREACH( QModelIndex idx, selectedIndexes ) {
        const KDGantt::Span s = grid->mapToChart( grid->model()->index( idx.row(), 0 ) );
        if ( span.isValid() ) {
            span = span.expandedTo( s );
        } else {
            span = s;
        }
    }

    span.setLength( span.length()+20 );
    span.setStart( span.start()-10 );

    qDebug() << selectedIndexes << span;

    const qreal view_width = ui->ganttView->graphicsView()->viewport()->width();
    const QDateTime start = grid->mapFromChart( span.start() ).value<QDateTime>();
    const QDateTime end = grid->mapFromChart( span.end() ).value<QDateTime>();

    qreal delta = start.date().daysTo(end.date());
    delta += start.time().msecsTo(end.time())/( 1000.*24.*60.*60. );

    qDebug() << view_width << "/" << delta;
    grid->setDayWidth( view_width/( std::max( 1., delta ) ) );
    qDebug() << "daywidth set to" << grid->dayWidth();
    qDebug() << "start scroll to" << grid->mapToChart( start );
    ui->ganttView->graphicsView()->horizontalScrollBar()->setValue( grid->mapToChart( start ) );
}

void MainWindow::scaleAuto()
{
    KDGantt::DateTimeGrid* grid = static_cast<KDGantt::DateTimeGrid*>(ui->ganttView->grid());
    grid->setScale( KDGantt::DateTimeGrid::ScaleAuto );
}

void MainWindow::scaleHour()
{
    KDGantt::DateTimeGrid* grid = static_cast<KDGantt::DateTimeGrid*>(ui->ganttView->grid());
    grid->setScale( KDGantt::DateTimeGrid::ScaleHour );
}

void MainWindow::scaleDay()
{
    KDGantt::DateTimeGrid* grid = static_cast<KDGantt::DateTimeGrid*>(ui->ganttView->grid());
    grid->setScale( KDGantt::DateTimeGrid::ScaleDay );
}

void MainWindow::scaleWeek()
{
    KDGantt::DateTimeGrid* grid = static_cast<KDGantt::DateTimeGrid*>(ui->ganttView->grid());
    grid->setScale( KDGantt::DateTimeGrid::ScaleWeek );
}

void MainWindow::scaleMonth()
{
    KDGantt::DateTimeGrid* grid = static_cast<KDGantt::DateTimeGrid*>(ui->ganttView->grid());
    grid->setScale( KDGantt::DateTimeGrid::ScaleMonth );
}
    
