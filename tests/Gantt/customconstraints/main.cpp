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
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QPixmapCache>
#include <QMenu>

#include <KDGanttView>
#include <KDGanttGraphicsView>
#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>

class MyGraphicsView : public KDGantt::GraphicsView {
    Q_OBJECT
public:
    MyGraphicsView( QWidget* parent=0 ) : KDGantt::GraphicsView( parent )
    {
    }

    /*reimp*/void addConstraint( const QModelIndex& from,
                                 const QModelIndex& to,
                                 Qt::KeyboardModifiers modifiers )
    {
        qDebug() << "MyGraphicsView::addConstraint()";
        if ( isReadOnly() ) return;
        KDGantt::ConstraintModel* cmodel = constraintModel();
        KDGantt::Constraint c( from, to, ( modifiers&Qt::ShiftModifier )?KDGantt::Constraint::TypeHard:KDGantt::Constraint::TypeSoft );
        c.setData( KDGantt::Constraint::ValidConstraintPen, QPen( QColor( Qt::green ), 3, Qt::DashLine ) );
        c.setData( KDGantt::Constraint::InvalidConstraintPen, QPen( QColor( Qt::blue ), 3, Qt::DashLine ) );
        if ( cmodel->hasConstraint( c ) ) cmodel->removeConstraint( c );
        else cmodel->addConstraint( c );
    }

};

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

class MyWidget : public QWidget {
    Q_OBJECT
public:
    MyWidget() : QWidget( 0 )
    {
        view.setGraphicsView( new MyGraphicsView );

        qDebug() << "Building data";
        //proxyModel.setSourceModel( &model );
        for ( int h = 0; h < 2; ++h ) {
            QList<QStandardItem*> items;
            items
                << new MyStandardItem( QString::fromLatin1( "Item %1" ).arg( h ) )
                << new MyStandardItem( KDGantt::TypeTask )
                << new MyStandardItem( QDateTime::currentDateTime().addDays( h ), KDGantt::StartTimeRole )
                << new MyStandardItem( QDateTime::currentDateTime().addDays( h+1 ), KDGantt::EndTimeRole )
                << new MyStandardItem( 50 );
            model.appendRow( items );
        }

        qDebug() << "Creating view";
        slider.setOrientation( Qt::Horizontal );
        slider.setRange( 1, 10000 );
        slider.setValue( 100 );
        QVBoxLayout* l = new QVBoxLayout( this );
        l->addWidget( &view );
        l->addWidget( &slider );
        grid.setStartDateTime( QDateTime::currentDateTime().addDays( -3 ) );
        grid.setDayWidth( 100 );
        //grid.setNoInformationBrush( Qt::NoBrush );
        view.setGrid( &grid );
        view.setModel(&model);

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
};

int main( int argc, char** argv ) {
    QApplication app( argc, argv );
    //QPixmapCache::setCacheLimit( 30*1024 );

    MyWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
