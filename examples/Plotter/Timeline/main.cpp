#include <QApplication>
#include <QStandardItemModel>
#include <QTreeView>

#include <math.h>

#include <KDChartChart>
#include <KDChartPlotter>

#include "timeaxis.h"
#include "timechartmodel.h"

/**
 * This example demonstrates how to use time-based plots with timestamp-value data points
 * based on seconds and how to use a proxy model for defining the plotted "window" of the
 * measurement data.
 */
int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );

    QStandardItemModel model( 365, 2 );
    for( int i = 0; i < 365; ++i )
    {
        const QDateTime dt = QDateTime( QDate( 2010, 1, 1 ), QTime() );
        model.setData( model.index( i, 0 ), dt.addDays( i ) );
        model.setData( model.index( i, 1 ), sin( i / 10.0 ) * 10 );
    }

    TimeChartModel proxy;
    proxy.setSourceModel( &model );
    proxy.setVisibleRange( QDateTime( QDate( 2010, 2, 1 ), QTime() ), 
                           QDateTime( QDate( 2010, 3, 31 ), QTime() ) );

    KDChart::Chart* chart = new KDChart::Chart;
    KDChart::Plotter* plotter = new KDChart::Plotter;
    chart->coordinatePlane()->replaceDiagram( plotter );

    QTreeView tv;
    tv.setModel( &proxy );
    tv.show();

    TimeAxis* axis = new TimeAxis( plotter );
    axis->setPosition( TimeAxis::Bottom );
    plotter->addAxis( axis );

    plotter->setModel( &proxy );

    QObject::connect( &proxy, SIGNAL( rowsInserted( QModelIndex, int, int ) ), chart->coordinatePlane(), SLOT( adjustRangesToData() ), Qt::QueuedConnection );
    QObject::connect( &proxy, SIGNAL( rowsRemoved( QModelIndex, int, int ) ), chart->coordinatePlane(), SLOT( adjustRangesToData() ), Qt::QueuedConnection );
    
    dynamic_cast< KDChart::CartesianCoordinatePlane* >( chart->coordinatePlane() )->setHorizontalRange( qMakePair< qreal, qreal >( 1264978800, 1269986400 ) );
    proxy.setVisibleRange( QDateTime( QDate( 2010, 3, 15 ), QTime() ), 
                           QDateTime( QDate( 2010, 5, 18 ), QTime() ) );

    chart->show();

    return app.exec();
}
