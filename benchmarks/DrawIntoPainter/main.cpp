#include <QApplication>
#include "benchmarkwindow.h"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    BenchmarkWindow benchmarkWindow;
    benchmarkWindow.show();

    return app.exec();
}
