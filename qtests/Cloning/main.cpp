/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <QtTest/QtTest>

#include <KDChartBarDiagram>
#include <KDChartHeaderFooter>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartPieAttributes>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartRingDiagram>
#include <KDChartThreeDBarAttributes>
#include <KDChartThreeDPieAttributes>

using namespace KDChart;

class TestCloning : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase()
    {
    }

    void testCloningBarDiagram()
    {
        auto *diagram = new BarDiagram();
        diagram->setType(BarDiagram::Stacked);
        auto *axis = new CartesianAxis;
        diagram->addAxis(axis);
        QCOMPARE(diagram->axes().count(), 1);
        BarAttributes attrs;
        attrs.setFixedDataValueGap(3.0);
        attrs.setFixedBarWidth(30.0);
        attrs.setDrawSolidExcessArrows(false);
        diagram->setBarAttributes(attrs);
        attrs.setDrawSolidExcessArrows(true);
        diagram->setBarAttributes(0, attrs);
        attrs.setDrawSolidExcessArrows(false);
        ThreeDBarAttributes threeDAttrs;
        threeDAttrs.setUseShadowColors(false);
        diagram->setThreeDBarAttributes(threeDAttrs);
        BarDiagram *clone = diagram->clone();
        QCOMPARE(diagram->type(), clone->type());
        // We do not clone the axes.
        QCOMPARE(clone->axes().count(), 0);
        // And neither the reference diagram.
        QCOMPARE(clone->referenceDiagram(), ( AbstractCartesianDiagram * )0);
        QCOMPARE(clone->referenceDiagramOffset(), QPointF());
        // And neither the plane.
        QCOMPARE(clone->coordinatePlane(), ( AbstractCoordinatePlane * )0);
        QCOMPARE(diagram->allowOverlappingDataValueTexts(), clone->allowOverlappingDataValueTexts());
        QCOMPARE(diagram->antiAliasing(), clone->antiAliasing());
        QCOMPARE(diagram->percentMode(), clone->percentMode());
        QCOMPARE(diagram->datasetDimension(), clone->datasetDimension());
        QCOMPARE(diagram->barAttributes(), clone->barAttributes());
        QCOMPARE(diagram->threeDBarAttributes(), clone->threeDBarAttributes());

        QVERIFY(diagram->attributesModel() != clone->attributesModel());
    }

    void testCloningLineDiagram()
    {
        auto *diagram = new LineDiagram();
        diagram->setType(LineDiagram::Percent);
        LineAttributes attrs;
        attrs.setMissingValuesPolicy(LineAttributes::MissingValuesShownAsZero);
        diagram->setLineAttributes(attrs);
        LineDiagram *clone = diagram->clone();
        QCOMPARE(diagram->type(), clone->type());
        QCOMPARE(diagram->lineAttributes(), clone->lineAttributes());

        // the rest is already tested in testCloningBarDiagram()
    }

    void testCloningPieDiagram()
    {
        // commenting those tests - Deprecated method
        // will make new test for that in PolarCoordinatePlane
        // do we want the warning ?
        // if yes - we just need to un-comment
        auto *diagram = new PieDiagram();
        // diagram->coordinatePlane()->setStartPosition( 15.0 );
        diagram->setGranularity(1.5);
        PieAttributes attrs;
        attrs.setExplode(true);
        attrs.setExplodeFactor(1.5);
        ThreeDPieAttributes threeDAttrs;
        threeDAttrs.setUseShadowColors(false);
        PieDiagram *clone = diagram->clone();
        // QCOMPARE( diagram->startPosition(), clone->startPosition() );
        QCOMPARE(diagram->granularity(), clone->granularity());
        QCOMPARE(diagram->pieAttributes(), clone->pieAttributes());
        QCOMPARE(diagram->threeDPieAttributes(), clone->threeDPieAttributes());

        // the rest is already tested in testCloningBarDiagram()
    }

    void testCloningPolarDiagram()
    {
        // commenting those tests - Deprecated method
        // will make new test for that in PolarCoordinatePlane
        // do we want the warning ?
        // if yes - we just need to un-comment
        auto *diagram = new PolarDiagram();
        // diagram->setZeroDegreePosition( 5 );
        diagram->setRotateCircularLabels(true);
        diagram->setShowDelimitersAtPosition(Position::North, false);
        diagram->setShowDelimitersAtPosition(Position::South, true);
        diagram->setShowLabelsAtPosition(Position::North, true);
        diagram->setShowLabelsAtPosition(Position::South, false);
        PolarDiagram *clone = diagram->clone();
        // QCOMPARE( diagram->zeroDegreePosition(), clone->zeroDegreePosition() );
        QCOMPARE(diagram->rotateCircularLabels(), clone->rotateCircularLabels());
        QCOMPARE(diagram->showDelimitersAtPosition(Position::North),
                 clone->showDelimitersAtPosition(Position::North));
        QCOMPARE(diagram->showDelimitersAtPosition(Position::South), clone->showDelimitersAtPosition(Position::South));
        QCOMPARE(diagram->showLabelsAtPosition(Position::North), clone->showLabelsAtPosition(Position::North));
        QCOMPARE(diagram->showLabelsAtPosition(Position::South), clone->showLabelsAtPosition(Position::South));

        // the rest is already tested in testCloningBarDiagram()
    }
    void testCloningRingDiagram()
    {
        auto *diagram = new RingDiagram();
        diagram->setRelativeThickness(true);
        RingDiagram *clone = diagram->clone();
        QCOMPARE(diagram->relativeThickness(), clone->relativeThickness());
        // the rest is already tested in testCloningBarDiagram()
        // and testCloningPieDiagram()
    }

    void testCloningHeaderFooter()
    {
        auto *headerFooter = new HeaderFooter();
        headerFooter->setType(HeaderFooter::Footer);
        TextAttributes attrs;
        attrs.setPen(QPen(Qt::red));
        headerFooter->setTextAttributes(attrs);
        HeaderFooter *clone = headerFooter->clone();
        QCOMPARE(headerFooter->type(), clone->type());
        QCOMPARE(headerFooter->textAttributes(), clone->textAttributes());
    }

    void testCloningLegends()
    {
        auto *legend = new Legend();
        TextAttributes attrs;
        attrs.setPen(QPen(Qt::red));
        legend->setTextAttributes(attrs);
        legend->setShowLines(true);
        legend->setPosition(Position::North);
        Legend *clone = legend->clone();
        QCOMPARE(legend->textAttributes(), clone->textAttributes());
        QCOMPARE(legend->showLines(), clone->showLines());
        QCOMPARE(legend->position(), clone->position());
    }

    void cleanupTestCase()
    {
    }

private:
};

QTEST_MAIN(TestCloning)

#include "main.moc"
