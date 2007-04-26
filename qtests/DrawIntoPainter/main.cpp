#include "mainwindow.h"

#include <QtTest/QtTest>

class TestDrawIntoPainter: public QObject {
    Q_OBJECT
private slots:

  void initDrawIntoPainter()
  {
    MainWindow mainWindow;
    mainWindow.show();
  }

  void testOriginal()
  {
    mainWindow.updateData(tr(":/original"));
  }

  void testTest()
  {
    mainWindow.updateData(tr(":/test"));
  }

private:
    MainWindow mainWindow;

};

QTEST_MAIN(TestDrawIntoPainter);

#include "main.moc"
