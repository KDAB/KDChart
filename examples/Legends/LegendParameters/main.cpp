/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartBackgroundAttributes>
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartLegend>
#include <KDChartPosition>
#include <QApplication>
#include <QtGui>

using namespace KDChart;

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        m_model.insertRows(0, 2, QModelIndex());
        m_model.insertColumns(0, 3, QModelIndex());
        for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row + 1 * column));
            }
        }

        auto *diagram = new BarDiagram;
        diagram->setModel(&m_model);

        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // Add at one legend and set it up
        auto *legend = new Legend(diagram, &m_chart);
        legend->setPosition(Position::North);
        legend->setAlignment(Qt::AlignCenter);
        legend->setShowLines(true);
        legend->setSpacing(5);
        legend->setTitleText(tr("Legend"));
        legend->setOrientation(Qt::Horizontal);
        m_chart.addLegend(legend);

        // Configure the items markers
        MarkerAttributes lma(legend->markerAttributes(0));
        lma.setMarkerStyle(MarkerAttributes::MarkerDiamond);
        legend->setMarkerAttributes(0, lma);
        lma.setMarkerStyle(MarkerAttributes::MarkerCircle);
        legend->setMarkerAttributes(1, lma);

        // Configure Legend Title and labels
        legend->setTitleText("Bars");
        legend->setText(0, "Series 1");
        legend->setText(1, "Series 2");
        legend->setText(2, "Series 3");

        // adjust the legend item's font:
        TextAttributes lta(legend->textAttributes());
        lta.setPen(QPen(Qt::darkGray));
        Measure me(lta.fontSize());
        me.setValue(me.value() * 1.5);
        lta.setFontSize(Measure(9, KDChartEnums::MeasureCalculationModeAbsolute));
        legend->setTextAttributes(lta);

        // adjust the legend title's font:
        lta = legend->titleTextAttributes();
        lta.setPen(QPen(Qt::darkGray));
        me = lta.fontSize();
        me.setValue(me.value() * 1.5);
        lta.setFontSize(me);
        legend->setTitleTextAttributes(lta);

        // Configure a pen to surround
        // the markers with a border
        QPen markerPen;
        markerPen.setColor(Qt::darkGray);
        markerPen.setWidth(2);
        // Pending Michel use datasetCount() here as soon
        // as it is fixed
        for (uint i = 0; i < /*legend->datasetCount()*/ 3; i++)
            legend->setPen(i, markerPen);

        // Add a background to your legend
        BackgroundAttributes ba;
        ba.setBrush(Qt::white);
        ba.setVisible(true);
        legend->setBackgroundAttributes(ba);

        FrameAttributes fa;
        fa.setPen(markerPen);
        fa.setVisible(true);
        legend->setFrameAttributes(fa);

        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        m_chart.setGlobalLeadingTop(10);
        setLayout(l);
    }

private:
    Chart m_chart;
    QStandardItemModel m_model;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
