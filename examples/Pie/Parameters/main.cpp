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
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartPieAttributes>
#include <KDChartPieDiagram>
#include <KDChartPosition>
#include <QtGui>

#include <QApplication>

using namespace KDChart;

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        m_model.insertRows(0, 1, QModelIndex());
        m_model.insertColumns(0, 6, QModelIndex());
        for (int row = 0; row < 1; ++row) {
            for (int column = 0; column < 6; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row + 1 * column + 1));

                // this shows the index as static comments:
                // m_model.setData(index, QString("row: %1,  column: %2").arg(row).arg(column), KDChart::CommentRole);

                // this shows the index as volatile tooltips:
                m_model.setData(index, QString("row: %1,  column: %2").arg(row).arg(column), Qt::ToolTipRole);
            }
        }
        // We need a Polar plane for the Pie type
        auto *polarPlane = new PolarCoordinatePlane(&m_chart);
        // replace the default Cartesian plane with
        // our Polar plane
        m_chart.replaceCoordinatePlane(polarPlane);

        // assign the model to our pie diagram
        auto *diagram = new PieDiagram;
        diagram->setModel(&m_model);

        // Configure some Pie specifical attributes

        // explode a section
        PieAttributes pa(diagram->pieAttributes());
        pa.setExplodeFactor(0.1);

        // Assign the attributes
        // to the diagram
        diagram->setPieAttributes(1, pa);

        // Configure a generic attribute
        // available to all chart types
        QPen sectionPen;
        sectionPen.setWidth(5);
        sectionPen.setStyle(Qt::DashLine);
        sectionPen.setColor(Qt::magenta);

        diagram->setPen(1, sectionPen);

        // Display data values
        // not implemented yet - disable for now
        const QFont font(QFont("Comic", 10));
        const int colCount = diagram->model()->columnCount();
        for (int iColumn = 0; iColumn < colCount; ++iColumn) {
            DataValueAttributes dva(diagram->dataValueAttributes(iColumn));
            TextAttributes ta(dva.textAttributes());
            ta.setRotation(0);
            ta.setFont(font);
            ta.setPen(QPen(Qt::darkBlue));
            ta.setVisible(true);
            dva.setTextAttributes(ta);

            BackgroundAttributes back(dva.backgroundAttributes());
            back.setBrush(QBrush(QColor(255, 0, 0)));
            back.setVisible(true);
            dva.setBackgroundAttributes(back);

            RelativePosition posPos(dva.positivePosition());
            posPos.setReferencePosition(KDChart::Position::North);
            posPos.setAlignment(Qt::AlignCenter);
            posPos.setHorizontalPadding(KDChart::Measure(0.0));
            posPos.setVerticalPadding(KDChart::Measure(-1000.0));
            dva.setPositivePosition(posPos);
            dva.setVisible(true);
            diagram->setDataValueAttributes(iColumn, dva);
        }

        // Assign our diagram to the Chart
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
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
