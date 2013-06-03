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

#include "projectmodel.h"

#include <QList>
#include <QDateTime>
#include <QFile>

#include <QDebug>

#include <KDGanttGlobal>
#include <KDGanttStyleOptionGanttItem>

#include <cassert>

typedef QAbstractItemModel BASE;

class ProjectModel::Node {
public:
    explicit Node( Node* parent = 0 );
    virtual ~Node();

    void addChild( Node* );
    void insertChild( int i, Node* );
    void removeChild( Node* );

    Node* parent() const { return m_parent; }
    int childCount() const { return m_children.count(); }
    int childNumber( Node* n ) const { return m_children.indexOf( n ); }
    Node* child( int i ) const {
        return m_children.at( i );
    }
    void setParent( Node* p ) {
        m_parent = p;
    }

    void setStart( const QDateTime& dt ) {
        m_bStart = m_start;
        m_start = dt;
    }
    void setEnd( const QDateTime& dt ) {
        if ( m_end.isValid() )
        m_bEnd = m_end;
        m_end = dt;
    }
    void setLabel( const QString& str ) {
        m_label = str;
    }
    void setType( int t ) {
        m_type = static_cast<KDGantt::ItemType>( t );
        if ( !m_start.isValid() )
            m_start = m_bStart;
        if ( !m_end.isValid() )
            m_end = m_bEnd;
    }
    void setCompletion( int c ) {
        m_completion = c;
    }
    void setPosition( KDGantt::StyleOptionGanttItem::Position p ) {
        m_position = p;
    }

    QDateTime start() const { return m_start; }
    QDateTime end() const { return m_end; }

    QString label() const { return m_label; }

    KDGantt::ItemType type() const { return m_type; }

    int completion() const { return m_completion; }
    
    KDGantt::StyleOptionGanttItem::Position position() const { return m_position; }

private:
    Node* m_parent;
    QList<Node*> m_children;

    KDGantt::ItemType m_type;
    QDateTime m_start, m_end;
    QDateTime m_bStart, m_bEnd;
    QString m_label;
    int m_completion;
    KDGantt::StyleOptionGanttItem::Position m_position;
};

static int unnamed_count = 0;

ProjectModel::Node::Node( Node* parent )
    : m_parent( parent ),
      m_type( KDGantt::TypeTask ),
      m_start( QDateTime::currentDateTime() ),
      m_end( QDateTime::currentDateTime().addDays( 1 ) ),
      m_label( tr( "Unnamed task %1" ).arg( ++unnamed_count ) ),
      m_completion( -1 ),
      m_position( KDGantt::StyleOptionGanttItem::Right )
{
    if ( m_parent ) m_parent->addChild( this );
}

ProjectModel::Node::~Node()
{
    while ( !m_children.isEmpty() ) delete m_children.front();
    if ( m_parent ) m_parent->removeChild( this );
}

void ProjectModel::Node::addChild( Node* child )
{
    child->setParent( this );
    m_children.push_back( child );
}

void ProjectModel::Node::insertChild( int i, Node* child )
{
    child->setParent( this );
    m_children.insert( i, child );
}

void ProjectModel::Node::removeChild( Node* child )
{
    child->setParent( 0 );
    m_children.removeAll( child );
}


ProjectModel::ProjectModel( QObject* parent )
    : QAbstractItemModel( parent ), m_root( new Node )
{
}

ProjectModel::~ProjectModel()
{
    delete m_root;
}

bool ProjectModel::load( const QString& filename )
{
    Q_UNUSED(filename)
    // TODO: read data
    delete m_root;
    m_root = new Node;
 
    return true;
}

bool ProjectModel::save( const QString& filename )
{
    Q_UNUSED(filename);
    return true;
}

int ProjectModel::rowCount( const QModelIndex& idx ) const
{
    if ( idx.isValid() ) return static_cast<Node*>( idx.internalPointer() )->childCount();
    else return m_root->childCount();
}

int ProjectModel::columnCount( const QModelIndex& idx ) const
{
    Q_UNUSED(idx);
    return 5;
}

QModelIndex ProjectModel::index( int row, int col, const QModelIndex& parent ) const
{
    Node* p = m_root;
    if ( parent.isValid() ) {
        p = static_cast<Node*>( parent.internalPointer() );
    }
    if ( row < 0 || row >= p->childCount() ) return QModelIndex();
    else return createIndex( row, col, p->child( row ) );
}

QModelIndex ProjectModel::parent( const QModelIndex& idx ) const
{
    if ( !idx.isValid() ) return QModelIndex();

    Node* n = static_cast<Node*>( idx.internalPointer() );
    assert( n );
    Node* p = n->parent();
    if ( p==m_root )return QModelIndex();

    Node* pp = p->parent();
    assert( pp );
    return createIndex( pp->childNumber( p ), 0, p );
}

QVariant ProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation != Qt::Horizontal || role != Qt::DisplayRole ) return QVariant();
    switch ( section ) {
    case 0: return tr( "Name" );
    case 1: return tr( "Type" );
    case 2: return tr( "Start" );
    case 3: return tr( "End" );
    case 4: return tr( "Completion %" );
    default: return QVariant();
    }
}

QVariant ProjectModel::data( const QModelIndex& idx, int role ) const
{
    if ( !idx.isValid() ) return QVariant();

    Node* n = static_cast<Node*>( idx.internalPointer() );
    assert( n );
    if ( idx.column() == 0 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
			return n->label();
        case KDGantt::TextPositionRole:
            return n->position();
        }
    } else if ( idx.column() == 1 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return qVariantFromValue<int>( n->type() );
        }
    } else if ( idx.column() == 2 ) {
        switch ( role ) {
        case Qt::DisplayRole:
            return n->start().date().toString("dd-MM-yyyy");
        case Qt::EditRole:
        case KDGantt::StartTimeRole:
            return n->start();
        }
    } else if ( idx.column() == 3 ) {
        switch ( role ) {
        case Qt::DisplayRole:
            return n->end().date().toString("dd-MM-yyyy");
        case Qt::EditRole:
        case KDGantt::EndTimeRole:
            return n->end();
        }
    } else if ( idx.column() == 4 && n->completion() >= 0 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return n->completion();
        }
    } else if ( idx.column() == 5 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return n->label();
        case KDGantt::ItemTypeRole:
            return n->type();
        case KDGantt::StartTimeRole:
            return n->start();
        case KDGantt::EndTimeRole:
            return n->end();
        case KDGantt::TaskCompletionRole:
            if ( n->completion() >= 0 )
                return n->completion();
            break;
        }
    }
    return QVariant();
}

bool ProjectModel::setData( const QModelIndex& idx,  const QVariant& value,
                            int role )
{
    if ( !idx.isValid() ) return false;
    qDebug() << "ProjectModel::setData" << idx.column() << role << value;

    Node* n = static_cast<Node*>( idx.internalPointer() );
    assert( n );

    if ( idx.column() == 0 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            n->setLabel( value.toString() );
            emit dataChanged( idx, idx );
            break;
        case KDGantt::TextPositionRole:
            n->setPosition( static_cast<KDGantt::StyleOptionGanttItem::Position>(value.toInt()) );
            emit dataChanged( idx, idx );
            break;
        }
    } else if ( idx.column() == 1 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            n->setType( value.toInt() );
            emit dataChanged( idx, idx );
            break;
        }
    } else if ( idx.column() == 2 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        case KDGantt::StartTimeRole:
            n->setStart(value.toDateTime());
            emit dataChanged( idx, idx );
            break;
        }
    } else if ( idx.column() == 3 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        case KDGantt::EndTimeRole:
            n->setEnd(value.toDateTime());
            emit dataChanged( idx, idx );
            break;
        }
    } else if ( idx.column() == 4 ) {
        switch ( role ) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            n->setCompletion(value.toInt());
            emit dataChanged( idx, idx );
            break;
        }
    }
    return true;
}

bool ProjectModel::insertRows( int row, int count, const QModelIndex& parent )
{
    beginInsertRows( parent, row, row+count-1 );
    Node* p = m_root;
    if ( parent.isValid() ) p = static_cast<Node*>( parent.internalPointer() );
    assert( p );

    for ( int i = 0; i < count; ++i ) {
        Node* n = new Node;
        p->insertChild( row+i, n );
    }
    endInsertRows();
    return true;
}


Qt::ItemFlags ProjectModel::flags( const QModelIndex& idx ) const
{
    return BASE::flags( idx ) | Qt::ItemIsEditable;
}
