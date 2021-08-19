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

#include <QWidget>

class ModelParamVsParam;

QT_BEGIN_NAMESPACE
class QGridLayout;
class QLineEdit;
QT_END_NAMESPACE

namespace KDChart {
class LineDiagram;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(
        QWidget *p_parent = 0);

    ~MainWindow() override;

private slots:
    void timeoutEditingFinished();
    void editingFinished();

private:
    void addPlots();
    void removePlots();
    void setMarkerAttributes(
        KDChart::LineDiagram *p_lineDiagram);

private:
    ModelParamVsParam *m_sourceModel;
    QLineEdit *m_timeoutLineEdit;
    QLineEdit *m_nrOfParametersLineEdit;
    QLineEdit *m_nrOfSamplesLineEdit;
    QGridLayout *m_paramVsParamGridLayout;
}; //	class	MainWindow

#endif // MAINWINDOW_H
