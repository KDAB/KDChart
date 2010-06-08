#include <QApplication>
#include <KDChartWidget>

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    KDChart::Widget widget;
    widget.resize( 600, 600 );

    QVector< double > vec0,  vec1,  vec2;

    vec0 << -5 << -4 << -3 << -2 << -1 << 0
         << 1 << 2 << 3 << 4 << 5;
    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;
    vec2 << -125 << -64 << -27 << -8 << -1 << 0
         << 1 << 8 << 27 << 64 << 125;

    widget.setDataset( 0, vec0, "Linear" );
    widget.setDataset( 1, vec1, "Quadratic" );
    widget.setDataset( 2, vec2, "Cubic" );

    widget.show();

    return app.exec();
}
