/****************************************************************************
 ** Copyright (C) 2006 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include <QApplication>

#include <KDChartChart>
#include <KDChartLeveyJenningsAxis>
#include <KDChartLeveyJenningsDiagram>
#include <KDChartLeveyJenningsCoordinatePlane>
#include <KDChartLeveyJenningsGridAttributes>

#include <QStandardItemModel>

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    QStandardItemModel model( 14, 3 );

    // Lot 1
    model.setData( model.index( 0, 0 ), 1 );
    model.setData( model.index( 0, 1 ), 210 );
    model.setData( model.index( 0, 2 ), true );

    model.setData( model.index( 1, 0 ), 1 );
    model.setData( model.index( 1, 1 ), 195 );
    model.setData( model.index( 1, 2 ), true );

    model.setData( model.index( 2, 0 ), 1 );
    model.setData( model.index( 2, 1 ), 200 );
    model.setData( model.index( 2, 2 ), true );

    model.setData( model.index( 3, 0 ), 1 );
//    This values should be missing
//    model.setData( model.index( 3, 1 ), 210 );
    model.setData( model.index( 3, 2 ), true );

    model.setData( model.index( 4, 0 ), 1 );
    model.setData( model.index( 4, 1 ), 180 );
    model.setData( model.index( 4, 2 ), true );

    
    // Lot 2
    model.setData( model.index( 5, 0 ), 2 );
    model.setData( model.index( 5, 1 ), 210 );
    model.setData( model.index( 5, 2 ), true );

    model.setData( model.index( 6, 0 ), 2 );
    model.setData( model.index( 6, 1 ), 195 );
    model.setData( model.index( 6, 2 ), true );

    // this value is not OK
    model.setData( model.index( 7, 0 ), 2 );
    model.setData( model.index( 7, 1 ), 200 );
    model.setData( model.index( 7, 2 ), false );

    model.setData( model.index( 8, 0 ), 2 );
    model.setData( model.index( 8, 1 ), 210 );
    model.setData( model.index( 8, 2 ), true );

    model.setData( model.index( 9, 0 ), 2 );
    model.setData( model.index( 9, 1 ), 180 );
    model.setData( model.index( 9, 2 ), true );

    // this values is completely out of bounds and therefore cutted
    model.setData( model.index( 10,0 ), 2 );
    model.setData( model.index( 10,1 ), 290 );
    model.setData( model.index( 10,2 ), true );

    // this value is ok again
    model.setData( model.index( 11,0 ), 2 );
    model.setData( model.index( 11,1 ), 210 );
    model.setData( model.index( 11,2 ), true );

    model.setData( model.index( 12,0 ), 2 );
    model.setData( model.index( 12,1 ), 205 );
    model.setData( model.index( 12,2 ), true );

    model.setData( model.index( 13,0 ), 2 );
    model.setData( model.index( 13,1 ), 204 );
    model.setData( model.index( 13,2 ), true );

 
    KDChart::Chart* chart = new KDChart::Chart();

    KDChart::LeveyJenningsDiagram* diagram = new KDChart::LeveyJenningsDiagram;
    diagram->setModel( &model );
    diagram->setExpectedMeanValue( 200 );
    diagram->setExpectedStandardDeviation( 20 );
    KDChart::LeveyJenningsCoordinatePlane* plane = new KDChart::LeveyJenningsCoordinatePlane;
    chart->replaceCoordinatePlane( plane );
    plane->replaceDiagram( diagram );

    //diagram->setLotChangedSymbolPosition( Qt::AlignBottom );

    KDChart::LeveyJenningsAxis* axis = new KDChart::LeveyJenningsAxis( diagram );
    axis->setPosition( KDChart::CartesianAxis::Left );
    diagram->addAxis( axis );

    KDChart::LeveyJenningsAxis* axis2 = new KDChart::LeveyJenningsAxis( diagram );
    axis2->setPosition( KDChart::CartesianAxis::Right );
    axis2->setType( KDChart::LeveyJenningsGridAttributes::Calculated );
    diagram->addAxis( axis2 );

    diagram->selectionModel()->setCurrentIndex( model.index( 1, 0 ), QItemSelectionModel::Current );

    chart->show();

    return app.exec();
}