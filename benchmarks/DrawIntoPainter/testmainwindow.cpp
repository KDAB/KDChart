/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <KDChartChart>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartLineDiagram>
#include <KDChartLineAttributes>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartThreeDLineAttributes>
#include <KDChartMarkerAttributes>
#include <KDChartFrameAttributes>
#include <KDChartBackgroundAttributes>
#include <KDChartLegend>

#include <QDebug>
#include <QPainter>
#include <QTime>

#include <iostream>
#include "testmainwindow.h"

using namespace std;

// keep drawIntoPixmap in sync with ../../examples/DrawIntoPixmap/mainwindow.cpp

static QPixmap drawIntoPixmap( const QSize& size, KDChart::Chart* chart )
{
    QPixmap pix( size );
    pix.fill( Qt::white );
    QPainter painter( &pix );
    chart->paint( &painter, QRect( 0, 0, size.width(), size.height() ) );
    return pix;
}

TestMainWindow::TestMainWindow( MainWindow* parent ) :
    MainWindow( parent )
{
}

void TestMainWindow::testLab(int count, int row, DynamicTableModel *measureModel)
{

    QTime t; t.start();
		DynamicTableModel m_model(6);

		// measure setData()
		for(int i = 0; i < count; i++) {
			for(int j = 0; j < 6; j++) {
//  		m_model.setData(index, QVariant(TEST_VALUE), (int) Qt::EditRole);
				m_model.appendData(QVariant(TEST_VALUE), i, j);
			}
		}
 		measureModel->appendData(QVariant(t.elapsed()), row, 0);
    t.restart();

		// measure setModel()
    m_lines->setModel( &m_model );
 		measureModel->appendData(QVariant(t.elapsed()), row, 2);
    t.restart();

		// measure drawIntoPixmap
    QSize size1 = QSize( 200, 200 );
    m_pix1 = drawIntoPixmap( size1, m_chart );
 		measureModel->appendData(QVariant(t.elapsed()), row, 1);
    t.restart();

		// measure show()
    m_smallChart1->setPixmap( m_pix1 );
    m_smallChart1->show();
 		measureModel->appendData(QVariant(t.elapsed()), row, 3);

}
