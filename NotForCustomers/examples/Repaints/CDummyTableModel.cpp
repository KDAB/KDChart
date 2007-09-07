#include "CDummyTableModel.h"

#include <QDebug>
#include <cmath>

CDummyTableModel::CDummyTableModel( int num, double xMin, double xMax, QObject* parent ) : QAbstractItemModel( parent ), values( 0 )
{
   /* 
    * Fill the model. It will be structered as a simple TableModel, without children.
    */
   this->fillModel( num, xMin, xMax );

}

CDummyTableModel::~CDummyTableModel( )
{
   if ( this->values )
   {
      delete[] this->values ;
   }
}

int CDummyTableModel::rowCount( const QModelIndex& parent ) const
{
   int result = 0;
   if ( parent == QModelIndex( ) )
   {
      /* A simple TableModel: only the "root"-node contains data */
      result = this->numRows;
   }
   return result;
}

int CDummyTableModel::columnCount( const QModelIndex& parent ) const
{
   int result = 0;
   if ( parent == QModelIndex( ) )
   {
      /* A simple TableModel: only the "root"-node contains data */
      result = this->numCols;
   }
   return result;
}

QModelIndex CDummyTableModel::index( int row, int column, const QModelIndex& parent ) const
{
   QModelIndex result;
   if ( parent == QModelIndex( ) )
   {
      /* A simple TableModel: only the "root"-node contains data */
      if ( row >= 0   &&   row < this->numRows   &&   column >= 0   &&   column < this->numCols )
      {
         /* The desired index has data in this model, because its row- and column-indices are within range */
         result = createIndex( row, column );
      }
   }
   return result;
}

QModelIndex CDummyTableModel::parent( const QModelIndex& ) const
{
   /* A simple TableModel: No parent-child relations */
   return QModelIndex( );
}

QVariant CDummyTableModel::data( const QModelIndex& index, int role ) const
{
   QVariant result;
   if ( index.parent( ) == QModelIndex( )  &&  role == Qt::DisplayRole )
   {
      /* A simple TableModel: only the "root"-node contains data */
      int row = index.row( );
      int col = index.column( );
      if ( row >= 0   &&   row < this->numRows   &&   col >= 0   &&   col <= this->numCols )
      {
          // Uncommment the following line to make a small gap with "empty 
          // cells" in the first two columns of the table. Provided it has the 
          // necessary rows.
//          if ( ( col == 0  &&  (row < 40 || row >50 ) )  ||  ( col == 1  &&  (row<55 || row>90 ) ) || col > 1 )
          {
              /* The desired index has data in this model, because its row- and column-indices are within range */
              result = this->values[row*this->numCols + col];
          }
      }
   }
   return result;
}

QVariant CDummyTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
   QVariant result;
   if ( orientation == Qt::Vertical )
   {
      /* The header for a row is required: This is the same as the one already provided by Qt's AbstractItemModel */
      result = QAbstractItemModel::headerData( section, orientation, role );
   }
   else
   {
      if ( section >= 0  &&  section < this->numCols )
      {
         /* Only provide a column-header if the required column exists */
         switch( role )
         {
            case Qt::DisplayRole:
               /* Provide the headertext if it is for DisplayRole */
               result = this->headertext[section];
               break;
            case Qt::TextAlignmentRole:
               /* Provide centered alignment if it is for TextAlignmentRole */
               result = Qt::AlignCenter;
               break;
            default:
               /* Default: Provide the default already provided by Qt's AbstractItemModel */
               result = QAbstractItemModel::headerData( section, orientation, role );
               break;
         }
      }
   }

   return result;
}

void CDummyTableModel::fillModel( int num, double xMin, double xMax )
{
   this->numRows = num;

   /* 6 Columns: X, X*Sin(X), 0.1*X*X, X, Cos(X), 5*Exp(-X*X) */
   this->numCols = 6;
   this->values = new double[this->numRows * this->numCols];

   /* Take num uniformly distributed steps from xMin to xMax */
   double xStep = (xMax-xMin) / ((double)num-1);
   
   /* Fill the array of values with the 4 columns */
   for ( int i=0; i<num; i++ )
   {
      double x = xMin + i*xStep;
      double v1 = x*sin(x);
      double v2 = 0.1*x*x;
      double v3 = x;
      double v4 = cos(x);
      double v5 = 5*exp(-x*x);

      this->values[i*this->numCols + 0] = x;
      this->values[i*this->numCols + 1] = v1;
      this->values[i*this->numCols + 2] = v2;
      this->values[i*this->numCols + 3] = v3;
      this->values[i*this->numCols + 4] = v4;
      this->values[i*this->numCols + 5] = v5;
   }

   this->headertext << "X" << "X*Sin(X)" << "0.1*X*X" << "X2" << "Cos(X)" << "5*Exp(-X*X)" ;
}
