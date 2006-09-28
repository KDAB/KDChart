#include <KDChartChart>
#include <KDChartPolarDiagram>
#include <KDChartPolarCoordinatePlane>

#include <QApplication>
#include <QStandardItemModel>

using namespace KDChart;

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    // creating the chart
    Chart chart;

    // the model
    QStandardItemModel model;
    model.insertRows( 0, 40 );
    model.insertColumns(  0,  5 );
    int i = 0;

    // fill it with some nice looking data
    for ( int column = 0; column < model.columnCount(); ++column ) {
        for ( int row = 0; row < model.rowCount(); ++row ) {
            QModelIndex index = model.index( row, column );
            model.setData( index, QVariant( i++  ) );
        }
    }

    // we want a polar diagram on a matching coordinate plane
    PolarDiagram diag;
    PolarCoordinatePlane plane( &chart );
    diag.setModel( &model );

    chart.replaceCoordinatePlane( &plane );
    chart.coordinatePlane()->replaceDiagram( &diag );

    chart.show();

    return app.exec();
}
