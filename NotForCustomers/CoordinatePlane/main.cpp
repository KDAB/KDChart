#include <QtDebug>
#include <QSplitter>
#include <QTableView>
#include <QApplication>
#include <QStandardItemModel>

#include "BarDiagram.h"
#include "CoordinatePlane.h"

int main ( int argc,  char** argv )
{
    const int NumberOfColumns = 2;
    const int NumberOfRows = 5;

    QApplication app( argc,  argv );
    QSplitter splitter;
    QTableView table ( &splitter );

    CoordinatePlane plane( &splitter );

    QStandardItemModel model ( NumberOfRows, NumberOfColumns );
    for ( int row = 0; row < NumberOfRows; ++row )
    {
        for ( int column = 0; column < NumberOfColumns; ++column )
        {
            QModelIndex index = model.index ( row, column );
            const int value = column % 2 == 0 ? row + 1 : NumberOfRows - row;
            model.setData ( index, QVariant( value) );
        }
    }
    table.setModel ( &model );

    KDChart::Bar2Diagram diagram; // this is the local one, remember...
    diagram.setModel ( &model );
    plane.replaceDiagram( &diagram );

    splitter.resize ( 800, 800 );
    splitter.show();
    table.hide(); // comment out to see the table :-)
    return app.exec();
}

