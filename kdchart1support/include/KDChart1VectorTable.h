/* -*- Mode: C++ -*-
   KDChart1 - a multi-platform charting engine
*/

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart1 library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#ifndef __KDCHART1VECTORTABLE_H__
#define __KDCHART1VECTORTABLE_H__

#include <QVector>
//#include <qtable.h>

#include <KDChart1DataIntern.h>
#include <KDChart1TableBase.h>

class KDChart1Shared
{
public:
    KDChart1Shared() : count(1) {}
    void ref() { ++count; }
    bool deref() { return !--count; }
    uint count;
};

class KDCHART1_EXPORT KDChart1VectorTablePrivate : public KDChart1Shared
{
public:

    KDChart1VectorTablePrivate() : KDChart1Shared() {
        row_count = 0;
        col_count = 0;
    }

    KDChart1VectorTablePrivate( uint _rows, uint _cols ) : KDChart1Shared() {
        matrix.resize( _rows * _cols );
        col_count = _cols;
        row_count = _rows;
    }

    KDChart1VectorTablePrivate( const KDChart1VectorTablePrivate& _t ) :
        KDChart1Shared(),
        matrix( _t.matrix ),
        col_count( _t.col_count ),
        row_count( _t.row_count ) {}

    ~KDChart1VectorTablePrivate() {}

    void expand( uint _rows, uint _cols ) {
        // Save the old table
        QVector<KDChart1Data> save( matrix );

        // Delete old data, then resize
        matrix.resize( 0 );
        matrix.resize( _rows * _cols );

        // Copy over the old data
        for( uint row = 0; row < qMin( row_count, _rows ); row++ )
            for( uint col = 0; col < qMin( col_count, _cols ); col++ )
                matrix[ row * _cols + col ].setAll( save[ row * col_count + col ] );

        // set the new counts
        col_count = _cols;
        row_count = _rows;
    }

    KDChart1Data& cell( uint _row, uint _col ) {
        Q_ASSERT( _row < row_count  );
        Q_ASSERT( _col < col_count );
        return matrix[ static_cast < int > ( _row * col_count + _col ) ];
    }
    const KDChart1Data& cell( uint _row, uint _col ) const {
        Q_ASSERT( _row < row_count  );
        Q_ASSERT( _col < col_count );
        return matrix[ static_cast < int > ( _row * col_count + _col ) ];
    }
    void setCell( uint _row, uint _col, const KDChart1Data& _element ) {
        Q_ASSERT( _row < row_count  );
        Q_ASSERT( _col < col_count );
        matrix[ static_cast < int > ( _row * col_count + _col ) ].setAll( _element );
    }

    void clearCell( uint _row, uint _col ) {
        Q_ASSERT( _row < row_count  );
        Q_ASSERT( _col < col_count );
        matrix[ static_cast < int > ( _row * col_count + _col ) ].clearValue();
    }

    void clearAllCells() {
        for ( uint r = 0; r < row_count; ++r )
            for ( uint c = 0; c < col_count; ++c )
                matrix[ r * col_count + c ].clearValue();
    }

    QVector<KDChart1Data> matrix;

    uint col_count;
    uint row_count;
};


class KDCHART1_EXPORT KDChart1VectorTableData : public KDChart1TableDataBase
{
    Q_OBJECT

private:
    typedef KDChart1VectorTablePrivate Priv;
    uint _usedRows, _usedCols;

public:
    /**
     * Typedefs
     */
    typedef QVector<KDChart1Data>::iterator Iterator;
    typedef QVector<KDChart1Data>::const_iterator ConstIterator;

    typedef QVector<int>::iterator RowIterator;
    typedef QVector<int>::const_iterator ConstRowIterator;

    typedef QVector<int>::iterator ColIterator;
    typedef QVector<int>::const_iterator ConstColIterator;

    /**
     * API
     */
    KDChart1VectorTableData() :
        KDChart1TableDataBase()
    {
        sh = new Priv;
        _usedCols = 0;
        _usedRows = 0;
    }
    KDChart1VectorTableData( uint _rows, uint _cols ) :
        KDChart1TableDataBase()
    {
        sh = new Priv( _rows, _cols );
        _usedRows = _rows;
        _usedCols = _cols;
    }

    KDChart1VectorTableData( const KDChart1VectorTableData& _t ) :
        KDChart1TableDataBase( _t ) {
        _useUsedRows = _t._useUsedRows;
        _useUsedCols = _t._useUsedCols;
        _usedRows = _t._usedRows;
        _usedCols = _t._usedCols;
        sh = _t.sh;
        sh->ref();
        setSorted( _t.sorted() );
    }

    virtual ~KDChart1VectorTableData() {
        if ( sh->deref() )
            delete sh;
    }

    KDChart1VectorTableData& operator=( const KDChart1VectorTableData& t ) {
        if ( &t == this )
            return * this;
        _useUsedRows = t._useUsedRows;
        _useUsedCols = t._useUsedCols;
        _usedRows = t._usedRows;
        _usedCols = t._usedCols;
        t.sh->ref();
        if ( sh->deref() )
            delete sh;
        sh = t.sh;
        setSorted( t.sorted() );
        return *this;
    }

public slots:
    Iterator begin() {
        return sh->matrix.begin();
    }

    ConstIterator begin() const {
        return sh->matrix.begin();
    }

    Iterator end() {
        return sh->matrix.end();
    }

    ConstIterator end() const {
        return sh->matrix.end();
    }

    bool isEmpty() const {
        return ( sh->col_count == 0 && sh->row_count == 0 );
    }

    uint cols() const {
        return sh->col_count;
    }

    uint rows() const {
        return sh->row_count;
    }
/*
    KDChart1Data& cell( uint _row, uint _col ) {
        detach();
        return sh->cell( _row, _col );
    }
*/
    virtual bool cellCoord( uint _row, uint _col,
                            QVariant& _value,
                            int coordinate=1 ) const
    {
        if( _row >= sh->row_count || _col >= sh->col_count )
            return false;
        _value = sh->cell( _row, _col ).value( coordinate );
        return true;
    }

    virtual bool cellProp( uint _row, uint _col,
                           int& _prop ) const
    {
        if( _row >= sh->row_count || _col >= sh->col_count )
            return false;
        _prop = sh->cell( _row, _col ).propertySet();
        return true;
    }

    virtual void setCell( uint _row, uint _col,
                          const QVariant& _value1,
                          const QVariant& _value2=QVariant() ) {
        detach();
        const KDChart1Data element( _value1, _value2 );
        sh->setCell( _row, _col, element );
    }

    virtual void setProp( uint _row, uint _col,
                          int _propSet=0 )
    {
        sh->cell( _row, _col ).setPropertySet( _propSet );
    }
    
    void clearCell( uint _row, uint _col ) {
        detach();
        sh->clearCell( _row, _col );
    }

    void clearAllCells() {
        detach();
        sh->clearAllCells();
    }

    void expand( uint _rows, uint _cols ) {
        detach();
        sh->expand( _rows, _cols );
        // adjust the usedRows / usedCols, if they had been set before
        if( _useUsedCols )
            setUsedCols( qMin( _usedCols, _cols ) );
        if( _useUsedRows )
            setUsedRows( qMin( _usedRows, _rows ) );
    }

    void setUsedRows( uint _rows ) {
        Q_ASSERT( _rows <= rows() );
        if( _usedRows < _rows )
            setSorted( false );
        _usedRows = _rows;
        _useUsedRows = true;
    }

    uint usedRows() const {
        return _useUsedRows ? _usedRows : rows();
    }

    void setUsedCols( uint _cols ) {
        Q_ASSERT( _cols <= cols() );
        if( _usedCols < _cols )
            setSorted( false );
        _usedCols = _cols;
        _useUsedCols = true;
    }

    uint usedCols() const {
        return _useUsedCols ? _usedCols : cols();
    }

private:
    /**
     * Helpers
     */
    void detach() {
        if ( sh->count > 1 ) {
            sh->deref();
            sh = new Priv( *sh );
        }
        setSorted( false );
    }

    /**
     * Variables
     */
    Priv* sh;
};

#endif
// __KDCHART1LISTTABLE_H__

