/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "ChartGraphicsItem.h"

using namespace KDChart;

ChartGraphicsItem::ChartGraphicsItem()
    : QGraphicsPolygonItem()
{
}

ChartGraphicsItem::ChartGraphicsItem(int row, int column)
    : QGraphicsPolygonItem()
    , m_row(row)
    , m_column(column)
{
}
