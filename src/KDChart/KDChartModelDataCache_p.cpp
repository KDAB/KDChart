/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartModelDataCache_p.h"

#include <limits>

using namespace KDChart::ModelDataCachePrivate;

ModelSignalMapperConnector::ModelSignalMapperConnector(ModelSignalMapper &mapper)
    : QObject(nullptr)
    , m_mapper(mapper)
{
}

ModelSignalMapperConnector::~ModelSignalMapperConnector()
{
}

void ModelSignalMapperConnector::connectSignals(QAbstractItemModel *model)
{
    connect(model, &QAbstractItemModel::destroyed, this, &ModelSignalMapperConnector::resetModel);
    connect(model, &QAbstractItemModel::columnsInserted, this, &ModelSignalMapperConnector::columnsInserted);
    connect(model, &QAbstractItemModel::columnsRemoved, this, &ModelSignalMapperConnector::columnsRemoved);
    connect(model, &QAbstractItemModel::dataChanged, this, &ModelSignalMapperConnector::dataChanged);
    connect(model, &QAbstractItemModel::layoutChanged, this, &ModelSignalMapperConnector::layoutChanged);
    connect(model, &QAbstractItemModel::modelReset, this, &ModelSignalMapperConnector::modelReset);
    connect(model, &QAbstractItemModel::rowsInserted, this, &ModelSignalMapperConnector::rowsInserted);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &ModelSignalMapperConnector::rowsRemoved);
}

void ModelSignalMapperConnector::disconnectSignals(QAbstractItemModel *model)
{
    disconnect(model, &QAbstractItemModel::destroyed, this, &ModelSignalMapperConnector::resetModel);
    disconnect(model, &QAbstractItemModel::columnsInserted, this, &ModelSignalMapperConnector::columnsInserted);
    disconnect(model, &QAbstractItemModel::columnsRemoved, this, &ModelSignalMapperConnector::columnsRemoved);
    disconnect(model, &QAbstractItemModel::dataChanged, this, &ModelSignalMapperConnector::dataChanged);
    disconnect(model, &QAbstractItemModel::layoutChanged, this, &ModelSignalMapperConnector::layoutChanged);
    disconnect(model, &QAbstractItemModel::modelReset, this, &ModelSignalMapperConnector::modelReset);
    disconnect(model, &QAbstractItemModel::rowsInserted, this, &ModelSignalMapperConnector::rowsInserted);
    disconnect(model, &QAbstractItemModel::rowsRemoved, this, &ModelSignalMapperConnector::rowsRemoved);
}

void ModelSignalMapperConnector::resetModel()
{
    m_mapper.resetModel();
}

void ModelSignalMapperConnector::columnsInserted(const QModelIndex &parent, int start, int end)
{
    m_mapper.columnsInserted(parent, start, end);
}

void ModelSignalMapperConnector::columnsRemoved(const QModelIndex &parent, int start, int end)
{
    m_mapper.columnsRemoved(parent, start, end);
}

void ModelSignalMapperConnector::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    m_mapper.dataChanged(topLeft, bottomRight);
}

void ModelSignalMapperConnector::layoutChanged()
{
    m_mapper.layoutChanged();
}

void ModelSignalMapperConnector::modelReset()
{
    m_mapper.modelReset();
}

void ModelSignalMapperConnector::rowsInserted(const QModelIndex &parent, int start, int end)
{
    m_mapper.rowsInserted(parent, start, end);
}

void ModelSignalMapperConnector::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    m_mapper.rowsRemoved(parent, start, end);
}
