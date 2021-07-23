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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "Model.h"

#include "ui_Controls.h"

#include <QWidget>

namespace KDChart {
class Plotter;
}
class Model;

QT_BEGIN_NAMESPACE
class QPushButton;
class QRadioButton;
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget();

private slots:
    void functionToggled(bool checked);
    void addPointsButtonClicked();

private:
    QWidget *m_controlsContainer;
    Ui::Controls m_controls;
    QVector<QRadioButton *> m_functionSelector;
    QVector<QPushButton *> m_addPointsButtons;

    KDChart::Plotter *m_plotter;
    Model m_model;
};

#endif
