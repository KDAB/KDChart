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

#include <QtGui>
#include <KDChartChart>
#include <KDChartLineDiagram>
#include <QApplication>

using namespace KDChart;

class ChartWidget : public QWidget {
  Q_OBJECT
public:

  explicit ChartWidget(QWidget* parent=0)
    : QWidget(parent)
  {

    m_model.insertRows( 0,6, QModelIndex() );
    m_model.insertColumns( 0,1, QModelIndex() );

    m_model.setData( m_model.index( 0, 0, QModelIndex()),  15);
    m_model.setData( m_model.index( 1, 0, QModelIndex()),  11);
    m_model.setData( m_model.index( 2, 0, QModelIndex()),  7);
    m_model.setData( m_model.index( 3, 0, QModelIndex()),  3);
    m_model.setData( m_model.index( 4, 0, QModelIndex()),  -1);
    m_model.setData( m_model.index( 5, 0, QModelIndex()),  -5);

    LineDiagram* diagram = new LineDiagram;
    diagram->setModel(&m_model);

    m_chart.coordinatePlane()->replaceDiagram(diagram);

    // paint the areas in a few cells
    // using different brushes
    LineAttributes la3(diagram->lineAttributes(m_model.index(3,0,QModelIndex())));
    la3.setDisplayArea(  true );
    la3.setTransparency( 150 );
    diagram->setBrush(m_model.index(1,0,QModelIndex()),QBrush(Qt::green));
    diagram->setLineAttributes( m_model.index( 1, 0,  QModelIndex()),la3);
    diagram->setBrush(m_model.index(3,0,QModelIndex()),QBrush(Qt::yellow));
    diagram->setLineAttributes(m_model.index(3,0,QModelIndex()),la3);
    diagram->setBrush(m_model.index(4,0,QModelIndex()),QBrush(Qt::red));
    diagram->setLineAttributes(m_model.index(4,0,QModelIndex()),la3);

    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    setLayout(l);
  }

private:
  Chart m_chart;
  QStandardItemModel m_model;
};

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
