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
#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartMeasure>
#include <QtTest/QtTest>

#include <TableModel.h>

using namespace KDChart;

class TestMeasure : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_model = new TableModel(this);
        m_model->loadFromCSV(":/data");
        m_bars = new BarDiagram();
        m_bars->setModel(m_model);
        m_chart->coordinatePlane()->replaceDiagram(m_bars);
        m_chart->setGeometry(100, 100, 300, 200);
        m_chart->show();
    }

    void testMeasureCalculationModeAbsolute()
    {
        KDChart::Measure m(100.0);
        m.setCalculationMode(KDChartEnums::MeasureCalculationModeAbsolute);
        QCOMPARE(
            m.calculatedValue(0, KDChartEnums::MeasureOrientationAuto),
            100.0);
    }

    void testMeasureCalculationModeRelativeHorizontalOrientation()
    {
        KDChart::Measure m(
            100.0,
            KDChartEnums::MeasureCalculationModeRelative,
            KDChartEnums::MeasureOrientationHorizontal);
        m.setReferenceArea(m_chart);
        QCOMPARE(
            m.calculatedValue(0, KDChartEnums::MeasureOrientationAuto),
            m_chart->size().width() / 10.0);
    }

    void testMeasureCalculationModeRelativeVerticalOrientation()
    {
        KDChart::Measure m(
            100.0,
            KDChartEnums::MeasureCalculationModeRelative,
            KDChartEnums::MeasureOrientationVertical);
        m.setReferenceArea(m_chart);
        QCOMPARE(
            m.calculatedValue(0, KDChartEnums::MeasureOrientationAuto),
            m_chart->size().height() / 10.0);
    }

    void testMeasureCalculationModeAuto()
    {
        KDChart::Measure m(100.0);
        m.setCalculationMode(KDChartEnums::MeasureCalculationModeAuto);
        QCOMPARE(
            m.calculatedValue(m_chart, KDChartEnums::MeasureOrientationHorizontal),
            m_chart->size().width() / 10.0);
        QCOMPARE(
            m.calculatedValue(m_chart, KDChartEnums::MeasureOrientationVertical),
            m_chart->size().height() / 10.0);
        QCOMPARE(
            m.calculatedValue(m_chart, KDChartEnums::MeasureOrientationMinimum),
            qMin(m_chart->size().width(), m_chart->size().height()) / 10.0);
        QCOMPARE(
            m.calculatedValue(m_chart, KDChartEnums::MeasureOrientationMaximum),
            qMax(m_chart->size().width(), m_chart->size().height()) / 10.0);
    }

    void testMeasureCalculationModeAutoArea()
    {
        KDChart::Measure m(
            100.0,
            KDChartEnums::MeasureCalculationModeAutoArea,
            KDChartEnums::MeasureOrientationHorizontal);
        QCOMPARE(
            m.calculatedValue(m_chart, KDChartEnums::MeasureOrientationAuto),
            m_chart->size().width() / 10.0);
        m.setReferenceOrientation(KDChartEnums::MeasureOrientationVertical);
        QCOMPARE(
            m.calculatedValue(m_chart, KDChartEnums::MeasureOrientationAuto),
            m_chart->size().height() / 10.0);
        m.setReferenceOrientation(KDChartEnums::MeasureOrientationMinimum);
        QCOMPARE(
            m.calculatedValue(m_chart, KDChartEnums::MeasureOrientationAuto),
            qMin(m_chart->size().width(), m_chart->size().height()) / 10.0);
        m.setReferenceOrientation(KDChartEnums::MeasureOrientationMaximum);
        QCOMPARE(
            m.calculatedValue(m_chart, KDChartEnums::MeasureOrientationAuto),
            qMax(m_chart->size().width(), m_chart->size().height()) / 10.0);
    }

    void testMeasureAutoOrientation()
    {
        KDChart::Measure m(
            100.0,
            KDChartEnums::MeasureCalculationModeAutoOrientation,
            KDChartEnums::MeasureOrientationAuto);
        m.setReferenceArea(m_chart);
        QCOMPARE(
            m.calculatedValue(0, KDChartEnums::MeasureOrientationHorizontal),
            m_chart->size().width() / 10.0);
        QCOMPARE(
            m.calculatedValue(0, KDChartEnums::MeasureOrientationVertical),
            m_chart->size().height() / 10.0);
        QCOMPARE(
            m.calculatedValue(0, KDChartEnums::MeasureOrientationMinimum),
            qMin(m_chart->size().width(), m_chart->size().height()) / 10.0);
        QCOMPARE(
            m.calculatedValue(0, KDChartEnums::MeasureOrientationMaximum),
            qMax(m_chart->size().width(), m_chart->size().height()) / 10.0);
    }

    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    BarDiagram *m_bars;
    TableModel *m_model;
};

QTEST_MAIN(TestMeasure)

#include "main.moc"
