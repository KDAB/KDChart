#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>

class QStandardItem;
class QStandardItemModel;

namespace KDGantt {
    class ConstraintModel;
    class DateTimeGrid;
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );

private slots:
    void addNewEntry();
    void removeEntry();
    void addDemoEntry();
    void printPreview();
    void showContextMenu( const QPoint& );
    void enableActions( const QItemSelection& selected );
    void zoomIn();
    void zoomOut();

    void slotClicked( const QModelIndex& );
    void slotDoubleClicked( const QModelIndex& );

private:
    void initModel();
    void initActions();
    void initItemDelegate();
    void initGrid();

    void setReadOnly( const QModelIndex& index, bool readOnly );
    void addConstraint( const QModelIndex& index1, const QModelIndex& index2 );
    void addConstraint( const QStandardItem* item1, const QStandardItem* item2 );

    QStandardItemModel* model;
    KDGantt::ConstraintModel* constraintModel;
    KDGantt::DateTimeGrid* grid;
    int dayWidth;

    QAction* newEntryAction;
    QAction* removeEntryAction;
    QAction* demoAction;
    QAction* printAction;
    QAction* zoomInAction;
    QAction* zoomOutAction;

    Ui::MainWindow* ui;
};

#endif /* MAINWINDOW_H */
