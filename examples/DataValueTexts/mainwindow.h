/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <KDChartDataValueAttributes>
#include <TableModel.h>

namespace KDChart {
class Chart;
class BarDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void on_scopeOneBarRB_toggled(bool checked);
    void on_scopeBarDatasetSB_valueChanged(int i);
    void on_scopeBarItemSB_valueChanged(int i);
    void on_scopeDatasetRB_toggled(bool checked);
    void on_scopeDatasetSB_valueChanged(int i);
    void on_scopeCommonRB_toggled(bool checked);

    void on_paintValuesCB_toggled(bool checked);

    void on_fontCombo_currentIndexChanged(int index);
    void on_relativeSizeSB_valueChanged(int i);
    void on_minimumSizeSB_valueChanged(int i);
    void on_rotationSB_valueChanged(int i);

    void on_posPosCombo_currentIndexChanged(int index);
    void on_posAlignCombo_currentIndexChanged(int index);
    void on_posPadHoriSB_valueChanged(int i);
    void on_posPadVertSB_valueChanged(int i);
    void on_negPosCombo_currentIndexChanged(int index);
    void on_negAlignCombo_currentIndexChanged(int index);
    void on_negPadHoriSB_valueChanged(int i);
    void on_negPadVertSB_valueChanged(int i);

    void on_labelLE_textEdited(const QString &text);
    void on_prefixLE_textEdited(const QString &text);
    void on_suffixLE_textEdited(const QString &text);

private:
    const QModelIndex currentIndex() const;
    const KDChart::DataValueAttributes attributes() const;
    const char *positionToScreenName(const KDChart::Position &pos) const;
    const Qt::Alignment alignmentFromScreeName(const QString &name) const;
    const QString alignmentToScreenName(const Qt::Alignment &align) const;
    void populateWidgets();
    void setAttributes(const KDChart::DataValueAttributes &da);

    KDChart::Chart *m_chart;
    KDChart::BarDiagram *m_bars;
    TableModel m_model;
};

#endif /* MAINWINDOW_H */
