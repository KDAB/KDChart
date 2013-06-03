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

#include <QApplication>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTimer>
#include <QWidget>
#include <QSplitter>
#include <QPushButton>

#include <math.h>

#include <KDChartChart>
#include <KDChartPlotter>

#include "timeaxis.h"
#include "timechartmodel.h"

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(QWidget* parent = 0) : QWidget(parent), m_counter(0) {
        QSplitter* splitter = new QSplitter(this);
        QHBoxLayout* l = new QHBoxLayout(this);
        setLayout(l);
        l->addWidget( splitter );

        QWidget* leftWidget = new QWidget( splitter );
        QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
        leftWidget->setLayout(leftLayout);
        
        QPushButton* button = new QPushButton("Animate", leftWidget);
        leftLayout->addWidget( button );
        button->setCheckable( true );
        connect( button, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)) );

        QTreeView* tv = new QTreeView( leftWidget );
        leftLayout->addWidget( tv );

        m_chart = new KDChart::Chart( splitter );
        
        m_model = new QStandardItemModel( 365, 2, this );
        for ( int i = 0; i < 365; ++i ) {
            const QDateTime dt = QDateTime( QDate( 2010, 1, 1 ), QTime() );
            m_model->setData( m_model->index( i, 0 ), dt.addDays( i ) );
            m_model->setData( m_model->index( i, 1 ), sin( i / 10.0 ) * 10 );
        }

        TimeChartModel* proxy = new TimeChartModel( this );
        proxy->setSourceModel( m_model );
        proxy->setVisibleRange( QDateTime( QDate( 2010, 2, 1 ), QTime() ),
                                QDateTime( QDate( 2010, 3, 31 ), QTime() ) );

        KDChart::Plotter* plotter = new KDChart::Plotter;
        m_chart->coordinatePlane()->replaceDiagram( plotter );

        tv->setModel( proxy );
        tv->show();

        TimeAxis* axis = new TimeAxis( plotter );
        axis->setPosition( TimeAxis::Bottom );
        plotter->addAxis( axis );

        plotter->setModel( proxy );

        connect( proxy, SIGNAL( rowsInserted( QModelIndex, int, int ) ),
                 m_chart->coordinatePlane(), SLOT( adjustRangesToData() ) );
        connect( proxy, SIGNAL( rowsRemoved( QModelIndex, int, int ) ),
                 m_chart->coordinatePlane(), SLOT( adjustRangesToData() ) );

        proxy->setVisibleRange( QDateTime( QDate( 2010, 3, 15 ), QTime() ),
                                QDateTime( QDate( 2010, 5, 18 ), QTime() ) );
        qobject_cast< KDChart::CartesianCoordinatePlane* >( m_chart->coordinatePlane() )->adjustRangesToData();

        m_timer = new QTimer(this);
        connect( m_timer, SIGNAL( timeout() ), this, SLOT( slotTimeout() ) );
    }
private slots:
    void slotTimeout() {
        QVariant v1, v2;
        for ( int i = 0; i < 365; ++i ) {
            m_model->setData( m_model->index( i, 1 ), sin( i / 10.0 + m_counter ) * 10 );
        }
        m_counter += 0.02;
    }
    void buttonToggled(bool checked) {
        if (checked)
            m_timer->start( 200 );
        else
            m_timer->stop();
    }
private:
    KDChart::Chart* m_chart;
    QStandardItemModel* m_model;
    QTimer* m_timer;
    qreal m_counter;
};

/**
 * This example demonstrates how to use time-based plots with timestamp-value data points
 * based on seconds and how to use a proxy model for defining the plotted "window" of the
 * measurement data.
 */
int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    
    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
