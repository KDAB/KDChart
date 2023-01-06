/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "mainwindow.h"
#include "AdjustedCartesianAxis.h"

#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartGridAttributes>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>

#include <QDebug>
#include <QLinearGradient>
#include <QTextDocument>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    auto *chartLayout = new QHBoxLayout(m_chartFrame);
    m_chart = new KDChart::Chart;
    chartLayout->addWidget(m_chart);

    m_model.loadFromCSV(":/data");

    // Set up the diagram
    m_lines = new KDChart::LineDiagram();
    m_lines->setModel(&m_model);

    m_xAxis = new KDChart::CartesianAxis(m_lines);
    KDChart::TextAttributes ta(m_xAxis->textAttributes());

    auto *yAxis = new AdjustedCartesianAxis(m_lines);
    yAxis->setBounds(3, 6);
    m_xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    yAxis->setPosition(KDChart::CartesianAxis::Left);

// set the following to 0, to see the default Abscissa labels (== X headers, as read from the data file)
#if 1
    QStringList daysOfWeek;
    daysOfWeek << "Monday"
               << "Tuesday"
               << "Wednesday"
               << "Thursday"
               << "Friday"
               << "Saturday"
               << "Sunday";
    m_xAxis->setLabels(daysOfWeek);

    // QStringList shortDays;
    // shortDays << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    // m_xAxis->setShortLabels( shortDays );
#endif

// Use HTML for drawing the text in the axis labels.
#if 0
    QStringList htmlStyles;
    htmlStyles << "<b>Bold</b>" << "<i>Italic</i>" << "<u>Underline</u>" << "<font color='red'>Red</font>";
    m_xAxis->setLabels( htmlStyles );
    ta.setTextDocument(new QTextDocument);
#endif

    // Illustration of custom ticks
    QList<qreal> ticks;
    ticks.append(0.5);
    ticks.append(3.5);
    ticks.append(4.2);
    ticks.append(6.5);
    m_xAxis->setCustomTicks(ticks);
    yAxis->setCustomTicks(ticks);

    // rotate abscissa labels by -60 degrees:
    ta.setRotation(-60);

    m_xAxis->setTextAttributes(ta);
    m_lines->addAxis(m_xAxis);
    m_lines->addAxis(yAxis);
    m_chart->coordinatePlane()->replaceDiagram(m_lines);
    // Set up the legend
    m_xAxis->setCustomTickLength(11);
    yAxis->setCustomTickLength(11);
    m_legend = new KDChart::Legend(m_lines, m_chart);
    m_legend->setPosition(KDChart::Position::East);
    m_legend->setAlignment(Qt::AlignTop);
    m_chart->addLegend(m_legend);

    connect(m_annotations, SIGNAL(toggled(bool)), SLOT(annotationsToggled(bool)));
    connect(m_linesOnAnnotations, SIGNAL(toggled(bool)), SLOT(gridLinesOnAnnotationsToggled(bool)));
}

void MainWindow::annotationsToggled(bool showAnnotations)
{
    QMultiMap<qreal, QString> annotations;
    if (showAnnotations) {
        // set custom axis labels at custom positions
        annotations.insert(0.5, "Left");
        annotations.insert(3.5, "Center");
        annotations.insert(4.2, "Off Center");
        annotations.insert(6.5, "Right");
    }
    m_xAxis->setAnnotations(annotations);
    m_chart->update();
}

void MainWindow::gridLinesOnAnnotationsToggled(bool onAnnotations)
{
    // Draw grid lines where the annotations are
    auto *plane =
        static_cast<KDChart::CartesianCoordinatePlane *>(m_chart->coordinatePlane());
    KDChart::GridAttributes ga = plane->gridAttributes(Qt::Horizontal);
    ga.setLinesOnAnnotations(onAnnotations);
    plane->setGridAttributes(Qt::Horizontal, ga);
    m_chart->update();
}
