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
#include <math.h>
#include <limits.h>
//#include <qtable.h>

#include <KDChart1TableBase.h>


/**
   \class KDChart1TableDataBase KDChart1TableBase.h
   \brief Encapsulates all data values that are to be used in a chart.

   \note To create your data table you would <em>not</em> use a
   \c KDChart1TableDataBase but instantiate the class \c KDChart1TableData.
   The \c KDChart1TableData class is an auxiliary class: depending on your
   Qt version it will be mapped onto a \c KDChart1VectorTableData or onto
   a \c KDChart1ListTableData both of which are derived from
   \c KDChart1TableDataBase and implement all of its functions.
   Thus you would create a table of 3 datasets with 25 cells each like this:
   \verbatim

   KDChart1TableData myData( 3, 25 );

   \endverbatim

   Data values may be specified via \c setCell().
   Cell specific properties may be specified via \c setCellProp().

   You may adjust or modify your table like this:

   \li Entering the data can be done either manually using \c setCell()
   or by passing a QTable to the \c importFromQTable() function.

   \li Performance of KD Chart can be increased by specifying the number
   of rows and or the number of columns actually used: \c setUsedRows()
   and/or \c setUsedCols() prevents KD Chart from iterating over thousands
   of empty rows/cols that might follow your data cells in case your
   table is much bigger than needed.

   \li In case you want to increase your table's size without using the
   data stored in it please call the \c expand() function with the new
   total number of rows and cells.

   \li Accessing one data cell is possible via \c cellCoord()
   and via \c cellProp(),
   e.g. you might assign a special property set ID to all cells with a
   future absicssa axis value:
   \verbatim

   const QDateTime currentTime( QDateTime::currentDateTime() );
   for( int iCell = 0;  iCell < usedValues;  ++iCell ){
   KDChart1Data& cell = myData.cell( 0, iCell );
   // assign special property set ID if X value is in the future
   if( cell.isDateTime( 2 ) && cell.dateTimeValue( 2 ) > currentTime )
   cell.setPropertySet( idProp_FutureValues );
   }

   \endverbatim

   \note All of the other functions provided by KDChart1TableDataBase are
   either used internally by KD Chart or they are const methods
   returning some usefull figures like the sum of all values in a row...
*/


void KDChart1TableDataBase::setUsedRows( uint _rows ) {
    _usedRows = _rows;
    _useUsedRows = true;
}
uint KDChart1TableDataBase::usedRows() const {
    return _useUsedRows ? _usedRows : rows();
}
void KDChart1TableDataBase::setUsedCols( uint _cols ) {
    _usedCols = _cols;
    _useUsedCols = true;
}
uint KDChart1TableDataBase::usedCols() const {
    return _useUsedCols ? _usedCols : cols();
}


bool KDChart1TableDataBase::cellsHaveSeveralCoordinates(
    QVariant::Type* type2Ref ) const
{
    return cellsHaveSeveralCoordinates( 0, UINT_MAX, type2Ref );
}


bool KDChart1TableDataBase::cellsHaveSeveralCoordinates(
    uint row1,
    uint row2,
    QVariant::Type* type2Ref ) const
{
    // return true if all wanted datasets have at least two coordinates
    // stored in all of their cells - BUT only if these coordinates are
    // of equal type for each of the cells
    // note: We skip cells that are empty, this means having
    //       set neither coordinate #1 nor coordinate #2.
    bool severalCoordinates = row1 < usedRows();
    if( severalCoordinates ) {
        severalCoordinates = false;
        QVariant::Type testType = QVariant::Invalid;
        const uint r2 = (UINT_MAX == row2)
                  ? usedRows()
                  : qMin( row2+1, usedRows() );
        QVariant value1;
        QVariant value2;
        for ( uint row = row1; row < r2; ++row ){
            for ( uint col = 0; col < usedCols(); ++col ){
                if( cellCoords( row, col, value1, value2 ) ){
                    if( QVariant::Invalid != value2.type() ){
                        if( (QVariant::Invalid != testType) &&
                            (value2.type()     != testType) ){
                            severalCoordinates = false;
                            break;
                        }else{
                            testType = value2.type();
                            if( NULL != type2Ref )
                                *type2Ref = testType;
                            severalCoordinates = true;
                        }
                    }else if( QVariant::Invalid != value1.type() ){
                        severalCoordinates = false;
                        break;
                    }
                }
            }
        }
    }
    return severalCoordinates;
}


QVariant::Type KDChart1TableDataBase::cellsValueType(
    uint row1,
    uint row2,
    int coordinate ) const
{
    QVariant::Type res = QVariant::Invalid;
    const uint r2 = (UINT_MAX == row2)
              ? usedRows()
              : qMin( row2+1, usedRows() );

    QVariant value;
    for ( uint row = row1; row < r2; ++row )
        for ( uint col = 0; col < usedCols(); ++col )
            if( cellCoord( row, col, value, coordinate ) )
                if( QVariant::Invalid != value.type() )
                    res = value.type();
    return res;
}


QVariant::Type KDChart1TableDataBase::cellsValueType(
    int coordinate ) const
{
    return cellsValueType( 0, UINT_MAX, coordinate );
}


double KDChart1TableDataBase::maxValue( int coordinate ) const
{
    double maxValue = 0.0;
    bool bStart = true;
    QVariant value;
    double dVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        for ( uint col = 0; col < usedCols(); col++ ) {
            if( cellCoord( row, col, value, coordinate ) &&
                QVariant::Double == value.type() ) {
                dVal = value.toDouble();
                if( isNormalDouble( dVal ) ){
                    if ( bStart ) {
                        maxValue = dVal;
                        bStart = false;
                    } else
                        maxValue = qMax( maxValue, dVal );
                }
            }
        }
    }
    return maxValue;
}



double KDChart1TableDataBase::minValue( int coordinate, bool bOnlyGTZero ) const
{
    double minValue = 0.0;
    bool bStart = true;
    QVariant value;
    double dVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        for ( uint col = 0; col < usedCols(); col++ ) {
            if( cellCoord( row, col, value, coordinate ) &&
                QVariant::Double == value.type() ) {
                dVal = value.toDouble();
                if( !bOnlyGTZero || 0.0 < dVal ){
                    if ( bStart ) {
                        minValue = dVal;
                        bStart = false;
                    }else{
                        minValue = qMin( minValue, dVal );
                    }
                }
            }
        }
    }
    return minValue;
}


QDateTime KDChart1TableDataBase::maxDtValue( int coordinate ) const
{
    QDateTime maxValue = QDateTime( QDate(1970,1,1) );
    bool bStart = true;
    QVariant value;
    QDateTime dtVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        for ( uint col = 0; col < usedCols(); col++ ) {
            if( cellCoord( row, col, value, coordinate ) &&
                QVariant::DateTime == value.type() ) {
                dtVal = value.toDateTime();
                if ( bStart ) {
                    maxValue = dtVal;
                    bStart = false;
                } else
                    maxValue = qMax(maxValue, dtVal);
            }
        }
    }
    return maxValue;
}



QDateTime KDChart1TableDataBase::minDtValue( int coordinate ) const
{
    QDateTime minValue = QDateTime( QDate(1970,1,1) );
    bool bStart = true;
    QVariant value;
    QDateTime dtVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        for ( uint col = 0; col < usedCols(); col++ ) {
            if( cellCoord( row, col, value, coordinate ) &&
                QVariant::DateTime == value.type() ) {
                dtVal = value.toDateTime();
                if ( bStart ) {
                    minValue = dtVal;
                    bStart = false;
                } else
                    minValue = qMin(minValue, dtVal);
            }
        }
    }
    return minValue;
}


double KDChart1TableDataBase::maxColSum( int coordinate ) const
{
    double maxValue = 0.0;
    bool bStart = true;
    for ( uint col = 0; col < usedCols(); col++ ) {
        double colValue = colSum( col, coordinate );
        if ( bStart ) {
            maxValue = colValue;
            bStart = false;
        } else
            maxValue = qMax( maxValue, colValue );
    }
    return maxValue;
}


double KDChart1TableDataBase::minColSum( int coordinate ) const
{
    double minValue = 0.0;
    bool bStart = true;
    for ( uint col = 0; col < usedCols(); col++ ) {
        double colValue = colSum( col, coordinate );
        if ( bStart ) {
            minValue = colValue;
            bStart = false;
        } else
            minValue = qMin( minValue, colValue );
    }

    return minValue;
}


double KDChart1TableDataBase::maxColSum( uint row, uint row2, int coordinate ) const
{
    double maxValue = 0;
    bool bStart = true;
    if ( 0 < usedRows() ) {
        uint a = row;
        uint z = row2;
        if ( usedRows() <= a )
            a = usedRows() - 1;
        if ( usedRows() <= z )
            z = usedRows() - 1;
        for ( uint col = 0; col < usedCols(); col++ ) {
            double valueValue = 0.0;
            QVariant value;
            double dVal;
            for ( uint row = a; row <= z; row++ ) {
                if( cellCoord( row, col, value, coordinate ) &&
                    QVariant::Double == value.type() ) {
                    dVal = value.toDouble();
                    if( isNormalDouble( dVal ) )
                        valueValue += dVal;
                }
            }
            if ( bStart ) {
                maxValue = valueValue;
                bStart = false;
            } else
                maxValue = qMax( maxValue, valueValue );
        }
    }
    return maxValue;
}


double KDChart1TableDataBase::minColSum( uint row, uint row2, int coordinate ) const
{
    double minValue = 0;
    bool bStart = true;
    if ( 0 < usedRows() ) {
        uint a = row;
        uint z = row2;
        if ( usedRows() <= a )
            a = usedRows() - 1;
        if ( usedRows() <= z )
            z = usedRows() - 1;
        for ( uint col = 0; col < usedCols(); col++ ) {
            double valueValue = 0.0;
            QVariant value;
            double dVal;
            for ( uint row = a; row <= z; row++ ) {
                if( cellCoord( row, col, value, coordinate ) &&
                    QVariant::Double == value.type() ) {
                    dVal = value.toDouble();
                    if( isNormalDouble( dVal ) )
                        valueValue += dVal;
                }
            }
            if ( bStart ) {
                minValue = valueValue;
                bStart = false;
            } else
                minValue = qMin( minValue, valueValue );
        }
    }
    return minValue;
}


double KDChart1TableDataBase::colSum( uint col, int coordinate ) const
{
    double sum = 0.0;
    QVariant value;
    double dVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        if( cellCoord( row, col, value, coordinate ) &&
            QVariant::Double == value.type() ) {
            dVal = value.toDouble();
            if( isNormalDouble( dVal ) )
                sum += dVal;
        }
    }

    return sum;
}


double KDChart1TableDataBase::colAbsSum( uint col, int coordinate ) const
{
    double sum = 0.0;
    QVariant value;
    double dVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        if( cellCoord( row, col, value, coordinate ) &&
            QVariant::Double == value.type() ) {
            dVal = value.toDouble();
            if( isNormalDouble( dVal ) )
                sum += fabs( dVal );
        }
    }

    return sum;
}


double KDChart1TableDataBase::maxRowSum( int coordinate ) const
{
    double maxValue = 0.0;
    bool bStart = true;
    for ( uint row = 0; row < usedRows(); row++ ) {
        double rowValue = rowSum( row, coordinate );
        if ( bStart ) {
            maxValue = rowValue;
            bStart = false;
        } else
            maxValue = qMax( maxValue, rowValue );
    }
    return maxValue;
}


double KDChart1TableDataBase::minRowSum( int coordinate ) const
{
    double minValue = 0.0;
    bool bStart = true;
    for ( uint row = 0; row < usedRows(); row++ ) {
        double rowValue = rowSum( row, coordinate );
        if ( bStart ) {
            minValue = rowValue;
            bStart = false;
        } else
            minValue = qMin( minValue, rowValue );
    }

    return minValue;
}


double KDChart1TableDataBase::rowSum( uint row, int coordinate ) const
{
    double sum = 0.0;
    QVariant value;
    double dVal;
    for ( uint col = 0; col < usedCols(); col++ ) {
        if( cellCoord( row, col, value, coordinate ) &&
            QVariant::Double == value.type() ) {
            dVal = value.toDouble();
            if( isNormalDouble( dVal ) )
                sum += dVal;
        }
    }
    return sum;
}


double KDChart1TableDataBase::rowAbsSum( uint row, int coordinate ) const
{
    double sum = 0.0;
    QVariant value;
    double dVal;
    for ( uint col = 0; col < usedCols(); col++ ) {
        if( cellCoord( row, col, value, coordinate ) &&
            QVariant::Double == value.type() ) {
            dVal = value.toDouble();
            if( isNormalDouble( dVal ) )
                sum += fabs( dVal );
        }
    }
    return sum;
}


double KDChart1TableDataBase::maxInColumn( uint col, int coordinate ) const
{
    double maxValue = 0.0;
    bool bStart = true;
    QVariant value;
    double dVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        if( cellCoord( row, col, value, coordinate ) &&
            QVariant::Double == value.type() ) {
            dVal = value.toDouble();
            if( isNormalDouble( dVal ) ){
                if ( bStart ) {
                    maxValue = dVal;
                    bStart = false;
                } else
                    maxValue = qMax( maxValue, dVal );
            }
        }
    }

    return maxValue;
}


double KDChart1TableDataBase::minInColumn( uint col, int coordinate ) const
{
    double minValue = 0.0;
    bool bStart = true;
    QVariant value;
    double dVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        if( cellCoord( row, col, value, coordinate ) &&
            QVariant::Double == value.type() ) {
            dVal = value.toDouble();
            if( isNormalDouble( dVal ) ){
                if ( bStart ) {
                    minValue = dVal;
                    bStart = false;
                } else
                    minValue = qMin( minValue, dVal );
            }
        }
    }

    return minValue;
}


double KDChart1TableDataBase::maxInRow( uint row, int coordinate ) const
{
    double maxValue = DBL_MIN;
    bool bStart = true;
    QVariant value;
    double dVal;
    if ( UINT_MAX > row ) {
        for ( uint col = 0; col < usedCols(); col++ ) {
            if( cellCoord( row, col, value, coordinate ) &&
                QVariant::Double == value.type() ) {
                dVal = value.toDouble();
                if( isNormalDouble( dVal ) ){
                    if ( bStart ) {
                        maxValue = dVal;
                        bStart = false;
                    } else
                        maxValue = qMax( maxValue, dVal );
                }
            }
        }
    }
    return maxValue;
}


double KDChart1TableDataBase::minInRow( uint row, int coordinate ) const
{
    double minValue = DBL_MAX;
    bool bStart = true;
    QVariant value;
    double dVal;
    if ( UINT_MAX > row ) {
        for ( uint col = 0; col < usedCols(); col++ ) {
            if( cellCoord( row, col, value, coordinate ) &&
                QVariant::Double == value.type() ) {
                dVal = value.toDouble();
                if( isNormalDouble( dVal ) ){
                    if ( bStart ) {
                        minValue = dVal;
                        bStart = false;
                    } else
                        minValue = qMin( minValue, dVal );
                }
            }
        }
    }
    return minValue;
}


double KDChart1TableDataBase::maxInRows( uint row, uint row2, int coordinate ) const
{
    double maxValue = 0.0;
    bool bStart = true;
    if ( 0 < usedRows() ) {
        uint a = row;
        uint z = row2;
        // qDebug("KDChart1TableDataBase::maxInRows()   (1)     a: %u     z: %u", a, z);
        if ( usedRows() <= a )
            a = usedRows() - 1;
        if ( usedRows() <= z )
            z = usedRows() - 1;
        // qDebug("KDChart1TableDataBase::maxInRows()   (2)     a: %u     z: %u", a, z);
        for ( uint row = a; row <= z; ++row ) {
            QVariant value;
            double dVal;
            for ( uint col = 0; col < usedCols(); ++col ) {
                if( cellCoord( row, col, value, coordinate ) &&
                    QVariant::Double == value.type() ) {
                    dVal = value.toDouble();
                    if( isNormalDouble( dVal ) ){
                        if ( bStart ) {
                            maxValue = dVal;
                            bStart = false;
                        } else
                            maxValue = qMax( maxValue, dVal );
                    }
                }
            }
        }
    }
    return maxValue;
}


double KDChart1TableDataBase::minInRows( uint row, uint row2, int coordinate, bool bOnlyGTZero ) const
{
    double minValue = 0.0;
    bool bStart = true;
    if ( 0 < usedRows() ) {
        uint a = row;
        uint z = row2;
        // qDebug("KDChart1TableDataBase::minInRows()   (1)     a: %u     z: %u", a, z);
        if ( usedRows() <= a )
            a = usedRows() - 1;
        if ( usedRows() <= z )
            z = usedRows() - 1;
        //qDebug("KDChart1TableDataBase::minInRows()   (2)     a: %u     z: %u", a, z);
        for ( uint row = a; row <= z; ++row ) {
            QVariant value;
            double dVal;
            for ( uint col = 0; col < usedCols(); ++col ) {
                if( cellCoord( row, col, value, coordinate ) &&
                    QVariant::Double == value.type() ) {
                    dVal = value.toDouble();
                    if( isNormalDouble( dVal ) ){
                        if( !bOnlyGTZero || 0.0 < dVal ){
                            if ( bStart ) {
                                minValue = dVal;
                                bStart = false;
                            }else{
                                minValue = qMin( minValue, dVal );
                            }
                        }
                    }
                }
            }
        }
    }
    return minValue;
}


QDateTime KDChart1TableDataBase::maxDtInRows( uint row, uint row2,
                                             int coordinate ) const
{
    QDateTime maxValue = QDateTime( QDate(1970,1,1) );
    bool bStart = true;
    if ( 0 < usedRows() ) {
        uint a = row;
        uint z = row2;
        if ( usedRows() <= a )
            a = usedRows() - 1;
        if ( usedRows() <= z )
            z = usedRows() - 1;
        for ( uint row = a; row <= z; ++row ) {
            QVariant value;
            QDateTime dtVal;
            for ( uint col = 0; col < usedCols(); ++col ) {
                if( cellCoord( row, col, value, coordinate ) &&
                    QVariant::DateTime == value.type() ) {
                    dtVal = value.toDateTime();
                    if ( bStart ) {
                        maxValue = dtVal;
                        bStart = false;
                    } else
                        maxValue = qMax( maxValue, dtVal );
                }
            }
        }
    }
    return maxValue;
}


QDateTime KDChart1TableDataBase::minDtInRows( uint row, uint row2,
                                             int coordinate ) const
{
    QDateTime minValue = QDateTime( QDate(1970,1,1) );
    bool bStart = true;
    if ( 0 < usedRows() ) {
        uint a = row;
        uint z = row2;
        if ( usedRows() <= a )
            a = usedRows() - 1;
        if ( usedRows() <= z )
            z = usedRows() - 1;
        for ( uint row = a; row <= z; ++row ) {
            QVariant value;
            QDateTime dtVal;
            for ( uint col = 0; col < usedCols(); ++col ) {
                if( cellCoord( row, col, value, coordinate ) &&
                    QVariant::DateTime == value.type() ) {
                    dtVal = value.toDateTime();
                    if ( bStart ) {
                        minValue = dtVal;
                        bStart = false;
                    } else
                        minValue = qMin( minValue, dtVal );
                }
            }
        }
    }
    return minValue;
}


uint KDChart1TableDataBase::lastPositiveCellInColumn( uint col, int coordinate ) const
{
    uint ret = UINT_MAX;
    QVariant value;
    double dVal;
    for ( uint row = 0; row < usedRows(); row++ ) {
        if( cellCoord( row, col, value, coordinate ) &&
            QVariant::Double == value.type() ) {
            dVal = value.toDouble();
            if( isNormalDouble( dVal ) && 0 < dVal )
                ret = row;
        }
    }
    return ret;
}

/*
void KDChart1TableDataBase::importFromQTable( QTable* table )
{
    if( table->numRows() > (int)rows() ||
        table->numCols() > (int)cols() )
        expand( table->numRows(), table->numCols() );
    setUsedRows( table->numRows() );
    setUsedCols( table->numCols() );
    for( int row = 0; row < table->numRows(); row++ )
        for( int col = 0; col < table->numCols(); col++ ) {
            QString cellContents = table->text( row, col );
            if( !cellContents.isEmpty() ) {
                // First try to parse a double
                bool ok = false;
                double value = cellContents.toDouble( &ok );
                if( ok ) {
                    // there was a double
                    setCell( row, col, value );
                } else {
                    // no double, but at least a string
                    setCell( row, col, cellContents );
                }
            } // don't do anything if no contents
        }
    setSorted( false );
}
*/

void KDChart1TableDataBase::setSorted(bool sorted)
{
    _sorted = sorted;
}
bool KDChart1TableDataBase::sorted() const
{
    return _sorted;
}
