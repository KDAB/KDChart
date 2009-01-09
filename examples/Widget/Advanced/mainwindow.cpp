/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "mainwindow.h"

#include <KDChartWidget>
#include <KDChartAbstractDiagram>
#include <QDebug>
#include <QMessageBox>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent )
 : QWidget( parent ), datasetCount( 3 )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    widget = new Widget( chartFrame );
    chartLayout->addWidget( widget );

    typeSelector->setCurrentIndex(1); // we start by LineDiagram

    connect( typeSelector, SIGNAL( activated( int )), SLOT( changeType() ));

    connect( btnAddDataset, SIGNAL( clicked()), SLOT( addDataset() ));

    connect( leadingSelector, SIGNAL( valueChanged( int )),
		    this, SLOT( changeLeading( int )));
}

void MainWindow::changeType()
{
    QString text = typeSelector->currentText();
    if ( text == "Widget::Bar" )
        widget->setType( Widget::Bar );
    else if ( text == "Widget::Line" )
        widget->setType( Widget::Line );
    else if ( text == "Widget::Pie" )
        widget->setType( Widget::Pie );
    else if ( text == "Widget::Polar" )
        widget->setType( Widget::Polar );
    else
        widget->setType( Widget::NoType );
}

void MainWindow::changeLeading( int leading )
{
    widget->setGlobalLeading( leading, leading, leading, leading );
}

void MainWindow::addDataset()
{
    QStringList parts = lineAddDataset->text().split(';');
    bool ok;
    QVector< double > vec;
    foreach ( const QString &str, parts ) {
        const double val = str.toDouble(&ok);
        if(ok)
            vec.append( val );
    }
    const int rows = widget->diagram()->model()->rowCount();
    if( vec.count() != rows )
        QMessageBox::warning( this, "Wrong number of values entered!",
                              QString("You have entered %1 values,<br>but the data model needs %2 ones.<br><br>Note: Use <b>;</b> to separate the values!")
                                      .arg( vec.count() )
                                      .arg( rows ) );
    else
        widget->setDataset( datasetCount++, vec, "user data" );
}
