#include <QtTest/QtTest>

#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartPieDiagram>
#include <KDChartPieAttributes>
#include <KDChartThreeDPieAttributes>
#include <KDChartPolarDiagram>
#include <KDChartRingDiagram>

using namespace KDChart;

class TestCloning: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
        {
        }

    void testCloningBarDiagram()
        {
            BarDiagram* diagram = new BarDiagram();
            diagram->setType( BarDiagram::Stacked );
            BarDiagram* clone = diagram->clone();
            QCOMPARE( diagram->type(), clone->type() );
            // We do not clone the axes.
            QCOMPARE( clone->axes().count(), 0 );
            // And neither the reference diagram.
            QCOMPARE( clone->referenceDiagram(), (AbstractCartesianDiagram*)0 );
            QCOMPARE( clone->referenceDiagramOffset(), QPointF() );
            // And neither the plane.
            QCOMPARE( clone->coordinatePlane(), (AbstractCoordinatePlane*)0 );
            QCOMPARE( diagram->allowOverlappingDataValueTexts(), clone->allowOverlappingDataValueTexts() );
            QCOMPARE( diagram->antiAliasing(), clone->antiAliasing() );
            QCOMPARE( diagram->percentMode(), clone->percentMode() );
            QCOMPARE( diagram->datasetDimension(), clone->datasetDimension() );

            // PENDING(kalle) What could we test in the attributes model?
        }

    void testCloningLineDiagram()
        {
            LineDiagram* diagram = new LineDiagram();
            diagram->setType( LineDiagram::Percent );
            LineDiagram* clone = diagram->clone();
            QCOMPARE( diagram->type(), clone->type() );

            // the rest is already tested in testCloningBarDiagram()
        }

    void testCloningPieDiagram()
        {
            PieDiagram* diagram = new PieDiagram();
            diagram->setStartPosition( 15.0 );
            diagram->setGranularity( 1.5 );
            PieAttributes attrs;
            attrs.setExplode( true );
            attrs.setExplodeFactor( 1.5 );
            ThreeDPieAttributes threeDAttrs;
            threeDAttrs.setUseShadowColors( false );
            PieDiagram* clone = diagram->clone();
            QCOMPARE( diagram->startPosition(), clone->startPosition() );
            QCOMPARE( diagram->granularity(), clone->granularity() );
            QCOMPARE( diagram->pieAttributes(), clone->pieAttributes() );
            QCOMPARE( diagram->threeDPieAttributes(), clone->threeDPieAttributes() );

            // the rest is already tested in testCloningBarDiagram()
        }

    void testCloningPolarDiagram()
        {
            PolarDiagram* diagram = new PolarDiagram();
            diagram->setZeroDegreePosition( 5 );
            diagram->setRotateCircularLabels( true );
            diagram->setShowDelimitersAtPosition( Position::North, false );
            diagram->setShowDelimitersAtPosition( Position::South, true );
            diagram->setShowLabelsAtPosition( Position::North, true );
            diagram->setShowLabelsAtPosition( Position::South, false );
            PolarDiagram* clone = diagram->clone();
            QCOMPARE( diagram->zeroDegreePosition(), clone->zeroDegreePosition() );
            QCOMPARE( diagram->rotateCircularLabels(), clone->rotateCircularLabels() );
            QCOMPARE( diagram->showDelimitersAtPosition( Position::North ),
                      clone->showDelimitersAtPosition( Position::North ) );
            QCOMPARE( diagram->showDelimitersAtPosition( Position::South ), clone->showDelimitersAtPosition( Position::South ) );
            QCOMPARE( diagram->showLabelsAtPosition( Position::North ), clone->showLabelsAtPosition( Position::North ) );
            QCOMPARE( diagram->showLabelsAtPosition( Position::South ), clone->showLabelsAtPosition( Position::South ) );

            // the rest is already tested in testCloningBarDiagram()
        }
    void testCloningRingDiagram()
        {
            RingDiagram* diagram = new RingDiagram();
            diagram->setRelativeThickness( true );
            RingDiagram* clone = diagram->clone();
            QCOMPARE( diagram->relativeThickness(), clone->relativeThickness() );
            // the rest is already tested in testCloningBarDiagram()
            // and testCloningPieDiagram()
        }

    void cleanupTestCase()
        {
        }


private:
};

QTEST_MAIN(TestCloning)

#include "main.moc"
