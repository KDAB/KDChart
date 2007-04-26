#include "mainwindow.h"

#include <QtTest/QtTest>

class TestParamVsParam: public QObject {
    Q_OBJECT
private slots:

  void testMainWindow()
  {
    QTime t;
    t.start();

    MainWindow      mainWindow;

    qDebug("time constructor: %d ms", t.elapsed());

    mainWindow.show();
    QTimer::singleShot(0, qApp, SLOT(quit()));

    qDebug("time show(): %d ms", t.elapsed());

  }
};

QTEST_MAIN(TestParamVsParam);

#include "main.moc"
