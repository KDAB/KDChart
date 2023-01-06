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
#include <TableModel.h>

#include <QPixmap>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class LineDiagram;
class LineAttributes;
class CartesianAxis;
class Legend;
}

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void updateData(QString data);

protected:
    /**
     * Adjusts the two small charts when the window is resized.
     */
    /* reimp */ void resizeEvent(QResizeEvent *) override;

private slots:

    void on_lineTypeCB_currentIndexChanged(int index);
    void on_paintLegendCB_toggled(bool checked);
    void on_paintValuesCB_toggled(bool checked);
    void on_paintMarkersCB_toggled(bool checked);
    void on_markersStyleCB_currentIndexChanged(int index);
    void on_markersWidthSB_valueChanged(int i);
    void on_markersHeightSB_valueChanged(int i);
    void on_displayAreasCB_toggled(bool checked);
    void on_transparencySB_valueChanged(int i);
    void on_zoomFactorSB_valueChanged(qreal factor);
    void on_hSBar_valueChanged(int value);
    void on_vSBar_valueChanged(int value);
    void on_savePB_clicked();

private:
    KDChart::Chart *m_chart;
    TableModel m_model;
    KDChart::DatasetProxyModel *m_datasetProxy;
    KDChart::LineDiagram *m_lines;
    KDChart::Legend *m_legend;
    QPixmap m_pix1, m_pix2;
    QLabel *m_smallChart1;
    QLabel *m_smallChart2;
    // mutable KDChart::CartesianAxis xAxis;
    // mutable KDChart::CartesianAxis yAxis;
};

#endif /* MAINWINDOW_H */
