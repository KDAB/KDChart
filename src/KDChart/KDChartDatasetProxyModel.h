/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTDATASETPROXYMODEL_H
#define KDCHARTDATASETPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QVector>

#include "kdchart_export.h"

namespace KDChart {

class IndexOutOfBoundsException;

typedef QVector<int> DatasetDescriptionVector;

/** DatasetProxyModel takes a KDChart dataset configuration and translates
        it into a filtering proxy model.

        The resulting model will only contain the part of the model that is
        selected by the dataset, and the according row and column header
        data.

        Currently, this model is implemented for table models only. The way it
        would work with models representing a tree is to be decided.

        The column selection is configured by passing a dataset description
        vector to the model. This vector (of integers) is supposed to have one
        value for each column of the original model. If the value at position
        x is -1, column x of the original model is not included in the
        dataset. If it is between 0 and (columnCount() -1), it is the column
        the source column is mapped to in the resulting model. Any other value
        is an error.
    */
class KDCHART_EXPORT DatasetProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /** Create a DatasetProxyModel.
            Without further configuration, this model is invalid.
            @see setDatasetDescriptionVector
        */
    explicit DatasetProxyModel(QObject *parent = nullptr);

    QModelIndex buddy(const QModelIndex &index) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    /** Implements the mapping from the source to the proxy indexes. */
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    /** Implements the mapping from the proxy to the source indexes. */
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    /** Overloaded from base class. */
    QVariant data(const QModelIndex &index, int role) const override;

    /** Overloaded from base class. */
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    /** Overloaded from base class. */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /** Overloaded from base class. */
    void setSourceModel(QAbstractItemModel *sourceModel) override;

    /** Set the root index of the table in
            the source model */
    void setSourceRootIndex(const QModelIndex &rootIdx);

public Q_SLOTS:
    /** Reset all dataset description.
            After that, the result of the proxying is an empty model (a new
            dataset description needs to be set to achieve a non-empty result).
        */
    void resetDatasetDescriptions();

    /** Configure the dataset selection for the columns.
            Every call to this method resets the previous dataset
            description.
        */
    void setDatasetColumnDescriptionVector(const DatasetDescriptionVector &columnConfig);

    /** Configure the dataset selection for the rows.
            Every call to this method resets the previous dataset
            description.
        */
    void setDatasetRowDescriptionVector(const DatasetDescriptionVector &rowConfig);

    /** Convenience method to configure rows and columns in one step. */
    void setDatasetDescriptionVectors(
        const DatasetDescriptionVector &rowConfig,
        const DatasetDescriptionVector &columnConfig);

    // FIXME: add convenience methods to configure common dataset
    // selections (like rectangular areas etc)

protected:
    /** Decide whether the column is accepted. */
    bool filterAcceptsColumn(int sourceColumn,
                             const QModelIndex &) const override;

    /** Decide whether the row is accepted. */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    /** Map a proxy column to a source column. */
    int mapProxyColumnToSource(const int &proxyColumn) const;

    /** Map a source column to a proxy column. */
    int mapSourceColumnToProxy(const int &sourceColumn) const;

    /** Map a proxy row to a source row. */
    int mapProxyRowToSource(const int &proxyRow) const;

    /** Map a source row to a proxy row. */
    int mapSourceRowToProxy(const int &sourceRow) const;

    /** Initialize the transformation vectors from the dataset
            description.

            The input parameter "Configuration" is a vector that specifies
            what srce column will be mapped to what proxy column. Example:

            position: [0][1][2]
            value:    [2][0][1]

            This will map the source column 2 to proxy column 0, source 0 to
            proxy 1, and source 1 to proxy 2. Source needs to have at least 2
            column. The source-to-proxy mapping looks the same, except that it
            may contain values of -1, which means this column is not part of
            the resulting model. The values in the configuration vector must
            be unique (otherwise, a 1-to-1 mapping in both directions is
            impossible).

            sourceCount is the number of columns in the source model. The proxy-to-source map has
            as many elements as the proxy has columns, the source-to-proxy map
            has as many elements as the source has columns. Same goes for rows
            (the mapping logic is the same).

         */
    void initializeDatasetDecriptors(
        const DatasetDescriptionVector &inConfiguration,
        int sourceCount,
        DatasetDescriptionVector &outSourceToProxyMap,
        DatasetDescriptionVector &outProxyToSourceMap);

    DatasetDescriptionVector mColSrcToProxyMap;
    DatasetDescriptionVector mColProxyToSrcMap;
    DatasetDescriptionVector mRowSrcToProxyMap;
    DatasetDescriptionVector mRowProxyToSrcMap;

    int mProxyRowCount = 0;
    int mProxyColumnCount = 0;
    QModelIndex mRootIndex;
};
}

#endif
