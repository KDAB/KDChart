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
#include <QTimer>

#include <cassert>

#include <KDGanttView>
#include <KDGanttConstraintModel>

/* Test model that uses beginMoveRows() */
class MyTaskModel : public QAbstractTableModel {
    struct Task {
        QString title;
        QDateTime start, end;
    };
    QList<Task> m_tasks;
public:
    MyTaskModel( QObject* parent=0 )
        : QAbstractTableModel( parent )
    {
        Task task;
        task.title = tr( "Item 1" );
        task.start = QDateTime::currentDateTime();
        task.end = task.start.addDays( 1 );
        m_tasks.push_back( task );
        task.title = tr( "Item 2" );
        task.start = QDateTime::currentDateTime().addDays( 2 );
        task.end = task.start.addDays( 1 );
        m_tasks.push_back( task );
        task.title = tr( "Item 3" );
        task.start = QDateTime::currentDateTime().addDays( 4 );
        task.end = task.start.addDays( 1 );
        m_tasks.push_back( task );
        task.title = tr( "Item 4" );
        task.start = QDateTime::currentDateTime().addDays( 6 );
        task.end = task.start.addDays( 1 );
        m_tasks.push_back( task );
    }

    /*reimp*/int rowCount( const QModelIndex& index = QModelIndex() ) const {
        return index.isValid()?0:m_tasks.size();
    }
    /*reimp*/int columnCount( const QModelIndex& index = QModelIndex() ) const {
        return index.isValid()?0:4;
    }
    /*reimp*/QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const
    {
        if ( index.isValid() && index.row() < rowCount() && index.column() < columnCount() ) {
            switch ( index.column() ) {
            case 0: return ( role == Qt::DisplayRole )
                    ?QVariant::fromValue( m_tasks[index.row()].title )
                    :QVariant();
            case 1: return ( role == Qt::DisplayRole )
                    ?QVariant::fromValue( ( int )KDGantt::TypeTask )
                    :QVariant();
            case 2: return ( role == KDGantt::StartTimeRole || role == Qt::DisplayRole )
                    ?QVariant::fromValue( m_tasks[index.row()].start )
                    :QVariant();
            case 3: return ( role == KDGantt::EndTimeRole || role == Qt::DisplayRole)
                    ?QVariant::fromValue( m_tasks[index.row()].end )
                    :QVariant();
            }
        }
        return QVariant();
    }

    void moveRow( int from, int to ) {
        if ( from == to ) return;
        if ( from >= m_tasks.size() || to >= m_tasks.size()+1 ) return;

        if ( beginMoveRows( QModelIndex(), from, from, QModelIndex(), to ) ) {
            m_tasks.move( from, to );
            endMoveRows();
        } else {
            assert( 0 );
        }
    }
};

/* Test class to see the effect of moving rows */
class MoveHelper : public QObject {
public:
    MoveHelper( MyTaskModel* model,
                KDGantt::ConstraintModel* constraints,
                KDGantt::View* view,
                int row1, int row2 )
        : QObject( model ),
          m_model( model ),
          m_constraints( constraints ),
          m_view( view ),
          m_row1( row1 ),
          m_row2( row2 )
    {
    }

    void showContraints( const QString& /*pfx*/ ) {
#if 0
        qDebug() << pfx << *m_constraints;
        qDebug() << "0:" << m_constraints->constraintsForIndex( m_model->index( 0, 0 ) );
        qDebug() << "1:"<< m_constraints->constraintsForIndex( m_model->index( 1, 0 ) );
        qDebug() << "2:"<< m_constraints->constraintsForIndex( m_model->index( 2, 0 ) );
        qDebug() << "3:"<< m_constraints->constraintsForIndex( m_model->index( 3, 0 ) );
#endif
    }

    ~MoveHelper() {
        qDebug() << "Moving row" << m_row1 << "to" << m_row2;
        showContraints( "Before:" );
        m_model->moveRow( m_row1, m_row2 );
        showContraints( "After:" );

        // Hack until KDGantt supports this:
        //m_view->setConstraintModel( m_constraints );
    }

private:
    MyTaskModel* m_model;
    KDGantt::ConstraintModel* m_constraints;
    KDGantt::View* m_view;
    int m_row1, m_row2;
};

int main( int argc, char** argv )
{
    QApplication app( argc, argv );


    KDGantt::View* view = new KDGantt::View;

    MyTaskModel model;

    KDGantt::ConstraintModel constraints;
    constraints.addConstraint( KDGantt::Constraint( model.index( 0, 0 ), model.index( 1, 0 ) ) );
    constraints.addConstraint( KDGantt::Constraint( model.index( 0, 0 ), model.index( 2, 0 ) ) );
    constraints.addConstraint( KDGantt::Constraint( model.index( 1, 0 ), model.index( 3, 0 ) ) );

    view->setModel( &model );
    view->setConstraintModel( &constraints );
    view->show();

    /* After 5 seconds, move row 1 to pos 0: */
    QTimer::singleShot( 5000, new MoveHelper( &model, &constraints, view, 1, 0 ), SLOT( deleteLater() ) );

    return app.exec();
}
