#include <qapplication.h>
#include "mainwindow.h"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    MainWindow mainWindow;
    mainWindow.show();
    mainWindow.setWindowTitle(QObject::tr("You can have a chart without having any data at all."));

    return app.exec();
}
