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

#include <QtGui/QtGui>
#include <QApplication>
#include <KDChart/KDChartChart>
#include <KDChart/KDChartBarDiagram>


//class ChartWidget : public QWidget {
//  Q_OBJECT
//public:
//  explicit ChartWidget(QWidget* parent=0)
//    : QWidget(parent)
//  {

//    m_model.insertRows( 0, 2, QModelIndex() );
//    m_model.insertColumns(  0,  3,  QModelIndex() );
//    for (int row = 0; row < 3; ++row) {
//            for (int column = 0; column < 3; ++column) {
//                QModelIndex index = m_model.index(row, column, QModelIndex());
//                m_model.setData(index, QVariant(row+1 * column) );
//            }
//    }

//    KDChart::BarDiagram* diagram = new KDChart::BarDiagram;
//    diagram->setModel(&m_model);

//    m_chart.coordinatePlane()->replaceDiagram(diagram);

//    QVBoxLayout* l = new QVBoxLayout(this);
//    l->addWidget(&m_chart);
//    setLayout(l);
//    m_timer = new QTimer(this);
//    connect( m_timer, SIGNAL( timeout() ),
//             this, SLOT( slotTimeout() ) );
//    m_timer->start( 200 );
//  }

//private slots:
//      void slotTimeout() {
//          QModelIndex index = m_model.index( 0, 1, QModelIndex());
//          qreal value = ( m_model.data( index ).toInt() % 24 ) +1;
//          m_model.setData( index, value );
//      }

//private:
//  KDChart::Chart m_chart;
//  QStandardItemModel m_model;
//  QTimer *m_timer;
//};

#include "mainwindow.h"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    //ChartWidget w;
    MainWindow w;
    w.show();

    return app.exec();
}

//#include "main.moc"
