/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef DIAGRAMTYPEDIALOG_H
#define DIAGRAMTYPEDIALOG_H

#include <QDialog>
#include <QtCore/QHash>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

namespace KDChart {
class Chart;
}

class DiagramTypeDialog : public QDialog
{
    Q_OBJECT
public:
    enum DiagramType
    {
        Bar,
        LyingBar,
        Line,
        Plotter,
        Pie
    };
    enum Subtype
    {
        Normal,
        Percent,
        Stacked
    };
    explicit DiagramTypeDialog(KDChart::Chart *chart, QWidget *parent = nullptr);
    ~DiagramTypeDialog() override;

    void setDefaultModels(QHash<DiagramType, QAbstractItemModel *> models);

Q_SIGNALS:
    void diagramTypeChanged(DiagramType, Subtype);

private:
    class Private;
    Private *d;
};

#endif // DIAGRAMTYPEDIALOG_H
