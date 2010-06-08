#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>
#include <TableModel.h>

namespace KDChart {
    class Chart;
    class BarDiagram;
    class DatasetProxyModel;
}

class QAbstractItemView;
class QStandardItemModel;
class QTableView;
class QItemSelectionModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected slots:
    void selectionChanged( const QItemSelection & selected, const QItemSelection & deselected );

private slots:
    void initializeData();
    void openFile(const QString &path = QString());
    void saveFile();

    void itemClicked( const QModelIndex& index );

private:
    void setupModel();
    void setupViews();

    KDChart::Chart* m_chart;
    KDChart::DatasetProxyModel* m_datasetProxy;

    QStandardItemModel  *m_model;
    KDChart::BarDiagram *m_diagramView;
    QTableView          *m_tableView;
    QItemSelectionModel *m_selectionModel;
};

#endif
