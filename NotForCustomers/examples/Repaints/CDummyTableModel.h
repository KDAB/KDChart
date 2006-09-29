#ifndef CDUMMYTABLEMODEL_H
#define CDUMMYTABLEMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

/*
 * The CDummyTableModel is a simple subclass from QAbstractItemModel.
 * It contains a 2D matrix of doubles, and has its data structured in a table.
 * Currently, it provides 4 columns: X, X*Sin(X), 0.1*X*X and X.
 * The amount of rows and the ranges can be set by editing config/dummymodel.cfg.
 */

class CDummyTableModel : public QAbstractItemModel
{
      Q_OBJECT

   public:
      CDummyTableModel( int num, double xMin, double xMax, QObject* parent = 0 );
      ~CDummyTableModel( );

      /* Pure methods from QAbstractItemModel */
      int columnCount( const QModelIndex& parent = QModelIndex( ) ) const;
      int rowCount( const QModelIndex& parent = QModelIndex( ) ) const;

      QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex( ) ) const;                       
      QModelIndex parent( const QModelIndex& index = QModelIndex( ) ) const;

      QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

      QVariant headerData( int section, Qt::Orientation, int role = Qt::DisplayRole ) const;


   private:
      void fillModel( int num, double xMin, double xMax );

   private:
      int numRows;
      int numCols;
      double* values;
      QStringList headertext;

};
#endif /* CDUMMYTABLEMODEL_H */
