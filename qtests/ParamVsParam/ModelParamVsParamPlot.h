/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef SUMGUI_MODELPARAMVSPARAMPLOT_H
#define SUMGUI_MODELPARAMVSPARAMPLOT_H

#include <QSortFilterProxyModel>

class ModelParamVsParamPlot : public QSortFilterProxyModel
{
public:
    ModelParamVsParamPlot(
        QAbstractItemModel *p_sourceModel,
        int p_column1,
        int p_column2,
        QObject *p_parent = 0);

    int columnCount(
        const QModelIndex &p_parent = QModelIndex()) const override;

protected:
    QVariant data(
        const QModelIndex &p_index,
        int p_role = Qt::DisplayRole) const override;

    bool filterAcceptsColumn(
        int p_source_column,
        const QModelIndex &p_source_parent) const override;

private:
    int m_column1;
    int m_column2;
}; // class ModelParamVsParamPlot

#endif // SUMGUI_MODELPARAMVSPARAMPLOT_H
