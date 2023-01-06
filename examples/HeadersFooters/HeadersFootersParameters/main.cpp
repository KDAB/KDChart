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
#include <KDChartHeaderFooter>
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

        // Add at one Header and set it up
        auto *header = new HeaderFooter(&m_chart);
        header->setPosition(Position::North);
        header->setText("A Simple Bar Chart");
        m_chart.addHeaderFooter(header);

        // Configure the Header text attributes
        TextAttributes hta;
        hta.setPen(QPen(Qt::blue));

        // let the header resize itself
        // together with the widget.
        // so-called relative size
        Measure m(35.0);
        m.setRelativeMode(header->autoReferenceArea(), KDChartEnums::MeasureOrientationMinimum);
        hta.setFontSize(m);
        // min font size
        m.setValue(3.0);
        m.setCalculationMode(KDChartEnums::MeasureCalculationModeAbsolute);
        hta.setMinimalFontSize(m);
        header->setTextAttributes(hta);

        // Configure the header Background attributes
        BackgroundAttributes hba;
        hba.setBrush(Qt::white);
        hba.setVisible(true);
        header->setBackgroundAttributes(hba);

        // Configure the header Frame attributes
        FrameAttributes hfa;
        hfa.setPen(QPen(QBrush(Qt::darkGray), 2));
        hfa.setVisible(true);
        header->setFrameAttributes(hfa);

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
