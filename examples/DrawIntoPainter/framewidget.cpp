/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "framewidget.h"

#include <KDChartChart>

#include <QDebug>
#include <QPainter>

FrameWidget::FrameWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    // this block left empty intentionally
}

void FrameWidget::setChart(KDChart::Chart *chart)
{
    mChart = chart;
    // This is necessary because Chart can't automatically schedule somebody else (this object) to
    // call its custom paint method.
    connect(mChart, SIGNAL(propertiesChanged()), SLOT(update()));
}

void FrameWidget::paintEvent(QPaintEvent *e)
{
    if (!mChart) {
        QWidget::paintEvent(e);
    } else {
        QPainter painter(this);

        const int wid = 64;
        const QRect r(rect());
        const QPen oldPen(painter.pen());
        const QBrush oldBrush(painter.brush());
        // paint below the chart
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(QBrush(QColor(0xd0, 0xd0, 0xff)));
        painter.drawEllipse(r.left(), r.top(), wid, wid);
        painter.drawEllipse(r.left() + r.width() - wid - 1, r.top(), wid, wid);
        painter.drawEllipse(r.left(), r.top() + r.height() - wid - 1, wid, wid);
        painter.drawEllipse(r.left() + r.width() - wid - 1, r.top() + r.height() - wid - 1, wid, wid);
        painter.setBrush(oldBrush);
        painter.setPen(oldPen);
        // paint the chart
        mChart->paint(&painter, QRect(r.left() + wid / 2, r.top() + wid / 2, r.width() - wid, r.height() - wid));
        // paint over the chart
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(QBrush(QColor(0xd0, 0xff, 0xff)));
        const int wid2 = 40;
        const int gap = (wid - wid2) / 2;
        painter.drawEllipse(r.left() + gap, r.top() + gap, wid2, wid2);
        painter.drawEllipse(r.left() + r.width() - wid + gap - 1, r.top() + gap, wid2, wid2);
        painter.drawEllipse(r.left() + gap, r.top() + r.height() - wid + gap - 1, wid2, wid2);
        painter.drawEllipse(r.left() + r.width() - wid + gap - 1, r.top() + r.height() - wid + gap - 1, wid2, wid2);
        painter.setBrush(oldBrush);
        painter.setPen(oldPen);
    }
}
