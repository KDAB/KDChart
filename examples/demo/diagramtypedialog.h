/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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
