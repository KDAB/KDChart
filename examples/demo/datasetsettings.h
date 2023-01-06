/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef DATASETSETTINGS_H
#define DATASETSETTINGS_H

#include <QWidget>

namespace KDChart {
class Chart;
}

class DatasetSettings : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int datasetCount READ datasetCount WRITE setDatasetCount NOTIFY datasetCountChanged())

public:
    explicit DatasetSettings(KDChart::Chart *chart, QWidget *parent = nullptr);
    ~DatasetSettings() override;
    int datasetCount() const;
public Q_SLOTS:
    void setDatasetCount(int value);
    void indexChanged(int index);
    void diagramTypeChanged();

Q_SIGNALS:
    void datasetCountChanged();

private:
    class Private;
    Private *d;
};

#endif // DATASETSETTINGS_H
