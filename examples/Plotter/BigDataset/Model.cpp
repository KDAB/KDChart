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

#include "Model.h"

#include <QDebug>
#include <QTime>

#define _USE_MATH_DEFINES // this should give us the M_PI constant
#include <cmath>
// MSVC...
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const qreal s_stepWidth = 0.1;

Model::Model()
   : m_x( 0 ),
     m_function( SineFunction )
{
    m_appendTimer.setInterval( 3 );
    connect( &m_appendTimer, SIGNAL( timeout() ), SLOT( appendPoint() ) );
    // pre-fill some values
    appendPoints( 100 );
}

int Model::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent );
    return 2; // row 0: x, row 1: y
}

int Model::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent );
    return m_data.count();
}

QModelIndex Model::index( int row, int column, const QModelIndex& parent ) const
{
    if ( column > 1 || row >= m_data.count() || parent.isValid() ) {
        return QModelIndex();
    }
    return createIndex( row, column );
}

QModelIndex Model::parent( const QModelIndex& index ) const
{
    Q_UNUSED( index );
    return QModelIndex();
}

QVariant Model::data( const QModelIndex& index, int role ) const
{
    if ( role != Qt::DisplayRole || index.parent().isValid() ||
         index.column() > 1 || index.row() >= m_data.count() ) {
        return QVariant();
    }
    if ( index.column() == 0 ) {
        return index.row() * s_stepWidth;
    } else {
        return m_data.at( index.row() );
    }
}

// slot
void Model::setFunction( Function f )
{
    m_function = f;
}

void Model::setRunning( bool running )
{
    if ( running ) {
        m_appendTimer.start();
    } else {
        m_appendTimer.stop();
    }
}

// slot
void Model::appendPoint()
{
    appendPoints( 1 );
}

void Model::appendPoints( int numPoints )
{
    Q_ASSERT( numPoints >= 1);
    beginInsertRows( QModelIndex(), m_data.count(), m_data.count() + numPoints - 1 );

    for ( int i = 0; i < numPoints; i++ ) {
        m_data.append( nextFunctionValue() );
    }

    QTime stopWatch;
    stopWatch.start();
    endInsertRows(); // this immediately triggers the signals that cause the diagram to update
    qDebug() << "Adding" << numPoints << "data points to the existing" << m_data.count() - numPoints
             << "took" << stopWatch.elapsed() << "milliseconds";
}

qreal Model::nextFunctionValue()
{
    qreal fx = 0.0;

    switch ( m_function ) {
    case SineFunction:
        fx = sin( m_x );
        break;
    case TriangleFunction: {
        qreal x = fmod( m_x + 0.5 * M_PI, 2 * M_PI ); // make it look like sine, only less round
        if ( x < M_PI ) {
            fx = -1.0 + x * ( 2.0 / M_PI);
        } else {
            fx = 3.0 - x * ( 2.0 / M_PI);
        }
        break; }
    case SquareFunction: {
        qreal x = fmod( m_x, 2 * M_PI );
        fx = x < M_PI ? 1 : -1;
        break; }
    case NoiseFunction:
        fx = -1.0 + qreal( qrand() ) * 2.0 / qreal( RAND_MAX );
        break;
    case SineOneDivFunction: {
        // we want this repeating and we want negative arguments, too.
        qreal x = fmod( m_x + 10, 20 ) - 10;
        if ( qAbs( x ) < 1e-6 ) {
            break;
        }
        fx = sin( 1.0 / x );
        break; }
    case OneDivSineFunction: {
        qreal s = sin( m_x );
        fx = qAbs( s ) > 1e-6 ? 1.0 / s : 0.0;
        break; }
    default:
        Q_ASSERT( false );
    }

    m_x += s_stepWidth;
    return fx;
}
