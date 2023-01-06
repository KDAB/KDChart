/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "TernaryPoint.h"
#include "TernaryConstants.h"

#include <limits>

#include <QChar>
#include <QTextStream>

TernaryPoint::TernaryPoint()
{
    Q_ASSERT(!isValid());
}

TernaryPoint::TernaryPoint(qreal a, qreal b)
{
    set(a, b);
}

void TernaryPoint::set(qreal a, qreal b)
{
    if (a >= 0.0 && a <= 1.0
        && b >= 0.0 && b <= 1.0
        && 1.0 - a - b >= -2.0 * std::numeric_limits<qreal>::epsilon()) {
        m_a = a;
        m_b = b;
        Q_ASSERT(isValid()); // more a test for isValid
    } else {
        m_a = -1.0;
        m_b = -1.0;
        Q_ASSERT(!isValid());
    }
}

bool TernaryPoint::isValid() const
{
    return m_a >= 0.0 && m_a <= 1.0
        && m_b >= 0.0 && m_b <= 1.0
        && 1.0 - m_a + m_b >= -std::numeric_limits<qreal>::epsilon();
}

QDebug operator<<(QDebug stream, const TernaryPoint &point)
{
    QString string;
    QTextStream text(&string);
    text << "[TernaryPoint: ";
    if (point.isValid()) {
        text.setFieldWidth(2);
        text.setPadChar(QLatin1Char('0'));
        text << ( int )(point.a() * 100.0) << "%|"
             << ( int )(point.b() * 100.0) << "%|"
             << ( int )(point.c() * 100.0) << "%]";
    } else {
        text << "a=" << point.a() << " - b=" << point.b() << " - INVALID]";
    }
    stream << string;
    return stream;
}

QPointF translate(const TernaryPoint &point)
{
    if (point.isValid()) {
        // the position is calculated by
        // - first moving along the B-C line to the function that b
        //   selects
        // - then traversing the selected function until we meet with
        //   the function that A selects (which is a parallel of the B-C
        //   line)
        QPointF bPosition(1.0 - point.b(), 0.0);
        QPointF aPosition(point.a() * AxisVector_C_A);
        QPointF result(bPosition + aPosition);
        return result;
    } else {
        qWarning() << "TernaryPoint::translate(TernaryPoint): cannot translate invalid ternary points:"
                   << point;
        return QPointF();
    }
}
