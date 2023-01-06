/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef DATAVALUESETTINGS_H
#define DATAVALUESETTINGS_H

#include <QWidget>

namespace KDChart {
class Chart;
}

class DataValueSettings : public QWidget
{
    Q_OBJECT

public:
    DataValueSettings(KDChart::Chart *chart, QWidget *parent = nullptr);
    ~DataValueSettings() override;

public Q_SLOTS:
    void refresh();

private:
    class Private;
    Private *d;
};

#endif /* MAINWINDOW_H */
