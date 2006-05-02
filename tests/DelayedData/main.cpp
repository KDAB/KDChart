#include <QtGui>
#include <KDChartChart>
#include <KDChartBarDiagram>
#include <KDChartCartesianCoordinatePlane>

class ChartWidget : public QWidget {
  Q_OBJECT
public:
  explicit ChartWidget(QWidget* parent=0)
    : QWidget(parent)
  {
    m_model.insertColumns( 0, 10, QModelIndex() );
    KDChart::BarDiagram* diagram = new KDChart::BarDiagram;
    diagram->setModel(&m_model);    
    m_chart.coordinatePlane()->replaceDiagram(diagram);


    m_button.setText( tr("Add data") );
    connect( &m_button, SIGNAL(clicked()),
	     this, SLOT(addData()));

    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    l->addWidget(&m_button);

    setLayout(l);
  }

private slots:

  void addData()
  {
    m_model.insertRows(m_model.rowCount(),1);
    for( int i = 0; i < 10; ++i ) {
      m_model.setData( m_model.index(m_model.rowCount()-1, i), i );
    }
  }

private:
  KDChart::Chart m_chart;
  QPushButton m_button;
  QStandardItemModel m_model;
};

int main( int argc, char** argv ) {
    QApplication app( argc, argv );
    
    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
