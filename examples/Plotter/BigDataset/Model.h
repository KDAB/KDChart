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

#ifndef MODEL_H
#define MODEL_H

#include <QAbstractItemModel>
#include <QTimer>
#include <QVector>

class Model : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Function {
        SineFunction = 0,
        TriangleFunction,
        SquareFunction,
        NoiseFunction,
        OneDivSineFunction,
        SineOneDivFunction
    };
    Model();

    int columnCount( const QModelIndex& parent ) const;
    int rowCount( const QModelIndex& parent ) const;

    QModelIndex index( int row, int column, const QModelIndex& parent ) const;
    QModelIndex parent( const QModelIndex& index ) const;

    QVariant data( const QModelIndex& index, int role ) const;

    void setFunction( Function f );

    void appendPoints( int numPoints );

public slots:
    void setRunning( bool );

private slots:
    void appendPoint();

private:
    qreal nextFunctionValue();

    qreal m_x;
    Function m_function;
    QVector< qreal > m_data;
    QTimer m_appendTimer;
};

#endif
