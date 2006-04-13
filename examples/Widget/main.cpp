#include <QApplication>
#include <KDChartWidget>

#include <QDebug>
#include <QVector>

#include "mainwindow.h"

using namespace KDChart;

int main( int argc, char** argv ) {
    QApplication app( argc, argv );
    
    MainWindow window;
    
    window.resize( 800, 600 );

    QVector< double > vec0;
    vec0.append( -5 );
    vec0.append( -4 );
    vec0.append( -3 );
    vec0.append( -2 );
    vec0.append( -1 );
    vec0.append( 0 );
    vec0.append( 1 );
    vec0.append( 2 );
    vec0.append( 3 );
    vec0.append( 4 );
    vec0.append( 5 );
    window.widget->setDataset( 0, vec0, "Linear" );
    
    QVector< double > vec1;
    vec1.append( 25 );
    vec1.append( 16 );
    vec1.append( 9 );
    vec1.append( 4 );
    vec1.append( 1 );
    vec1.append( 0 );
    vec1.append( 1 );
    vec1.append( 4 );
    vec1.append( 9 );
    vec1.append( 16 );
    vec1.append( 25 );
    window.widget->setDataset( 1, vec1, "Quadratic" );

    QVector< double > vec2;
    vec2.append( -125 );
    vec2.append( -64 );
    vec2.append( -27 );
    vec2.append( -8 );
    vec2.append( -1 );
    vec2.append( 0 );
    vec2.append( 1 );
    vec2.append( 8 );
    vec2.append( 27 );
    vec2.append( 64 );
    vec2.append( 125 );
    window.widget->setDataset( 2, vec2, "Cubic" );
   
    window.widget->addHeaderFooter( "NorthWest", HeaderFooter::NorthWest );
    window.widget->addHeaderFooter( "North", HeaderFooter::North );
    window.widget->addHeaderFooter( "NorthEast", HeaderFooter::NorthEast );
    window.widget->addHeaderFooter( "SouthEast", HeaderFooter::SouthEast );
    window.widget->addHeaderFooter( "South", HeaderFooter::South );
    window.widget->addHeaderFooter( "SouthWest", HeaderFooter::SouthWest );

    window.widget->addLegend( Legend::East );
   
    //window.widget->setType( Widget::Polar );
    
    window.show();
    
    return app.exec();
}
