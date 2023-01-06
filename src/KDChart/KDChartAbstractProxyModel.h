/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTPROXYMODEL_H
#define KDCHARTABSTRACTPROXYMODEL_H

#include <QAbstractProxyModel>

#include "KDChartGlobal.h"

namespace KDChart {
/**
 * @brief Base class for all proxy models used inside KD Chart
 * \internal
 */
class KDCHART_EXPORT AbstractProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit AbstractProxyModel(QObject *parent = nullptr);

    /*! \reimp */
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    /*! \reimp */
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    /*! \reimp */
    QModelIndex index(int row, int col, const QModelIndex &index) const override;
    /*! \reimp */
    QModelIndex parent(const QModelIndex &index) const override;
};
}

#endif /* KDCHARTABSTRACTPROXYMODEL_H */
