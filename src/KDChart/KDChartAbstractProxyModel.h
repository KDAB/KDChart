/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
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
