/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>

namespace KDChart {
class Chart;
}

class FrameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameWidget(QWidget *parent = nullptr, Qt::WindowFlags f = {});

    void paintEvent(QPaintEvent *) override;

    void setChart(KDChart::Chart *chart);

private:
    KDChart::Chart *mChart = nullptr;
};

#endif /* FRAMEWIDGET_H */
