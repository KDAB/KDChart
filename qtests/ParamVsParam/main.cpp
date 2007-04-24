#include "mainwindow.h"

#include <QtTest/QtTest>

class TestParamVsParam: public QObject {
    Q_OBJECT
private slots:

  void testMainWindow()
  {

    char** argv = (char**) new char[1]; int argc = 1;
    argv[0] = "TestParamVsParam";

    QApplication    app(argc, argv);
    MainWindow      mainWindow;

    QTime t;
    t.start();

    mainWindow.show();
    QTimer::singleShot(0, &app, SLOT(quit()));

    app.exec();

    qDebug("Time elapsed: %d ms", t.elapsed());

  }
};

QTEST_MAIN(TestParamVsParam);

#include "main.moc"
