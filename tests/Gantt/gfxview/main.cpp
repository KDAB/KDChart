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
#include <QModelIndex>
#include <QStandardItemModel>
#include <QPushButton>
#include <QPointer>

#include <stdlib.h>

#include <KDGanttGraphicsView>
#include <KDGanttAbstractRowController>
#include <KDGanttStyleOptionGanttItem>

class MyRowController : public KDGantt::AbstractRowController {
private:
    static const int ROW_HEIGHT;
    QPointer<QAbstractItemModel> m_model;
public:
    MyRowController()
    {
    }

    void setModel( QAbstractItemModel* model )
    {
        m_model = model;
    }

    /*reimp*/int headerHeight() const { return 40; }

    /*reimp*/ bool isRowVisible( const QModelIndex& ) const { return true;}
    /*reimp*/ bool isRowExpanded( const QModelIndex& ) const { return false; }
    /*reimp*/ KDGantt::Span rowGeometry( const QModelIndex& idx ) const
    {
        qDebug() << "rowGeometry(" << idx.row()<<")";
        return KDGantt::Span( idx.row()*ROW_HEIGHT, ROW_HEIGHT );
    }
    /*reimp*/ int maximumItemHeight() const {
        return ROW_HEIGHT/2;
    }
    /*reimp*/ int totalHeight() const {
        return m_model.isNull()?0:m_model->rowCount()*ROW_HEIGHT;
    }

    /*reimp*/ QModelIndex indexAt( int height ) const {
        return m_model->index( height/ROW_HEIGHT, 0 );
    }

    /*reimp*/ QModelIndex indexBelow( const QModelIndex& idx ) const {
        if ( !idx.isValid() )return QModelIndex();
        return idx.model()->index( idx.row()+1, idx.column(), idx.parent() );
    }
    /*reimp*/ QModelIndex indexAbove( const QModelIndex& idx ) const {
        if ( !idx.isValid() )return QModelIndex();
        return idx.model()->index( idx.row()-1, idx.column(), idx.parent() );
    }

};

const int MyRowController::ROW_HEIGHT = 30;

class MyStandardItemModel : public QStandardItemModel {
public:
    /*reimp*/bool setData( const QModelIndex& idx, const QVariant& value, int role )
    {
        //qDebug() << "MyStandardItemModel::setData("<<idx<< value<<role<<")";
        return QStandardItemModel::setData( idx, value, role );
    }
};

static Qt::Alignment random_alignment()
{
    int r = (int) (3.0*rand()/(RAND_MAX+1.0));
    switch ( r ) {
    case 0: return Qt::AlignTop;
    case 1: return Qt::AlignBottom;
    case 2: return Qt::AlignCenter;
    }
    return Qt::Alignment();
}

static KDGantt::StyleOptionGanttItem::Position random_position()
{
    int r = (int) (3.0*rand()/(RAND_MAX+1.0));
    switch ( r ) {
    case 0: return KDGantt::StyleOptionGanttItem::Left;
    case 1: return KDGantt::StyleOptionGanttItem::Right;
    case 2:
    default: return KDGantt::StyleOptionGanttItem::Center;
    }
}

int main( int argc, char** argv ) {
    QApplication app( argc, argv );


    KDGantt::GraphicsView* view = new KDGantt::GraphicsView;
    MyRowController* rowController = new MyRowController;
    view->setReadOnly(true);
    view->setRowController( rowController );
    view->show();

    MyStandardItemModel model;
    for ( int i = 0; i < 10; ++i ) {
        QList<QStandardItem*> items;
        for ( int j = 0; j < 10; ++j ) {
            QStandardItem* item = new QStandardItem;
            //for ( int col = 0; col < 5; ++col ) {
            //  QStandardItem* colitem = new QStandardItem;
            item->setText( QString::fromLatin1( "Item %1" ).arg( i ) );
            item->setData( KDGantt::TypeTask, KDGantt::ItemTypeRole );
            item->setData( QDateTime::currentDateTime().addDays( 2*j ).addMSecs( 100000*i ), KDGantt::StartTimeRole );
            item->setData( QDateTime::currentDateTime().addDays( 2*j+1 ).addMSecs( 100000*i ), KDGantt::EndTimeRole );
            item->setData( qVariantFromValue<int>( random_alignment() ), Qt::TextAlignmentRole );
			item->setData( qVariantFromValue<int>( random_position()), KDGantt::TextPositionRole );
            item->setFlags( item->flags() & ~Qt::ItemIsSelectable );
            //  item->appendColumn( QList<QStandardItem*>() << colitem );
            //}
            items.push_back( item );
        }
        model.appendRow( items );
    }
    qDebug() << &model;
    rowController->setModel( &model );
    view->setModel( &model );
#if 0
    QPushButton* pb = new QPushButton( QObject::tr( "Reset" ) );
    QObject::connect( pb, SIGNAL( clicked() ),
                      &model, SIGNAL( modelReset() ) );
    pb->show();
#endif
    return app.exec();
    //return 0;
}
