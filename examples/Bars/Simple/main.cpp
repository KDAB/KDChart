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
#include <QtSvg/QSvgGenerator>
#include <KDChartChart>
#include <KDChartBarDiagram>
#include <QApplication>

class ChartWidget : public QWidget {
  Q_OBJECT
public:
  explicit ChartWidget(QWidget* parent=0)
    : QWidget(parent)
  {

    m_model.insertRows( 0, 2, QModelIndex() );
    m_model.insertColumns(  0,  3,  QModelIndex() );
    for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row+1 * column) );
                /*
                // show tooltips:
                m_model.setData(index,
                QString("<table>"
                    "<tr><td>Row</td><td>Column</td><td>Value</td></tr>"
                    "<tr><th>%1</th><th>%2</th><th>%3</th></tr></table>")
                .arg(row).arg(column).arg(row+1 * column), Qt::ToolTipRole );
                */
            }
    }

    /*
    {
    // show a comment at one data item:
        const int row = 0;
        const int column = 2;
        const QModelIndex index = m_model.index(row, column, QModelIndex());
        m_model.setData(
            index,
            QString("Value %1/%2: %3")
                    .arg( row )
                    .arg( column )
                    .arg( m_model.data( index ).toInt() ),
            KDChart::CommentRole );
    }
    */

    KDChart::BarDiagram* diagram = new KDChart::BarDiagram;
    diagram->setModel(&m_model);
    diagram->setPen( QPen( Qt::black, 0 ) );

    m_chart.coordinatePlane()->replaceDiagram(diagram);

    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    setLayout(l);

    /*
    // render chart to a SVG
    QSvgGenerator generator;
    generator.setFileName("/home/kdab/chart.svg");
    generator.setSize(QSize(300, 300));
    generator.setViewBox(QRect(0, 0, 300, 300));
    generator.setTitle(tr("SVG Chart"));
    QPainter painter;
    painter.begin(&generator);
    painter.setRenderHint(QPainter::Antialiasing);
    m_chart.paint(&painter, generator.viewBox());
    painter.end();
    */
  }

private:
  KDChart::Chart m_chart;
  QStandardItemModel m_model;
};

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
