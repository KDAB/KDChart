/****************************************************************************
 ** Copyright (C) 2007 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "tablemodel.h"

#include <iostream>
using namespace std;

#define HORIZONTAL 0
#define VERTICAL 1

TableModel::TableModel(QObject* parent)
	: QAbstractTableModel(parent)
{
	m_columns = 1;
}

TableModel::TableModel(int cols)
{
	m_columns = cols;
	m_header = new QStringList[2];
	m_header[0] = QStringList();
	m_header[1] = QStringList();
}

TableModel::~TableModel ()
{
	m_header->~QStringList();
	delete[] m_header;
}

QVariant TableModel::data ( const QModelIndex & index, int role) const
{
    if( !index.isValid() )
        return QVariant();
        
    Q_ASSERT( index.model() == this );
    Q_ASSERT( index.row() >= 0 && index.row() < rowCount() );
    Q_ASSERT( index.column() >= 0 && index.column() < columnCount() );

    if ( role == Qt::DisplayRole || role == Qt::EditRole )
        return m_table[ index.row() ] [index.column() ];
    return QVariant();
}

bool
TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

		if(index.row() < rowCount() && index.column() < columnCount() && role == Qt::EditRole)
		{
				m_table[index.row()][index.column()] = value;
        emit dataChanged( index, index );
        return true;
    } else {
				qWarning("TableModel::setData(): wrong index!");
        return false;
    }
}

void
TableModel::appendData(const QVector< QVariant > &column)
{
		m_table.resize(rowCount()+1);
 		m_table[rowCount()-1] = column;
}

int TableModel::rowCount ( const QModelIndex & ) const
{
	if(m_table.isEmpty())
		return 0;
	return m_table.size();
}

int TableModel::columnCount ( const QModelIndex & ) const
{
	return m_columns;
}

/*
 *	header
 */

QVariant
TableModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	if(role == Qt::DisplayRole)
	{
		if(orientation == Qt::Horizontal && m_header[HORIZONTAL].size() > 0) {
				return m_header[HORIZONTAL][section];
		} else if(orientation == Qt::Vertical && m_header[VERTICAL].size() > 0) {
				return m_header[VERTICAL][section];
		}
	}
	return QVariant();
}

bool
TableModel::addHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	if(role == Qt::EditRole){
		if(orientation == Qt::Horizontal) {
			m_header[HORIZONTAL] << value.toString();
			emit headerDataChanged(orientation, section, section);
			return true;
		} else if(orientation == Qt::Vertical) {
			m_header[VERTICAL] << value.toString();
			emit headerDataChanged(orientation, section, section);
			return true;
		}
	}
	return false;
}

QVector< QVariant >
TableModel::getRow()
{
	return QVector< QVariant > (columnCount());
}

QModelIndex
TableModel::getIndex(int row, int col)
{
 	return createIndex(row, col);
}

