/*!
 *	see examples/tools/TableModel.h
 */

#ifndef DYNAMICTABLEMODEL_H
#define DYNAMICTABLEMODEL_H

#include "TableModel.h"

#define TEST_VALUE 4.123456789

class DynamicTableModel : public TableModel
{
    Q_OBJECT
public:
		DynamicTableModel ( TableModel * parent = 0 );
		DynamicTableModel ( int _columns );
		void appendData ( QVariant value, int row, int col );
		void reset();

private:
		int columns;

};

#endif
