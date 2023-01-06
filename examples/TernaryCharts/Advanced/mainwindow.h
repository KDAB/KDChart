/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QStandardItemModel>

#include "ui_mainwindow.h"

namespace KDChart {

class Chart;
class TernaryLineDiagram;
class TernaryPointDiagram;
class TernaryCoordinatePlane;
}

class MainWindow : public QDialog,
                   private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void indexClicked(const QModelIndex &index);

private:
    void setupModel();

    KDChart::Chart *m_chart = nullptr;
    KDChart::TernaryPointDiagram *m_diagram = nullptr;
    KDChart::TernaryCoordinatePlane *m_ternaryPlane = nullptr;

    QStandardItemModel m_model;
};

#endif
