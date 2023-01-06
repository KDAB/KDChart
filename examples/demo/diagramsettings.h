/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef DIAGRAMSETTINGS_H
#define DIAGRAMSETTINGS_H

#include <QWidget>

namespace KDChart {
class Chart;
}

class DiagramSettings : public QWidget
{
    Q_OBJECT

public:
    explicit DiagramSettings(KDChart::Chart *chart, QWidget *parent = nullptr);
    ~DiagramSettings() override;

public Q_SLOTS:
    void refreshSettings();

private:
    class Private;
    Private *d;
};

#endif // DIAGRAMSETTINGS_H
