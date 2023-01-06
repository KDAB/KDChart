/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <QApplication>
#include <QtGui>

using namespace KDChart;

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);

private:
    Chart m_chart;
    QStandardItemModel m_model;
};

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
{
    m_model.insertRows(0, 2, QModelIndex());
    m_model.insertColumns(0, 3, QModelIndex());
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            QModelIndex index = m_model.index(row, column, QModelIndex());
            m_model.setData(index, row * 3 + column);
        }
    }

    auto *diagram = new KDChart::BarDiagram;
    diagram->setModel(&m_model);

    BarAttributes ba(diagram->barAttributes());
    // set the bar width and enable it
    ba.setFixedBarWidth(140);
    ba.setUseFixedBarWidth(true);
    // configure gap between values
    // and blocks
    ba.setGroupGapFactor(0.50);
    ba.setBarGapFactor(0.125);

    // assign to the diagram
    diagram->setBarAttributes(ba);

    // display the values
    DataValueAttributes dva(diagram->dataValueAttributes());
    TextAttributes ta(dva.textAttributes());
    // rotate if you wish
    // ta.setRotation( 0 );
    ta.setFont(QFont("Comic", 9));
    ta.setPen(QPen(QColor(Qt::darkGreen)));
    ta.setVisible(true);
    dva.setTextAttributes(ta);
    dva.setVisible(true);
    diagram->setDataValueAttributes(dva);

    // draw a surrounding line around bars
    QPen linePen;
    linePen.setColor(Qt::magenta);
    linePen.setWidth(4);
    linePen.setStyle(Qt::DotLine);
    // draw only around a dataset
    // to draw around all the bars
    //  call setPen( myPen );
    diagram->setPen(1, linePen);

    m_chart.coordinatePlane()->replaceDiagram(diagram);
    m_chart.setGlobalLeadingTop(40);

    auto *l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    setLayout(l);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
