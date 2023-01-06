/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef CHARTGRAPHICSITEM_H
#define CHARTGRAPHICSITEM_H

#include <QGraphicsPolygonItem>

namespace KDChart {

/**
 * @brief Graphics item used inside of the ReverseMapper
 * \internal
 */
class ChartGraphicsItem : public QGraphicsPolygonItem
{
public:
    enum
    {
        Type = UserType + 1
    };

    ChartGraphicsItem();

    ChartGraphicsItem(int row, int column);

    int row() const
    {
        return m_row;
    }
    int column() const
    {
        return m_column;
    }
    int type() const override
    {
        return Type;
    }

private:
    int m_row = -1;
    int m_column = -1;
};
}

#endif
