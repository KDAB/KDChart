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
#include <KDChartCartesianAxis>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartLegend>
#include <QApplication>
#include <QPushButton>
#include <QtGui>

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = 0)
        : QWidget(parent)
    {
        m_model.insertRows(0, 0, QModelIndex());
        auto *diagram = new KDChart::BarDiagram;
        diagram->setModel(&m_model);

        auto *legend = new KDChart::Legend(diagram, diagram);
        m_chart.addLegend(legend);

        // The code below doesn't work, but it would
        // be nice if it did:
#if 0
    KDChart::Legend* legend = new KDChart::Legend;
    legend->addDiagram(diagram1);
    legend->addDiagram(diagram2);
    ...
    m_chart.addLegend(legend);
#endif

        auto *abcissa = new KDChart::CartesianAxis(diagram);
        abcissa->setPosition(KDChart::CartesianAxis::Bottom);
        auto *ordinate = new KDChart::CartesianAxis(diagram);
        ordinate->setPosition(KDChart::CartesianAxis::Left);
        diagram->addAxis(abcissa);
        diagram->addAxis(ordinate);

        // NOTE: If this is done before adding axes,
        // the axes don't show up at all
        m_chart.coordinatePlane()->replaceDiagram(diagram);

        m_rowbutton.setText(tr("Add rows"));
        m_colbutton.setText(tr("Add columns"));
        connect(&m_rowbutton, SIGNAL(clicked()),
                this, SLOT(addRows()));
        connect(&m_colbutton, SIGNAL(clicked()),
                this, SLOT(addCols()));

        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        l->addWidget(&m_rowbutton);
        l->addWidget(&m_colbutton);

        setLayout(l);
    }

private slots:

    void addRows()
    {
        m_model.insertRows(m_model.rowCount(), 1);
        for (int i = 0; i < m_model.columnCount(); ++i) {
            m_model.setData(m_model.index(m_model.rowCount() - 1, i), i);
        }
    }
    void addCols()
    {
        m_model.insertColumns(m_model.columnCount(), 1);
        for (int i = 0; i < m_model.rowCount(); ++i) {
            m_model.setData(m_model.index(i, m_model.columnCount() - 1), i);
        }
    }

private:
    KDChart::Chart m_chart;
    QPushButton m_rowbutton;
    QPushButton m_colbutton;
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
