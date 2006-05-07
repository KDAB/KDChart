#ifndef ALTERNATINGCOLUMNSPROXYMODEL_H
#define ALTERNATINGCOLUMNSPROXYMODEL_H

#include <QAbstractProxyModel>

class AlternatingColumnsProxyModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    AlternatingColumnsProxyModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
};

#endif
