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
#include <KDChartGlobal>
#include <KDChartLineDiagram>
#include <QtTest/QtTest>

#include <TableModel.h>

using namespace KDChart;

class TestAxisOwnership : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_bars = new BarDiagram();
        m_lines = new LineDiagram();
        m_tableModel = new TableModel(this);
        m_tableModel->loadFromCSV(":/data");
        m_bars->setModel(m_tableModel);
        m_lines->setModel(m_tableModel);
        m_chart->coordinatePlane()->replaceDiagram(m_bars);
        m_chart->coordinatePlane()->addDiagram(m_lines);
    }

    void testIntialOwnership()
    {
        m_axis = new CartesianAxis(m_bars);
        QCOMPARE(m_axis->diagram(), static_cast<const AbstractDiagram *>(m_bars));
        QVERIFY(m_axis->observedBy(m_bars));
    }

    void testTakingOwnership()
    {
        m_axis = new CartesianAxis();
        m_bars->addAxis(m_axis);
        QCOMPARE(m_axis->diagram(), static_cast<const AbstractDiagram *>(m_bars));
        QVERIFY(m_axis->observedBy(m_bars));
        QVERIFY(!m_axis->observedBy(m_lines));
    }

    void testGivingUpOwnership()
    {
        m_axis = new CartesianAxis();
        m_bars->addAxis(m_axis);
        m_lines->addAxis(m_axis);
        QCOMPARE(m_axis->diagram(), static_cast<const AbstractDiagram *>(m_bars));
        QVERIFY(m_axis->observedBy(m_bars));
        QVERIFY(m_axis->observedBy(m_lines));
        m_bars->takeAxis(m_axis);
        QCOMPARE(m_axis->diagram(), static_cast<const AbstractDiagram *>(m_lines));
        QVERIFY(m_axis->observedBy(m_lines));
        QVERIFY(!m_axis->observedBy(m_bars));
    }

    void testReplacing()
    {
        m_axis = new CartesianAxis;
        m_lines->addAxis(m_axis);
        m_chart->coordinatePlane()->takeDiagram(m_bars);
        QVERIFY(m_axis->observedBy(m_lines));
        QVERIFY(!m_axis->observedBy(m_bars));
        QPointer<LineDiagram> p(m_lines);
        m_chart->coordinatePlane()->replaceDiagram(m_bars, m_lines);
        QVERIFY(!p);
        AbstractDiagram *nullDiagram = 0;
        QCOMPARE(m_axis->diagram(), static_cast<const AbstractDiagram *>(nullDiagram));
        QVERIFY(!m_axis->observedBy(m_bars));
    }

    void testReplacingWithPropagation()
    {
        initTestCase();
        m_axis = new CartesianAxis;
        m_lines->addAxis(m_axis);
        m_bars->addAxis(m_axis);
        m_chart->coordinatePlane()->takeDiagram(m_bars);
        QCOMPARE(m_axis->diagram(), static_cast<const AbstractDiagram *>(m_lines));
        QVERIFY(m_axis->observedBy(m_bars));
        QPointer<LineDiagram> p(m_lines);
        m_chart->coordinatePlane()->replaceDiagram(m_bars, m_lines);
        QVERIFY(!p);
        QCOMPARE(m_axis->diagram(), static_cast<const AbstractDiagram *>(m_bars));
        QVERIFY(m_axis->observedBy(m_bars));
    }

    void testAxisDeletion()
    {
        initTestCase();
        m_axis = new CartesianAxis;
        m_lines->addAxis(m_axis);
        CartesianAxisList list = m_lines->axes();
        QVERIFY(!list.isEmpty());
        delete m_axis;
        list = m_lines->axes();
        QVERIFY(list.isEmpty());
    }

    void testAxisDeletionAndUnregister()
    {
        initTestCase();
        m_axis = new CartesianAxis;
        m_lines->addAxis(m_axis);
        m_bars->addAxis(m_axis);
        CartesianAxisList list = m_lines->axes();
        QVERIFY(!list.isEmpty());
        delete m_axis;
        list = m_lines->axes();
        QVERIFY(list.isEmpty());
        list = m_bars->axes();
        QVERIFY(list.isEmpty());
    }

    void testFreeStanding()
    {
        initTestCase();
        m_axis = new CartesianAxis;
        m_bars->addAxis(m_axis);
        auto *bars2 = new BarDiagram;
        bars2->setModel(m_tableModel);
        m_chart->coordinatePlane()->replaceDiagram(bars2);
        m_axis->sizeHint();
    }

    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    BarDiagram *m_bars;
    LineDiagram *m_lines;
    CartesianAxis *m_axis;
    TableModel *m_tableModel;
};

QTEST_MAIN(TestAxisOwnership)

#include "main.moc"
