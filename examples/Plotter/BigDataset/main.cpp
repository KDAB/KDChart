#include "MainWidget.h"

#include <QApplication>

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    MainWidget mainWidget;
    mainWidget.show();
    return app.exec();
}
