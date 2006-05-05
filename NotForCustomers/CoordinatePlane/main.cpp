#include <QApplication>
#include "CoordinatePlane.h"

int main ( int argc,  char** argv )
{
    QApplication app( argc,  argv );
    CoordinatePlane plane;
    plane.resize ( 800, 800 );
    plane.show();

    return app.exec();
}

