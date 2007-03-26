#include <QSize>
#include <QImage>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QtDebug>
#include <QApplication>
#include <QStandardItemModel>

#include "KDChartChart"
#include "KDChartLegend"
#include "KDChartLineDiagram"
#include "KDChartBackgroundAttributes"
#include "KDChartCartesianAxis"

int main( int argc, char** argv )
{
    QTime time0 = QTime::currentTime();
    const int NumberOfRows = 1000 * 10;
    const int NumberOfColumns = 10;
    const int ImageWidth = 500;
    const int ImageHeight = 500;
    QApplication app( argc, argv );
    QStandardItemModel model( NumberOfRows, NumberOfColumns );
    // we purposefully fill the data model *after* it is connected
    // with the line diagram, to see how this performs:
    // (not)
    for ( int row = 0; row < NumberOfRows; ++row ) {
        for ( int column = 0; column < NumberOfColumns; ++column ) {
            QModelIndex index = model.index( row, column );
            model.setData( index, row * 0.1 * ( column + 1 ) );
        }
    }

    // set up a KDChart::Chart that contains a line diagram:
    KDChart::Chart chart;
    chart.resize( ImageWidth, ImageHeight );
    KDChart::LineDiagram lines;
    lines.setModel( &model );
    chart.coordinatePlane()->replaceDiagram( &lines );

//     KDChart::BackgroundAttributes backgroundAttrs( chart.backgroundAttributes() );
//     backgroundAttrs.setVisible( true );
//     backgroundAttrs.setBrush( QColor(0xd0,0xd0,0xff) );
//     chart.setBackgroundAttributes( backgroundAttrs );
//     KDChart::CartesianAxis *xAxis = new KDChart::CartesianAxis( &lines );
//     KDChart::CartesianAxis *yAxis = new KDChart::CartesianAxis ( &lines );
//     KDChart::CartesianAxis *axisTop = new KDChart::CartesianAxis ( &lines );
//     KDChart::CartesianAxis *axisRight = new KDChart::CartesianAxis ( &lines );
//     xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
//     yAxis->setPosition ( KDChart::CartesianAxis::Left );
//     axisTop->setPosition( KDChart::CartesianAxis::Top );
//     axisRight->setPosition( KDChart::CartesianAxis::Right );
//     KDChart::Legend* legend = new KDChart::Legend( &lines, &chart );
//     legend->setPosition( KDChart::Position::SouthEast );
//     legend->setAlignment( Qt::AlignCenter );
//     legend->setShowLines( false );
//     legend->setTitleText( QObject::tr( "Legend" ) );
//     legend->setOrientation( Qt::Vertical );
//     chart.addLegend( legend );

    QImage image( QSize( ImageWidth, ImageHeight ), QImage::Format_ARGB32 );
    QPainter painter( &image );
    painter.setBrush( Qt::white );
    painter.fillRect( image.rect(), Qt::white );
    // now print a line diagram of the created model into a QImage:
    QTime time1 = QTime::currentTime();
    chart.paint( &painter, QRect( 0, 0, ImageWidth, ImageHeight) );
    painter.end();
    QTime time2 = QTime::currentTime();
    // wrap up:
    // and for verification, save the image:
    image.save( "result.png" );
    QTime time3 = QTime::currentTime();
    chart.show();
    QTimer::singleShot( 0, &app, SLOT( quit() ) );
    app.exec();
    QTime time4 = QTime::currentTime();
    qDebug() << "KDChart Benchmark main: setup time:" << time0.msecsTo( time1 );
    qDebug() << "KDChart Benchmark main: time to paint the chart:" << time1.msecsTo( time2 );
    qDebug() << "KDChart Benchmark main: time to show the chart:" << time3.msecsTo( time4 );
}
