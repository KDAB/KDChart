#include <QtGui>
#include <KDChartChart>
#include <KDChartBarDiagram>
#include <KDChartLegend>
#include <KDChartPosition>

using namespace KDChart;

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
            }
    }

    BarDiagram* diagram = new BarDiagram;
    diagram->setModel(&m_model);

    m_chart.coordinatePlane()->replaceDiagram(diagram);

    // Add at one legend and set it up
    Legend* legend = new Legend( diagram, &m_chart );
    legend->setPosition( Position::NorthEast );
    legend->setAlignment( Qt::AlignCenter );
    legend->setShowLines( false );
    legend->setTitleText( tr( "Legend" ) );
    legend->setOrientation( Qt::Vertical );
    m_chart.addLegend( legend );

    // Configure the items markers
    MarkerAttributes lma;
    lma.setMarkerStyle( MarkerAttributes::MarkerDiamond );
    legend->setMarkerAttributes( 0,  lma );
    lma.setMarkerStyle( MarkerAttributes::MarkerCircle );
    legend->setMarkerAttributes( 1,  lma );

    // Configure Legend Title and labels


    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(&m_chart);
    setLayout(l);
  }

private:
  Chart m_chart;
  QStandardItemModel m_model;
};

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
