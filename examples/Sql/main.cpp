/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include <QtGui>

#include <QApplication>

#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <KDChartChart>
#include <KDChartBarDiagram>
#include <KDChartHeaderFooter>
#include <KDChartPosition>
#include <KDChartBackgroundAttributes>
#include <KDChartFrameAttributes>

#include <QPixmap>

using namespace KDChart;

/**
 * Proxymodel that transposes columns and rows.
 */
class TransposeProxyModel : public QAbstractProxyModel{
public:
  explicit TransposeProxyModel(QObject* parent = 0) : QAbstractProxyModel(parent) {}
  virtual ~TransposeProxyModel() {}
  QModelIndex mapFromSource ( const QModelIndex & sourceIndex ) const { return index(sourceIndex.column(), sourceIndex.row()); }
  QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const { return sourceModel()->index(proxyIndex.column(), proxyIndex.row()); }
  QModelIndex index(int r, int c, const QModelIndex &ind=QModelIndex()) const { Q_UNUSED(ind) return createIndex(r,c); }
  QModelIndex parent(const QModelIndex&) const { return QModelIndex(); }
  int rowCount(const QModelIndex &) const { return sourceModel()->columnCount(); }
  int columnCount(const QModelIndex &) const { return sourceModel()->rowCount(); }
  QVariant data(const QModelIndex &ind, int role) const { return sourceModel()->data(mapToSource(ind), role); }
};

/**
 * The example that creates the SQL-model, adds data to it and display the data in a model.
 */
class ChartWidget : public QWidget {
  Q_OBJECT
public:
  explicit ChartWidget(QWidget* parent=0)
    : QWidget(parent)
    , m_model(0)
  {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(":memory:"); // in memory rather then in a file
    //db.setUserName("");
    //db.setPassword("");
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
            QSqlField field(column==0?"col1":"col2",QVariant::Int);
            field.setValue(row+1 * column);
            rec.append(field);
        }
        ok = m_model->insertRecord(-1, rec);
        Q_ASSERT(ok);
    }

    BarDiagram* diagram = new BarDiagram;

    TransposeProxyModel* proxymodel = new TransposeProxyModel(this);
    proxymodel->setSourceModel(m_model);
    diagram->setModel(proxymodel);

    m_chart.coordinatePlane()->replaceDiagram(diagram);

    // Add at one Header and set it up
    HeaderFooter* header = new HeaderFooter( &m_chart );
    header->setPosition( Position::North );
    header->setText( "A Simple Bar Chart" );
    m_chart.addHeaderFooter( header );

    // Configure the plane Frame attributes
    FrameAttributes pfa;
    pfa.setPen( QPen ( QBrush( Qt::blue ), 2 ) );
    pfa.setVisible( true );
    diagram->coordinatePlane()->setFrameAttributes(  pfa );

    // Configure the header Frame attributes
    FrameAttributes hfa;
    hfa.setPen( QPen ( QBrush( Qt::darkGray ), 2 ) );
    hfa.setPadding( 2 );
    hfa.setVisible( true );
    header->setFrameAttributes(  hfa );

    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    setLayout(l);
  }

private:
  Chart m_chart;
  QSqlTableModel *m_model;
};

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
