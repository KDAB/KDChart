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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class LineDiagram;
}

QT_BEGIN_NAMESPACE
class QPainterPath;
QT_END_NAMESPACE

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    void on_lineTypeCB_currentIndexChanged(const QString &text);
    void on_paintValuesCB_toggled(bool checked);
    void on_paintMarkersCB_toggled(bool checked);
    void on_paintLinesCB_toggled(bool checked);
    void on_markersStyleCB_currentIndexChanged(const QString &text);
    void on_markersWidthSB_valueChanged(int i);
    void on_markersHeightSB_valueChanged(int i);

private:
    KDChart::Chart *m_chart;
    KDChart::LineDiagram *m_lines;
    TableModel m_model;
    QPainterPath *path;
};

#endif /* MAINWINDOW_H */
