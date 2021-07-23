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

#include "ui_mainwindow.h"
#include <TableModel.h>

#include <QPixmap>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class LineDiagram;
class Legend;
}

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    /**
     * Adjusts the two small charts when the window is resized.
     */
    /* reimp */ void resizeEvent(QResizeEvent *) override;

private slots:

    void on_lineTypeCB_currentIndexChanged(const QString &text);
    void on_paintLegendCB_toggled(bool checked);
    void on_paintValuesCB_toggled(bool checked);
    void on_paintMarkersCB_toggled(bool checked);
    void on_markersStyleCB_currentIndexChanged(const QString &text);
    void on_markersWidthSB_valueChanged(int i);
    void on_markersHeightSB_valueChanged(int i);
    void on_displayAreasCB_toggled(bool checked);
    void on_transparencySB_valueChanged(int i);
    void on_zoomFactorSB_valueChanged(double factor);
    void on_hSBar_valueChanged(int value);
    void on_vSBar_valueChanged(int value);
    void on_savePB_clicked();
    void on_savePDF_clicked();

private:
    TableModel m_model;
    KDChart::DatasetProxyModel *m_datasetProxy;
    KDChart::Legend *m_legend;

    void paintMarkers(bool checked, const QSize &printSize);

protected:
    KDChart::Chart *m_chart;
    KDChart::LineDiagram *m_lines;
    QPixmap m_pix1, m_pix2;
    QLabel *m_smallChart1;
    QLabel *m_smallChart2;
};

#endif /* MAINWINDOW_H */
