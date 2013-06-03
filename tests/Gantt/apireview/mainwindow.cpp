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

#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttGraphicsView>
#include <KDGanttLegend>
#include <QAbstractItemView>
#include <QDebug>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QLabel>
#include <QPointer>

MainWindow::MainWindow( QWidget* parent, Qt::WindowFlags flags )
    : QMainWindow( parent, flags ),
      dayWidth( 70 ),
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

    connect( ui->ganttView->graphicsView(), SIGNAL( clicked( const QModelIndex& ) ),
             this, SLOT( slotClicked( const QModelIndex& ) ) );
    connect( ui->ganttView->graphicsView(), SIGNAL( qrealClicked( const QModelIndex& ) ),
             this, SLOT( slotDoubleClicked( const QModelIndex& ) ) );
}

void MainWindow::initModel()
{
    model = new QStandardItemModel( 0, 6, this );
    model->setHeaderData( 0, Qt::Horizontal, tr( "Task" ) );
    ui->ganttView->setModel( model );

    KDGantt::Legend* l = new KDGantt::Legend;
    l->setWindowTitle( tr( "Legend" ) );
    l->show();
    l->setModel( model );

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

    demoAction = new QAction( tr( "Demo entry" ), this );
    connect( demoAction, SIGNAL( triggered() ), this, SLOT( addDemoEntry() ) );

    printAction = new QAction( tr( "Print Preview..." ), this );
    connect( printAction, SIGNAL( triggered() ), this, SLOT( printPreview() ) );

    zoomInAction = new QAction( tr( "Zoom In" ), this );
    zoomInAction->setShortcut( QKeySequence::ZoomIn );
    connect( zoomInAction, SIGNAL( triggered() ), this, SLOT( zoomIn() ) );

    zoomOutAction = new QAction( tr( "Zoom Out" ), this );
    zoomOutAction->setShortcut( QKeySequence::ZoomOut );
    connect( zoomOutAction, SIGNAL( triggered() ), this, SLOT( zoomOut() ) );

    ui->ganttView->leftView()->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->ganttView->leftView()->addAction( newEntryAction );
    ui->ganttView->leftView()->addAction( removeEntryAction );

    QMenu* entryMenu = menuBar()->addMenu( tr( "Entry" ) );
    entryMenu->addAction( newEntryAction );
    entryMenu->addAction( removeEntryAction );
    entryMenu->addSeparator();
    entryMenu->addAction( demoAction );
    entryMenu->addSeparator();
    entryMenu->addAction( printAction );

    QMenu* zoomMenu = menuBar()->addMenu( tr( "Zoom" ) );
    zoomMenu->addAction( zoomInAction );
    zoomMenu->addAction( zoomOutAction );

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
    grid->setDayWidth( dayWidth );
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
    model->setData( model->index( row, 5, parent ), dialog->legend() );

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

void MainWindow::addConstraint( const QStandardItem* item1,  const QStandardItem* item2 )
{
    addConstraint( model->indexFromItem( item1 ), model->indexFromItem( item2 ) );
}

void MainWindow::removeEntry()
{
    QModelIndexList selectedIndexes = ui->ganttView->selectionModel()->selectedIndexes();
    QModelIndex index = selectedIndexes.value( 0 );

    if ( !index.isValid() )
        return;

    model->removeRow( index.row(), index.parent() );
}

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
    MyStandardItem( const QDateTime& dt, int role ) : QStandardItem()
    {
        setData( QVariant::fromValue( dt ), role );
    }
};

void MainWindow::addDemoEntry()
{
    QStandardItem* softwareRelease = new MyStandardItem( tr("Software Release" ) );
    QStandardItem* codeFreeze = new MyStandardItem( tr( "Code Freeze" ) );
    codeFreeze->setData( KDGantt::TextPositionRole, KDGantt::StyleOptionGanttItem::Right );
    QStandardItem* packaging = new MyStandardItem( tr( "Packaging" ) );
    QStandardItem* upload = new MyStandardItem( tr( "Upload" ) );
    QStandardItem* testing = new MyStandardItem( tr( "Testing" ) );
    QStandardItem* updateDocumentation = new MyStandardItem( tr( "Update Documentation" ) );

    QDateTime now = QDateTime::currentDateTime();

    softwareRelease->appendRow( QList<QStandardItem*>()
                                << codeFreeze << new MyStandardItem( KDGantt::TypeEvent )
                                << new MyStandardItem( now, KDGantt::StartTimeRole ) );
    softwareRelease->appendRow( QList<QStandardItem*>()
                                << packaging << new MyStandardItem( KDGantt::TypeTask )
                                << new MyStandardItem( now.addDays( 5 ), KDGantt::StartTimeRole )
                                << new MyStandardItem( now.addDays( 10 ), KDGantt::EndTimeRole ) );
    softwareRelease->appendRow( QList<QStandardItem*>()
                                << upload << new MyStandardItem( KDGantt::TypeTask )
                                << new MyStandardItem( now.addDays( 10 ).addSecs( 2*60*60 ), KDGantt::StartTimeRole )
                                << new MyStandardItem( now.addDays( 11 ), KDGantt::EndTimeRole ) );
    softwareRelease->appendRow( QList<QStandardItem*>()
                                << testing << new MyStandardItem( KDGantt::TypeTask )
                                << new MyStandardItem( now.addSecs( 3*60*60 ), KDGantt::StartTimeRole )
                                << new MyStandardItem( now.addDays( 5 ), KDGantt::EndTimeRole ) );
    softwareRelease->appendRow( QList<QStandardItem*>()
                                << updateDocumentation << new MyStandardItem( KDGantt::TypeTask )
                                << new MyStandardItem( now.addSecs( 3*60*60 ), KDGantt::StartTimeRole )
                                << new MyStandardItem( now.addDays( 3 ), KDGantt::EndTimeRole ) );

    model->appendRow( QList<QStandardItem*>()
                      << softwareRelease << new MyStandardItem( KDGantt::TypeSummary ) );

    addConstraint( codeFreeze, packaging );
    addConstraint( codeFreeze, testing );
    addConstraint( codeFreeze, updateDocumentation );
    addConstraint( packaging, upload );
    addConstraint( testing, packaging );
    addConstraint( updateDocumentation, packaging );
}

void MainWindow::zoomIn()
{
    dayWidth += 10;
    if ( dayWidth > 400 )
        grid->setScale( KDGantt::DateTimeGrid::ScaleHour );

    grid->setDayWidth( dayWidth );
}

void MainWindow::zoomOut()
{
    dayWidth -= 10;
    if ( dayWidth < 10 )
        dayWidth = 10;

    if ( dayWidth <= 400 )
        grid->setScale( KDGantt::DateTimeGrid::ScaleDay );

    grid->setDayWidth( dayWidth );
}

void MainWindow::printPreview()
{
    QLabel* preview = new QLabel( this, Qt::Window );
    preview->setAttribute( Qt::WA_DeleteOnClose );
    preview->setScaledContents( true );
    preview->setWindowTitle( tr( "Print Preview" ) );
    QPixmap pix( 1000, 300 );
    pix.fill( Qt::white );
    {
        QPainter p( &pix );
        p.setRenderHints( QPainter::Antialiasing );
        ui->ganttView->print( &p, pix.rect() );
    }
    preview->setPixmap( pix );
    preview->show();
}

void MainWindow::slotClicked( const QModelIndex& index )
{
    statusBar()->showMessage( tr( "(%1,%2,_,%4) clicked" )
                          .arg( index.row() )
                          .arg( index.column() )
                          .arg( ( quint64 )index.model() ) );
}
void MainWindow::slotDoubleClicked( const QModelIndex& index )
{
    statusBar()->showMessage( tr( "(%1,%2,_,%4) qreal clicked" )
                          .arg( index.row() )
                          .arg( index.column() )
                          .arg( ( quint64 )index.model() ) );
}
