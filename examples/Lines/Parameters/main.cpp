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
#include <KDChartDataValueAttributes>
#include <QApplication>

using namespace KDChart;

class ChartWidget : public QWidget {
  Q_OBJECT
public:
  explicit ChartWidget(QWidget* parent=0)
    : QWidget(parent)
  {

    m_model.insertRows( 0,5, QModelIndex() );
    m_model.insertColumns( 0,5, QModelIndex() );
    qreal increase = 1.15;
    for ( int i = 0; i < 5; ++i ) {
        increase += 0.06;
        for ( int j = 0; j < 5; ++j ) {
            m_model.setData( m_model.index( i,j,QModelIndex() ),
                             ( increase + i ) * j * (1.0 + 0.1 * (rand() % 10)) );
        }
    }

    LineDiagram* diagram = new LineDiagram;
    diagram->setModel(&m_model);

    // Display values
    // 1 - Call the relevant attributes
    DataValueAttributes dva( diagram->dataValueAttributes() );

    // 2 - We want to configure the font and colors
    //     for the data values text.
    TextAttributes ta( dva.textAttributes() );
    dva.setDecimalDigits( 2 );

    // Append a prefix/suffix to the
    // data value labels being displayed
    //
    //dva.setPrefix( "* " );
    dva.setSuffix( " Ohm" );

    //rotate if you wish
    //ta.setRotation( 0 );
    // 3 - Set up your text attributes
    ta.setFont( QFont( "Comic") );
    ta .setPen( QPen( QColor( Qt::darkGreen ) ) );
    ta.setVisible( true );
    // font size
    Measure me( ta.fontSize() );
    me.setValue( me.value() * 0.25 );
    ta.setFontSize( me );

    // 4 - Assign the text attributes to your
    //     DataValuesAttributes
    dva.setTextAttributes( ta );
    dva.setVisible( true );
    // 5 - Assign to the diagram
    diagram->setDataValueAttributes( dva );

    // Set thick line widths for all datasets
    for ( int i = 0; i < m_model.rowCount(); ++i ) {
        QPen pen( diagram->pen( i ) );
        pen.setWidth( 17 );
        diagram->setPen( i, pen );
    }

    // Draw one of the sections of a line differently.
    // 1 - Retrieve the pen for the dataset and change
    //    its style.
    //    This allow us to keep the line original color.
    QPen linePen( diagram->pen( 1 ) );
    linePen.setColor( Qt::yellow );
    linePen.setWidth( 7 );
    linePen.setStyle( Qt::DashLine );
    // 2 - Change the Pen for a section within a line
    //     while assigning it to the diagram
    diagram->setPen( m_model.index( 1, 1, QModelIndex() ), linePen );
    // 3 - Assign to the chart
    m_chart.coordinatePlane()->replaceDiagram(diagram);
    m_chart.setGlobalLeadingRight( 50 );

    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    setLayout(l);
  }

private:
  Chart m_chart;
  QStandardItemModel m_model;
};

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
