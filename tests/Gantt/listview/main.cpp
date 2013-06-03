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

#include <QApplication>
#include <QDateTime>
#include <QStandardItemModel>
#include <QDebug>
#include <QSlider>
#include <QVBoxLayout>
#include <QListView>

#include <KDGanttView>
#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttListViewRowController>

class MyStandardItem : public QStandardItem {
public:
    MyStandardItem( const QVariant& v, int role = Qt::DisplayRole ) : QStandardItem()
    {
        setData( v, role );
    }
    MyStandardItem( const QString& v ) : QStandardItem()
    {
        setData( v, Qt::DisplayRole );
    }
};

class MyListView : public QListView {
public:
  MyListView(QWidget* parent=0) : QListView(parent)
  {
    // Make some space for the header
    setViewportMargins(0, fontMetrics().height()*2+4*frameWidth(), 0, 0);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  }
};

class MyWidget : public QWidget {
    Q_OBJECT
public:
    MyWidget() : QWidget( 0 )
    {
        qDebug() << "Building data";
        //proxyModel.setSourceModel( &model );
	for ( int i = 0; i < 10; ++i ) {
	  QStandardItem* item = new MyStandardItem( QString::fromLatin1("Multi Item %1").arg(i) );
	  for ( int j=0; j < 20; j+=3 ) {
	    item->appendRow( QList<QStandardItem*>()
			     << new MyStandardItem( QString::fromLatin1( "Item %1" ).arg( j ) )
			     << new MyStandardItem( KDGantt::TypeTask )
			     << new MyStandardItem( QDateTime::currentDateTime().addDays( j ), KDGantt::StartTimeRole )
			     << new MyStandardItem( QDateTime::currentDateTime().addDays( j+1+i/7 ), KDGantt::EndTimeRole )
			     << new MyStandardItem( 50 ) );
	  }
	  item->appendRow( QList<QStandardItem*>()
			   << new MyStandardItem( QString::fromLatin1( "Event" ) )
			   << new MyStandardItem( KDGantt::TypeEvent )
			   << new MyStandardItem( QDateTime::currentDateTime(), KDGantt::StartTimeRole )
			   << new MyStandardItem( QDateTime(), KDGantt::EndTimeRole )
			   << new MyStandardItem( QString() ) );

	  model.appendRow( QList<QStandardItem*>()
			   << item
			   << new MyStandardItem( KDGantt::TypeMulti )
			   << new MyStandardItem( QString() )
			   << new MyStandardItem( QString() )
			   << new MyStandardItem( QString() ) );
	}

        //cmodel.addConstraint( KDGantt::Constraint( proxyModel.index( 0, 3 ), proxyModel.index( 10, 3 ) ) );
        //cmodel.addConstraint( KDGantt::Constraint( proxyModel.index( 10, 3 ), proxyModel.index( 5, 3 ) ) );
        QModelIndex pidx = model.index( 0, 0 );
        pidx = model.index( 0, 0, pidx );
        cmodel.addConstraint( KDGantt::Constraint( model.index( 0, 0, pidx), model.index( 1, 0, pidx ) ) );
        cmodel.addConstraint( KDGantt::Constraint( model.index( 1, 0, pidx), model.index( 0, 0, pidx ) ) );
        cmodel.addConstraint( KDGantt::Constraint( model.index( 1, 0, pidx), model.index( 10, 0, pidx ) ) );
        cmodel.addConstraint( KDGantt::Constraint( model.index( 3, 0, pidx), model.index( 5, 0, pidx ) ) );
        cmodel.addConstraint( KDGantt::Constraint( model.index( 7, 0, pidx), model.index( 4, 0, pidx ) ) );

        qDebug() << "Creating view";
        slider.setOrientation( Qt::Horizontal );
        slider.setRange( 1, 1000 );
        slider.setValue( 100 );
        QVBoxLayout* l = new QVBoxLayout( this );
        l->addWidget( &view );
        l->addWidget( &slider );
        grid.setStartDateTime( QDateTime::currentDateTime().addDays( -3 ) );
        grid.setDayWidth( 100 );
		grid.setFreeDays( QSet<Qt::DayOfWeek>() << Qt::Saturday << Qt::Sunday );
		grid.setFreeDaysBrush( QBrush( Qt::red ) );
	QListView* lv = new MyListView;
	view.setLeftView(lv);
	view.setRowController( new KDGantt::ListViewRowController(lv,view.ganttProxyModel()));
        view.setGrid( &grid );
        view.setModel(&model);
        //view.setConstraintModel( &cmodel );
        connect( &slider, SIGNAL( valueChanged( int ) ),
                 this, SLOT( slotZoom( int ) ) );
    }

public slots:
    void slotZoom( int z )
    {
        grid.setDayWidth( z );
    }
private:
    KDGantt::View view;
    KDGantt::DateTimeGrid grid;
    QSlider slider;
    QStandardItemModel model;
    KDGantt::ConstraintModel cmodel;
};

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    MyWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
