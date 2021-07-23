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

#ifndef SUMGUI_MODELPARAMVSPARAM_H
#define SUMGUI_MODELPARAMVSPARAM_H

#include <QStandardItemModel>
#include <QTimer>

class ModelParamVsParam : public QStandardItemModel
{
    Q_OBJECT;

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
