#include <QtDebug>
#include <qglobal.h>

#include "DynamicTableModel.h"
#include "TableModel.h"

DynamicTableModel::DynamicTableModel ( TableModel * parent )
     : TableModel ( parent )
{
	columns = 4;
	reset();
}

DynamicTableModel::DynamicTableModel ( int _columns )
{
	columns = _columns;
	reset();
}

void
DynamicTableModel::appendData ( QVariant value, int row, int col )
{

		if(!value.isValid())
			qFatal("value invalid!");

		QModelIndex index = createIndex(row, col);
		if(!index.isValid())
			qFatal("index not valid");

		if(col == 0) // =^ if(m_rows[row].isEmpty())
		{
			m_rows.resize(m_rows.size() + 1);
 			m_rows[row+1] = QVector<QVariant>(columns);
		}

		setData(index, value, (int) Qt::EditRole);

}

void
DynamicTableModel::reset()
{
	m_rows.resize(1);
	m_rows[0] = QVector<QVariant>(columns);
}
