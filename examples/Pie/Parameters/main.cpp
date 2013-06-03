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
#include <KDChartPieDiagram>
#include <KDChartDataValueAttributes>
#include <KDChartBackgroundAttributes>
#include <KDChartPieAttributes>
#include <KDChartPosition>

#include <QApplication>

using namespace KDChart;

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(QWidget* parent=0)
        : QWidget(parent)
    {

        m_model.insertRows( 0, 1, QModelIndex() );
        m_model.insertColumns(  0,  6,  QModelIndex() );
        for (int row = 0; row < 1; ++row) {
            for (int column = 0; column < 6; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row+1 * column+1) );

                // this shows the index as static comments:
                // m_model.setData(index, QString("row: %1,  column: %2").arg(row).arg(column), KDChart::CommentRole);

                // this shows the index as volatile tooltips:
                m_model.setData(index, QString("row: %1,  column: %2").arg(row).arg(column), Qt::ToolTipRole);
            }
        }
        // We need a Polar plane for the Pie type
        PolarCoordinatePlane* polarPlane = new PolarCoordinatePlane( &m_chart );
        // replace the default Cartesian plane with
        // our Polar plane
        m_chart.replaceCoordinatePlane( polarPlane );

        // assign the model to our pie diagram
        PieDiagram* diagram = new PieDiagram;
        diagram->setModel(&m_model);

        // Configure some Pie specifical attributes

        // explode a section
        PieAttributes pa(  diagram->pieAttributes() );
        pa.setExplodeFactor( 0.1 );

        // Assign the attributes
        // to the diagram
        diagram->setPieAttributes( 1,  pa );

        // Configure a generic attribute
        // available to all chart types
        QPen sectionPen;
        sectionPen.setWidth( 5 );
        sectionPen.setStyle( Qt::DashLine );
        sectionPen.setColor( Qt::magenta );

        diagram->setPen( 1, sectionPen );

        // Display data values
        // not implemented yet - disable for now
        const QFont font(QFont( "Comic", 10 ));
        const int colCount = diagram->model()->columnCount();
        for ( int iColumn = 0; iColumn<colCount; ++iColumn ) {
            DataValueAttributes dva( diagram->dataValueAttributes( iColumn ) );
            TextAttributes ta( dva.textAttributes() );
            ta.setRotation( 0 );
            ta.setFont( font );
            ta .setPen( QPen( Qt::darkBlue ) );
            ta.setVisible( true );
            dva.setTextAttributes( ta );

            BackgroundAttributes back( dva.backgroundAttributes() );
            back.setBrush( QBrush( QColor(255,0,0) ) );
            back.setVisible( true );
            dva.setBackgroundAttributes( back );

            RelativePosition posPos( dva.positivePosition() );
            posPos.setReferencePosition( KDChart::Position::North );
            posPos.setAlignment( Qt::AlignCenter );
            posPos.setHorizontalPadding( KDChart::Measure(0.0) );
            posPos.setVerticalPadding( KDChart::Measure(-1000.0) );
            dva.setPositivePosition( posPos );
            dva.setVisible( true );
            diagram->setDataValueAttributes( iColumn, dva);
        }

        // Assign our diagram to the Chart
        m_chart.coordinatePlane()->replaceDiagram(diagram);

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
