/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <KDChartWidget>

class ZoomWidget : public KDChart::Widget
{
    Q_OBJECT

public:
    explicit ZoomWidget(QWidget *parent);

protected:
    QPointF findNewZoomCenter(const QPoint &pos);
    void mousePressEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
};

#endif /* ZOOMWIDGET_H */
