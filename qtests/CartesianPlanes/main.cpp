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
#include <KDChartGridAttributes>
#include <KDChartPlotter>
#include <QPair>
#include <QPointF>
#include <QStandardItemModel>
#include <QString>
#include <QtTest/QtTest>

using namespace KDChart;

class NumericDataModel : public QStandardItemModel
{
    Q_OBJECT
public:
    void setYValues(const QList<qreal> &values)
    {
        min = QPointF(0, 1e9);
        max = QPointF(values.size() - 1, -1e9);
        setRowCount(values.size());
        setColumnCount(1);
        for (int i = 0; i < values.size(); i++) {
            min.setY(qMin(min.y(), values[i]));
            max.setY(qMax(max.y(), values[i]));

            QModelIndex idx = index(i, 0);
            QStandardItemModel::setData(idx, values[i]);
        }
    }

    void setXyValues(const QList<QPointF> &values)
    {
        min = QPointF(1e9, 1e9);
        max = QPointF(-1e9, -1e9);
        setRowCount(values.size());
        setColumnCount(2);
        for (int i = 0; i < values.size(); i++) {
            min.setX(qMin(min.x(), values[i].x()));
            max.setX(qMax(max.x(), values[i].x()));
            min.setY(qMin(min.y(), values[i].y()));
            max.setY(qMax(max.y(), values[i].y()));

            QModelIndex idx = index(i, 0);
            QStandardItemModel::setData(idx, values[i].x());
            idx = index(i, 1);
            QStandardItemModel::setData(idx, values[i].y());
        }
    }

    QPointF min;
    QPointF max;
};

class TestCartesianPlanes : public QObject
{
    Q_OBJECT
private slots:

    void init();
    void cleanup();
    void testIntialOwnership();
    void testDiagramOwnership();
    void testIsometricScalingSettings();
    void testZoomFactorsSettings();
    void testRangeSettingsBars();
    void testRangeSettingsPlotter();
    void testGlobalGridAttributesSettings();
    void testGridAttributesSettings();
    void testAxesCalcModesSettings();

private:
    void doTestRangeSettings(AbstractCartesianDiagram *diagram, const QPointF &min, const QPointF &max);

    Chart *m_chart;
    BarDiagram *m_bars;
    Plotter *m_plotter;
    CartesianCoordinatePlane *m_plane;
    NumericDataModel *m_model;
};

void TestCartesianPlanes::init()
{
    m_chart = new Chart(0);
    m_model = new NumericDataModel();
    m_model->setParent(m_chart);
    m_bars = new BarDiagram(m_chart);
    m_bars->setModel(m_model);
    m_plotter = new Plotter(m_chart);
    m_plotter->setModel(m_model);
    m_plane = new CartesianCoordinatePlane(m_chart);
    m_chart->addCoordinatePlane(m_plane);
    m_plane->setReferenceCoordinatePlane(m_chart->coordinatePlane());
    qDebug() << m_plotter->datasetDimension();
}

void TestCartesianPlanes::cleanup()
{
    delete m_chart;
}

void TestCartesianPlanes::testIntialOwnership()
{
    AbstractCoordinatePlane *plane = m_chart->coordinatePlane();
    QCOMPARE(m_plane->referenceCoordinatePlane(), m_chart->coordinatePlane());
    m_chart->takeCoordinatePlane(0);
    delete plane;
    QCOMPARE(m_plane->referenceCoordinatePlane(), ( AbstractCoordinatePlane * )0);
}

void TestCartesianPlanes::testDiagramOwnership()
{
    m_plane->addDiagram(m_bars);
    QCOMPARE(m_plane->diagrams().size(), 1);
    m_plane->addDiagram(m_plotter);
    QCOMPARE(m_plane->diagrams().size(), 2);
    QCOMPARE(dynamic_cast<BarDiagram *>(m_plane->diagram()), m_bars);
    m_plane->takeDiagram(m_bars);
    QCOMPARE(m_plane->diagrams().size(), 1);
    QCOMPARE(dynamic_cast<Plotter *>(m_plane->diagram()), m_plotter);
    m_plane->replaceDiagram(m_bars, m_plotter);
    QCOMPARE(m_plane->diagrams().size(), 1);
    QCOMPARE(dynamic_cast<BarDiagram *>(m_plane->diagram()), m_bars);
    m_plane->takeDiagram(m_bars);
    QCOMPARE(m_plane->diagrams().size(), 0);
    delete m_bars;
}

void TestCartesianPlanes::testIsometricScalingSettings()
{
    QVERIFY(m_plane->doesIsometricScaling() == false);
    m_plane->setIsometricScaling(true);
    QVERIFY(m_plane->doesIsometricScaling() == true);
}

void TestCartesianPlanes::testZoomFactorsSettings()
{
    QCOMPARE(m_plane->zoomFactorX(), 1.0);
    QCOMPARE(m_plane->zoomFactorY(), 1.0);
    QCOMPARE(m_plane->zoomCenter(), QPointF(0.5, 0.5));
    m_plane->setZoomFactorX(1.5);
    m_plane->setZoomFactorY(1.5);
    m_plane->setZoomCenter(QPointF(1.0, 1.0));
    QCOMPARE(m_plane->zoomFactorX(), 1.5);
    QCOMPARE(m_plane->zoomFactorY(), 1.5);
    QCOMPARE(m_plane->zoomCenter(), QPointF(1.0, 1.0));
}

void TestCartesianPlanes::doTestRangeSettings(AbstractCartesianDiagram *diagram,
                                              const QPointF &min, const QPointF &max)
{
    m_plane->addDiagram(diagram);
    // the range is null when auto adjustment is turned off - check that...
    m_plane->setAutoAdjustHorizontalRangeToData(100);
    m_plane->setAutoAdjustVerticalRangeToData(100);

    {
        const QPair<qreal, qreal> hrange = m_plane->horizontalRange();
        const QPair<qreal, qreal> vrange = m_plane->verticalRange();
        QCOMPARE(hrange.first, qreal(0.0));
        QCOMPARE(hrange.second, qreal(0.0));
        QCOMPARE(vrange.first, qreal(0.0));
        QCOMPARE(vrange.second, qreal(0.0));
    }

    // now check that auto adjustment works when enabled
    m_plane->setAutoAdjustHorizontalRangeToData(67);
    m_plane->setAutoAdjustVerticalRangeToData(67);
    {
        const QPair<qreal, qreal> hrange = m_plane->horizontalRange();
        const QPair<qreal, qreal> vrange = m_plane->verticalRange();
        QCOMPARE(hrange.first, min.x());
        QCOMPARE(hrange.second, max.x());
        QCOMPARE(vrange.first, min.y());
        QCOMPARE(vrange.second, max.y());
    }

    {
        QPair<qreal, qreal> hboundaries(diagram->dataBoundaries().first.x(),
                                        diagram->dataBoundaries().second.x());
        QPair<qreal, qreal> vboundaries(int(diagram->dataBoundaries().first.y() + 0.5),
                                        int(diagram->dataBoundaries().second.y() + 0.5));
        m_plane->setHorizontalRange(hboundaries);
        m_plane->setVerticalRange(vboundaries);
        const QPair<qreal, qreal> newhb = m_plane->horizontalRange();
        const QPair<qreal, qreal> newvb = m_plane->verticalRange();
        QCOMPARE(newhb.first, hboundaries.first);
        QCOMPARE(newhb.second, hboundaries.second);
        QCOMPARE(newvb.first, vboundaries.first);
        QCOMPARE(newvb.second, vboundaries.second);
    }
}

void TestCartesianPlanes::testRangeSettingsBars()
{
    QList<qreal> points;
    points << 40 << 45 << 42 << 34 << 34;
    m_model->setYValues(points);
    // data point "0" is shown at "0.5" in bar diagrams, which requires some data range hackery.
    // we correct for that like so:
    m_model->max.rx() += 1;

    doTestRangeSettings(m_bars, m_model->min, m_model->max);
}

void TestCartesianPlanes::testRangeSettingsPlotter()
{
    QList<QPointF> points;
    points << QPointF(0.0, 40.0) << QPointF(1.0, 45.0) << QPointF(2.0, 42.0)
           << QPointF(3.0, 34.0) << QPointF(4.0, 34.0);
    m_model->setXyValues(points);
    doTestRangeSettings(m_plotter, m_model->min, m_model->max);
}

void TestCartesianPlanes::testGlobalGridAttributesSettings()
{
    GridAttributes ga = m_plane->globalGridAttributes();
    QVERIFY(ga.isGridVisible() == true);
    ga.setGridVisible(false);
    m_plane->setGlobalGridAttributes(ga);
    QVERIFY(m_plane->globalGridAttributes().isGridVisible() == false);
    // reset to normal
    ga.setGridVisible(true);
    QVERIFY(m_plane->globalGridAttributes().isGridVisible() == false);
    m_plane->setGlobalGridAttributes(ga);
    QVERIFY(m_plane->globalGridAttributes().isGridVisible() == true);
}

void TestCartesianPlanes::testGridAttributesSettings()
{
    GridAttributes gh = m_plane->gridAttributes(Qt::Horizontal);
    GridAttributes gv = m_plane->gridAttributes(Qt::Vertical);
    QVERIFY(gh.isGridVisible() == true);
    QVERIFY(gv.isGridVisible() == true);
    gh.setGridVisible(false);
    m_plane->setGridAttributes(Qt::Horizontal, gh);
    QVERIFY(m_plane->hasOwnGridAttributes(Qt::Horizontal) == true);
    QVERIFY(m_plane->hasOwnGridAttributes(Qt::Vertical) == false);
    QVERIFY(m_plane->gridAttributes(Qt::Horizontal).isGridVisible() == false);
    QVERIFY(m_plane->gridAttributes(Qt::Vertical).isGridVisible() == true);
    gv.setGridVisible(false);
    m_plane->setGridAttributes(Qt::Vertical, gv);
    QVERIFY(m_plane->hasOwnGridAttributes(Qt::Horizontal) == true);
    QVERIFY(m_plane->hasOwnGridAttributes(Qt::Vertical) == true);
    QVERIFY(m_plane->gridAttributes(Qt::Horizontal).isGridVisible() == false);
    QVERIFY(m_plane->gridAttributes(Qt::Vertical).isGridVisible() == false);
    m_plane->resetGridAttributes(Qt::Horizontal);
    m_plane->resetGridAttributes(Qt::Vertical);
    QVERIFY(m_plane->gridAttributes(Qt::Horizontal).isGridVisible() == true);
    QVERIFY(m_plane->gridAttributes(Qt::Vertical).isGridVisible() == true);
    QVERIFY(m_plane->hasOwnGridAttributes(Qt::Horizontal) == false);
    QVERIFY(m_plane->hasOwnGridAttributes(Qt::Vertical) == false);
}

void TestCartesianPlanes::testAxesCalcModesSettings()
{
    QCOMPARE(m_plane->axesCalcModeX(), AbstractCoordinatePlane::Linear);
    QCOMPARE(m_plane->axesCalcModeY(), AbstractCoordinatePlane::Linear);
    m_plane->setAxesCalcModes(AbstractCoordinatePlane::Logarithmic);
    QCOMPARE(m_plane->axesCalcModeX(), AbstractCoordinatePlane::Logarithmic);
    QCOMPARE(m_plane->axesCalcModeY(), AbstractCoordinatePlane::Logarithmic);
    m_plane->setAxesCalcModeX(AbstractCoordinatePlane::Linear);
    QCOMPARE(m_plane->axesCalcModeX(), AbstractCoordinatePlane::Linear);
    QCOMPARE(m_plane->axesCalcModeY(), AbstractCoordinatePlane::Logarithmic);
    m_plane->setAxesCalcModeY(AbstractCoordinatePlane::Linear);
    QCOMPARE(m_plane->axesCalcModeX(), AbstractCoordinatePlane::Linear);
    QCOMPARE(m_plane->axesCalcModeY(), AbstractCoordinatePlane::Linear);
}

QTEST_MAIN(TestCartesianPlanes)

#include "main.moc"
