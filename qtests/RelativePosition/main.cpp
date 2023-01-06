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

#include <QLabel>

#include <KDChartGlobal>
#include <KDChartMeasure>
#include <KDChartPosition>
#include <KDChartRelativePosition>

using namespace KDChart;

class TestRelativePosition : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_window = new QLabel(0);
        m_window->setGeometry(100, 100, 300, 200);
        m_window->show();
    }

    void testRelativePositionHRelativeVAbsolute()
    {
        const qreal horizPerMille = 100.0;
        KDChart::Measure mHoriz(
            horizPerMille,
            KDChartEnums::MeasureCalculationModeRelative,
            KDChartEnums::MeasureOrientationHorizontal);
        mHoriz.setReferenceArea(m_window);

        const qreal vertAbsolute = -50.0;
        KDChart::Measure mVert(
            vertAbsolute,
            KDChartEnums::MeasureCalculationModeAbsolute);

        KDChart::RelativePosition relPos;
        relPos.setReferenceArea(m_window);
        relPos.setReferencePosition(KDChart::Position::NorthWest);
        relPos.setHorizontalPadding(mHoriz);
        relPos.setVerticalPadding(mVert);

        // no auto mode is active, so we pass an empty QSizeF:
        const QPointF pt(relPos.calculatedPoint(QSizeF()));
        const QRectF geo(m_window->geometry());
        QCOMPARE(pt.x(), geo.topLeft().x() + geo.size().width() / 1000.0 * horizPerMille);
        QCOMPARE(pt.y(), geo.topLeft().y() + vertAbsolute);
    }

    void testRelativePositionHAbsoluteVRelative()
    {
        const qreal horizAbsolute = 100.0;
        KDChart::Measure mHoriz(
            horizAbsolute,
            KDChartEnums::MeasureCalculationModeAbsolute);

        const qreal vertRelative = -50.0;
        KDChart::Measure mVert(
            vertRelative,
            KDChartEnums::MeasureCalculationModeRelative,
            KDChartEnums::MeasureOrientationVertical);
        mVert.setReferenceArea(m_window);

        KDChart::RelativePosition relPos;
        relPos.setReferenceArea(m_window);
        relPos.setReferencePosition(KDChart::Position::Center);
        relPos.setHorizontalPadding(mHoriz);
        relPos.setVerticalPadding(mVert);

        // no auto mode is active, so we pass an empty QSizeF:
        const QPointF pt(relPos.calculatedPoint(QSizeF()));
        const QRectF geo(m_window->geometry());
        QCOMPARE(pt.x(), geo.center().x() + horizAbsolute);
        QCOMPARE(pt.y(), geo.center().y() + geo.size().height() / 1000.0 * vertRelative);
    }

    void cleanupTestCase()
    {
    }

private:
    QLabel *m_window;
};

QTEST_MAIN(TestRelativePosition)

#include "main.moc"
