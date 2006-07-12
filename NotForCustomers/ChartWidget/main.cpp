#include <QtGui/QApplication>
#include "chartwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChartWidget w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
