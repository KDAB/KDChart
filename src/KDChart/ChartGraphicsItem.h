/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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
    enum {
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
