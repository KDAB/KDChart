#include <QtGui>
#include <KDChartChart>
#include <KDChartBarDiagram>
#include <KDChartCartesianAxis>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLegend>

class ChartWidget : public QWidget {
  Q_OBJECT
public:
  explicit ChartWidget(QWidget* parent=0)
    : QWidget(parent)
  {
    m_model.insertColumns( 0, 10, QModelIndex() );
    KDChart::BarDiagram* diagram = new KDChart::BarDiagram;
    diagram->setModel(&m_model);    

    KDChart::Legend* legend = new KDChart::Legend(diagram,diagram);
    m_chart.addLegend(legend);

    // The code below doesn't work, but it would 
    // be nice if it did:
#if 0
    KDChart::Legend* legend = new KDChart::Legend;
    legend->addDiagram(diagram1);
    legend->addDiagram(diagram2);
    ...
    m_chart.addLegend(legend);
#endif

    KDChart::CartesianAxis* abcissa = new KDChart::CartesianAxis(diagram);
    abcissa->setPosition( KDChart::CartesianAxis::Bottom );
    KDChart::CartesianAxis* ordinate = new KDChart::CartesianAxis(diagram);
    ordinate->setPosition( KDChart::CartesianAxis::Left );
    diagram->addAxis(abcissa);
    diagram->addAxis(ordinate);

    // NOTE: If this is done before adding axes,
    // the axes dont show up at all
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
    for( int i = 0; i < 7; ++i ) {
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
