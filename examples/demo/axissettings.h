/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef AXISSETTINGS_H
#define AXISSETTINGS_H

#include <QWidget>

namespace KDChart {
class Chart;
}

class AxisSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AxisSettings(KDChart::Chart *chart, QWidget *parent = nullptr);
    ~AxisSettings() override;

public Q_SLOTS:
    void diagramTypeChanged();

private:
    class Private;
    Private *d;
};

#endif // AXISSETTINGS_H
