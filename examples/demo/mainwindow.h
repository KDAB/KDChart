#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();
    
signals:
    
public slots:
private:
    class Private;
    Private *d;
    
};

#endif // MAINWINDOW_H
