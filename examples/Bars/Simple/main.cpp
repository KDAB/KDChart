#include <QtGui>
#include <KDChartChart>
#include <KDChartBarDiagram>


class ChartWidget : public QWidget {
  Q_OBJECT
public:
  explicit ChartWidget(QWidget* parent=0)
    : QWidget(parent)
  {

    m_model.insertRows( 0, 2, QModelIndex() );
    m_model.insertColumns(  0,  3,  QModelIndex() );
    for (int row = 0; row < 3; ++row) {
            for (int column = 0; column < 3; ++column) {
                QModelIndex index = m_model.index(row, column, QModelIndex());
                m_model.setData(index, QVariant(row+1 * column) );
                /*
                // show tooltips:
                m_model.setData(index,
                QString("<table>"
                    "<tr><td>Row</td><td>Column</td><td>Value</td></tr>"
                    "<tr><th>%1</th><th>%2</th><th>%3</th></tr></table>")
                .arg(row).arg(column).arg(row+1 * column), Qt::ToolTipRole );
                */
            }
    }

    /*
    {
    // show a comment at one data item:
        const int row = 0;
        const int column = 2;
        const QModelIndex index = m_model.index(row, column, QModelIndex());
        m_model.setData(
            index,
            QString("Value %1/%2: %3")
                    .arg( row )
                    .arg( column )
                    .arg( m_model.data( index ).toInt() ),
            KDChart::CommentRole );
    }
    */

    KDChart::BarDiagram* diagram = new KDChart::BarDiagram;
    diagram->setModel(&m_model);

    m_chart.coordinatePlane()->replaceDiagram(diagram);

    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    setLayout(l);
  }

private:
  KDChart::Chart m_chart;
  QStandardItemModel m_model;
};

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
