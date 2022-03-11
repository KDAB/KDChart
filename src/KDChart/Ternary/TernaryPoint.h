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

#ifndef TERNARYPOINT_H
#define TERNARYPOINT_H

#include <QtDebug>
#include <QPointF>

/**
  * @brief TernaryPoint defines a point within a ternary coordinate plane
  * \internal
  */
class TernaryPoint
{
public:
    TernaryPoint();
    TernaryPoint(qreal a, qreal b);

    qreal a() const
    {
        return m_a;
    }
    qreal b() const
    {
        return m_b;
    }
    qreal c() const
    {
        return 1.0 - m_a - m_b;
    }

    void set(qreal a, qreal b);

    bool isValid() const;

private:
    qreal m_a = -1.0;
    qreal m_b = -1.0;
};

QDebug operator<<(QDebug stream, const TernaryPoint &point);

QPointF translate(const TernaryPoint &);

#endif
