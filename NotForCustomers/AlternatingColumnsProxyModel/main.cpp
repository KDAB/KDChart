#include <QSplitter>
#include <QTableView>
#include <QApplication>
#include <QStandardItemModel>

#include "AlternatingColumnsProxyModel.h"

int main ( int argc,  char** argv )
{
    const int NoOfValues = 20;
    QApplication app ( argc, argv );

    QStandardItemModel model( NoOfValues/2, 2 );
    for ( int count = 0; count < NoOfValues; ++count )
    {
        QModelIndex index = model.index ( count, count % 2 );
        model.setData ( index, QVariant( count + 1 ) );
    }

    AlternatingColumnsProxyModel proxyModel;
    proxyModel.setSourceModel ( & model );

    QSplitter splitter;
    QTableView sourceview ( &splitter );
    sourceview.setModel ( &model );
    QTableView proxyView ( &splitter );
    proxyView.setModel ( &proxyModel );
    splitter.show();

    return app.exec();
}

