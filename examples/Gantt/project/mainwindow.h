#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace KDGantt {
    class View;
}

class ProjectModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow( QWidget* parent = 0 );

private slots:
    void slotFileOpen();
    void slotFileQuit();
    void slotToolsNewItem();
    void slotToolsAppendItem();
    void slotCollapseAll();
    void slotExpandAll();

    void slotAlignLeft();
    void slotAlignCenter();
    void slotAlignRight();
    void slotAlignHidden();
    
private:
    ProjectModel* m_model;
    KDGantt::View* m_view;
};
#endif /* MAINWINDOW_H */

