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

#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QVector>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

#include <QtCore/QDebug>

class TableModel : public QAbstractTableModel
{
	Q_OBJECT;
public:
	TableModel(QObject *parent = 0);
	TableModel(int cols);
  ~TableModel ();

	// data
  QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	// append a row to the list
	void appendData(const QVector< QVariant > &column);

  int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
  int columnCount ( const QModelIndex & parent = QModelIndex() ) const;

	// header
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	bool addHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

	// returns a valid row of the table
	QVector< QVariant > getRow();
	QModelIndex getIndex(int row, int col);

protected:

	QVector< QVector<QVariant> > m_table;
	QStringList* m_header;
	int m_columns;

};

#endif
