#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>

class QStandardItemModel;
class QCloseEvent;

namespace KDGantt {
    class ConstraintModel;
    class DateTimeGrid;
    class Legend;
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    virtual ~MainWindow();
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void addNewEntry();
    void removeEntry();
    void showContextMenu( const QPoint& );
    void enableActions( const QItemSelection& selected );
    void zoomIn();
    void zoomOut();
    void zoomFit();

private:
    void initModel();
    void initActions();
    void initItemDelegate();
    void initGrid();

    void setReadOnly( const QModelIndex& index, bool readOnly );
    void addConstraint( const QModelIndex& index1, const QModelIndex& index2 );

    QStandardItemModel* model;
    KDGantt::ConstraintModel* constraintModel;
    KDGantt::DateTimeGrid* grid;
    KDGantt::Legend* smallLegend;
    KDGantt::Legend* detailedLegend;

    QAction* newEntryAction;
    QAction* removeEntryAction;
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* zoomFitAction;

    Ui::MainWindow* ui;
};

#endif /* MAINWINDOW_H */
