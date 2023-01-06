/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef SUMGUI_MODELPARAMVSPARAM_H
#define SUMGUI_MODELPARAMVSPARAM_H

#include <QStandardItemModel>
#include <QTimer>

class ModelParamVsParam : public QStandardItemModel
{
    Q_OBJECT

public:
    ModelParamVsParam(
        QObject *p_parent = 0);

    void setTimeout(
        int p_timeout);

    void startSampling();

    void stopSampling();

    void populate(
        int p_nrOfParameters = 8,
        int p_nrOfSamples = 100);

private slots:
    void timeout();

private:
    QTimer m_timer;
}; // class ModelParamVsParam

#endif // SUMGUI_MODELPARAMVSPARAM_H
