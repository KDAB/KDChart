/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include <QtTest/QtTest>

#include <KDChartWidget>
#include <KDChartGlobal>
//#include <KDChartBarDiagram>
//#include <KDChartLineDiagram>
//#include <KDChartCartesianCoordinatePlane>
//#include <KDChartPolarCoordinatePlane>
#include <KDChartLegend>
//#include <KDChartHeaderFooter>

using namespace KDChart;

class TestWidgetElementOwnership: public QObject {
  Q_OBJECT
private slots:

  void initTestCase()
  {
      m_widget = new Widget(0);
      QVector< qreal > vec0;
      vec0.append( -2 );
      vec0.append( -1 );
      vec0.append( 0 );
      vec0.append( 1 );
      vec0.append( 2 );
      m_widget->setDataset( 0, vec0, "Linear" );

      QVector< qreal > vec1;
      vec1.append( -4 );
      vec1.append( -2 );
      vec1.append( 0 );
      vec1.append( 2 );
      vec1.append( 4 );
      m_widget->setDataset( 1, vec1, "Quadratic" );

      QVector< qreal > vec2;
      vec2.append( -8 );
      vec2.append( -2 );
      vec2.append( 0 );
      vec2.append( 2 );
      vec2.append( 8 );
      m_widget->setDataset( 2, vec2, "Cubic" );

  }

  void testPadding()
  {
      QVERIFY( m_widget->globalLeadingLeft() == false );
      m_widget->setGlobalLeading( 2, 2, 2, 2 );
      QCOMPARE( m_widget->globalLeadingLeft(), 2 );
      QCOMPARE( m_widget->globalLeadingTop(), 2 );
      QCOMPARE( m_widget->globalLeadingRight(), 2 );
      QCOMPARE( m_widget->globalLeadingBottom(), 2 );
      m_widget->setGlobalLeadingLeft( 5 );
      QCOMPARE( m_widget->globalLeadingLeft(), 5 );
      QCOMPARE( m_widget->globalLeadingTop(), 2 );
      QCOMPARE( m_widget->globalLeadingRight(), 2 );
      QCOMPARE( m_widget->globalLeadingBottom(), 2 );
      m_widget->setGlobalLeadingTop( 5 );
      QCOMPARE( m_widget->globalLeadingLeft(), 5 );
      QCOMPARE( m_widget->globalLeadingTop(), 5 );
      QCOMPARE( m_widget->globalLeadingRight(), 2 );
      QCOMPARE( m_widget->globalLeadingBottom(), 2 );
      m_widget->setGlobalLeadingRight( 5 );
      QCOMPARE( m_widget->globalLeadingLeft(), 5 );
      QCOMPARE( m_widget->globalLeadingTop(), 5 );
      QCOMPARE( m_widget->globalLeadingRight(), 5 );
      QCOMPARE( m_widget->globalLeadingBottom(), 2 );
      m_widget->setGlobalLeadingBottom( 5 );
      QCOMPARE( m_widget->globalLeadingLeft(), 5 );
      QCOMPARE( m_widget->globalLeadingTop(), 5 );
      QCOMPARE( m_widget->globalLeadingRight(), 5 );
      QCOMPARE( m_widget->globalLeadingBottom(), 5 );
      m_widget->setGlobalLeading( 2, 2, 2, 2 );
      QCOMPARE( m_widget->globalLeadingLeft(), 2 );
      QCOMPARE( m_widget->globalLeadingTop(), 2 );
      QCOMPARE( m_widget->globalLeadingRight(), 2 );
      QCOMPARE( m_widget->globalLeadingBottom(), 2 );
  }

  void testHeaderFooterOwnership()
  {
      QCOMPARE( m_widget->allHeadersFooters().size(), 0 );
      HeaderFooter * h = new HeaderFooter();
      HeaderFooter * h1 = new HeaderFooter();
      m_widget->addHeaderFooter( h );
      m_widget->addHeaderFooter( h1 );
      QCOMPARE( m_widget->allHeadersFooters().size(), 2 );
      QCOMPARE( m_widget->firstHeaderFooter(),  h );
      m_widget->takeHeaderFooter( h );
      QCOMPARE( m_widget->allHeadersFooters().size(), 1 );
      QCOMPARE( m_widget->firstHeaderFooter(),  h1 );
      m_widget->replaceHeaderFooter( h ,  h1 );
      QCOMPARE( m_widget->allHeadersFooters().size(), 1 );
      delete h;
      QCOMPARE( m_widget->allHeadersFooters().size(), 0 );
  }

  void testLegendOwnerShip()
  {
      // check no legend
      QCOMPARE( m_widget->allLegends().size(), 0 );
      // check add legend - take legend - delete legend
      m_widget->addLegend( Position::North );
      QCOMPARE( m_widget->allLegends().size(), 1 );
      Legend* legend = m_widget->legend();
      m_widget->takeLegend( legend );
      QCOMPARE( m_widget->allLegends().size(), 0 );
      m_widget->replaceLegend( legend );
      QCOMPARE( m_widget->allLegends().size(), 1 );
      delete legend;
      QCOMPARE( m_widget->allLegends().size(), 0 );
   }

  void testRetrieveDiagram()
  {
      //set Cartesian type
      m_widget->setType( Widget::Line );
      QCOMPARE( m_widget->type(), Widget::Line );
      QVERIFY( m_widget->barDiagram() == false );
      QVERIFY( m_widget->pieDiagram() == false );
      QVERIFY( m_widget->ringDiagram() == false );
      QVERIFY( m_widget->polarDiagram() == false );
      // set Polar type
      m_widget->setType( Widget::Polar );
      QCOMPARE( m_widget->type(), Widget::Polar );
      QVERIFY( m_widget->barDiagram() == false );
      QVERIFY( m_widget->lineDiagram() == false );
      QVERIFY( m_widget->ringDiagram() == false );
      QVERIFY( m_widget->pieDiagram() == false );
      // reset default
      m_widget->setType( Widget::Line );
  }

  void testTypeChangeWithLegend()
  {
      //set Cartesian type
      m_widget->addLegend( Position::North );
      QCOMPARE( m_widget->allLegends().size(), 1 );
      m_widget->setType( Widget::Bar, Widget::Stacked );
      Legend *legend = m_widget->legend();
      QVERIFY( legend != 0 );
      QVERIFY( legend->diagram() != 0 );
      QCOMPARE( legend->diagram(), m_widget->diagram() );
      QCOMPARE( m_widget->allLegends().size(), 1 );
      m_widget->setType( Widget::Line );
  }


  void testDiagramTypeSubType()
  {
      m_widget->setType( Widget::Line );
      QCOMPARE( m_widget->type(), Widget::Line );
      QCOMPARE( m_widget->subType(),  Widget::Normal );
      //check type subtype = default
      m_widget->setType( Widget::Bar );
      QCOMPARE( m_widget->type(), Widget::Bar );
      QCOMPARE( m_widget->subType(),  Widget::Normal );
      //check type subtype
      m_widget->setType( Widget::Line,  Widget::Stacked );
      QCOMPARE( m_widget->type(), Widget::Line );
      QCOMPARE( m_widget->subType(),  Widget::Stacked );
      //check subtype
      m_widget->setSubType( Widget::Percent );
      QCOMPARE( m_widget->subType(),  Widget::Percent );
  }

  void testPlaneOwnership()
  {
      // check plane
      AbstractCoordinatePlane*orig = m_widget->coordinatePlane();
      QCOMPARE( m_widget->coordinatePlane(), orig );
  }

  void testWidgetDeletion()
  {
      delete m_widget;
  }

  void cleanupTestCase()
  {

  }

private:
  Widget *m_widget;

};

QTEST_MAIN(TestWidgetElementOwnership)

#include "main.moc"
