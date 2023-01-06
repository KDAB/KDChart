/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <QtGui>

#include <QApplication>

#include <KDChartBackgroundAttributes>
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartHeaderFooter>
#include <KDChartPosition>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

#include <QPixmap>

using namespace KDChart;

/**
 * Proxymodel that transposes columns and rows.
 */
class TransposeProxyModel : public QAbstractProxyModel
{
public:
    explicit TransposeProxyModel(QObject *parent = nullptr)
        : QAbstractProxyModel(parent)
    {
    }
    ~TransposeProxyModel() override
    {
    }
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override
    {
        return index(sourceIndex.column(), sourceIndex.row());
    }
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override
    {
        return sourceModel()->index(proxyIndex.column(), proxyIndex.row());
    }
    QModelIndex index(int r, int c, const QModelIndex &ind = QModelIndex()) const override
    {
        Q_UNUSED(ind)
        return createIndex(r, c);
    }
    QModelIndex parent(const QModelIndex &) const override
    {
        return QModelIndex();
    }
    int rowCount(const QModelIndex &) const override
    {
        return sourceModel()->columnCount();
    }
    int columnCount(const QModelIndex &) const override
    {
        return sourceModel()->rowCount();
    }
    QVariant data(const QModelIndex &ind, int role) const override
    {
        return sourceModel()->data(mapToSource(ind), role);
    }
};

/**
 * The example that creates the SQL-model, adds data to it and display the data in a model.
 */
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setDatabaseName(":memory:"); // in memory rather then in a file
        // db.setUserName("");
        // db.setPassword("");
        bool ok = db.open();
        Q_ASSERT(ok);
        Q_UNUSED(ok) // release mode

        QSqlQuery createTableQuery = db.exec("CREATE TABLE IF NOT EXISTS MyTable (col1 INT NOT NULL PRIMARY KEY, col2 INT);");
        Q_ASSERT(!createTableQuery.lastError().isValid());

        m_model = new QSqlTableModel(this, db);
        m_model->setTable("MyTable");
        m_model->setEditStrategy(QSqlTableModel::OnRowChange);
        m_model->setSort(0, Qt::AscendingOrder);

        ok = m_model->select();
        Q_ASSERT(ok);

        m_model->setHeaderData(0, Qt::Horizontal, tr("Column 1"));
        m_model->setHeaderData(1, Qt::Horizontal, tr("Column 2"));

        for (int row = 0; row < 3; ++row) {
            QSqlRecord rec;
            for (int column = 0; column < 2; ++column) {
                QSqlField field(column == 0 ? "col1" : "col2", QVariant::Int);
                field.setValue(row + 1 * column);
                rec.append(field);
            }
            ok = m_model->insertRecord(-1, rec);
            Q_ASSERT(ok);
        }

        auto *diagram = new BarDiagram;

        auto *proxymodel = new TransposeProxyModel(this);
        proxymodel->setSourceModel(m_model);
        diagram->setModel(proxymodel);

        m_chart.coordinatePlane()->replaceDiagram(diagram);

        // Add at one Header and set it up
        auto *header = new HeaderFooter(&m_chart);
        header->setPosition(Position::North);
        header->setText("A Simple Bar Chart");
        m_chart.addHeaderFooter(header);

        // Configure the plane Frame attributes
        FrameAttributes pfa;
        pfa.setPen(QPen(QBrush(Qt::blue), 2));
        pfa.setVisible(true);
        diagram->coordinatePlane()->setFrameAttributes(pfa);

        // Configure the header Frame attributes
        FrameAttributes hfa;
        hfa.setPen(QPen(QBrush(Qt::darkGray), 2));
        hfa.setPadding(2);
        hfa.setVisible(true);
        header->setFrameAttributes(hfa);

        auto *l = new QVBoxLayout(this);
        l->addWidget(&m_chart);
        setLayout(l);
    }

private:
    Chart m_chart;
    QSqlTableModel *m_model = nullptr;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
