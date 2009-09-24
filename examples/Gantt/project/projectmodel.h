#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>

class ProjectModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit ProjectModel( QObject* parent = 0 );
    virtual ~ProjectModel();

    /*reimp*/ int rowCount( const QModelIndex& idx ) const;
    /*reimp*/ int columnCount( const QModelIndex& idx ) const;

    /*reimp*/ QModelIndex index( int row, int col, const QModelIndex& parent = QModelIndex() ) const;
    /*reimp*/ QModelIndex parent( const QModelIndex& idx ) const;

    /*reimp*/QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    /*reimp*/ QVariant data( const QModelIndex& idx, int role = Qt::DisplayRole ) const;
    /*reimp*/ bool setData( const QModelIndex& idx,  const QVariant& value,
                            int role = Qt::DisplayRole );

    /*reimp*/ bool insertRows( int row, int count, const QModelIndex& parent = QModelIndex() );

    /*reimp*/ Qt::ItemFlags flags( const QModelIndex& ) const;

    bool load( const QString& filename );
    bool save( const QString& filename );

private:
    class Node;

    Node* m_root;
};

#endif /* PROJECTMODEL_H */

