#include "mainwindow.h"

#include <QtTest/QtTest>

class TestParamVsParam: public QObject {
    Q_OBJECT
private slots:

  void testMainWindow()
  {

    MainWindow      mainWindow;

    QTime t;
    t.start();

    mainWindow.show();
    QTimer::singleShot(0, qApp, SLOT(quit()));

    qDebug("Time elapsed: %d ms", t.elapsed());

  }
};

QTEST_MAIN(TestParamVsParam);

#include "main.moc"
