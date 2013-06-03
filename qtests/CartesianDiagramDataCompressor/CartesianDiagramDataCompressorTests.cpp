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

#include <QtDebug>
#include <QDebug>
#include <QtTest/QtTest>
#include <QStandardItem>
#include <QStandardItemModel>

#include <KDChartCartesianDiagramDataCompressor_p.h>

typedef KDChart::CartesianDiagramDataCompressor::CachePosition CachePosition;

struct Match {
    Match( const CachePosition& pos, const QModelIndex& index )
        : cachePosition( pos ),
          index( index )
    {
    }
    CachePosition cachePosition;
    QModelIndex index;
};

class CartesianDiagramDataCompressorTests : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase()
    {
        // make 10 data sets a n elements
        model.clear();
        model.setColumnCount( ColumnCount );
        model.setRowCount( RowCount );
        for ( int row = 0; row < RowCount; ++row )
            for ( int column = 0; column < ColumnCount; ++column ) {
                QStandardItem* item = new QStandardItem();
                item->setData( 1, Qt::DisplayRole );
                model.setItem( row, column, item );
            }

        width = 200;
        height = 100;
        // these settings result in 5 indexes per pixel
    }

    void initializationTest()
    {
        QVERIFY2( compressor.modelDataColumns() == 0, "without a model, columns should be zero" );
        QVERIFY2( compressor.modelDataRows() == 0, "without a model,rows should be zero" );
    }

    void setModelTest()
    {
        compressor.setModel( &model );
        QVERIFY2( compressor.modelDataColumns() == model.columnCount(),
                  "columns should be equal to model columns, independent of resolution" );
        QVERIFY2( compressor.modelDataRows() == 0, "without a diagram resolution, columns should be zero" );
    }

    void setResolutionTest()
    {
        compressor.setResolution( width, height );
        QVERIFY2( compressor.modelDataColumns() == model.columnCount(),
                  "column count should be equal to dataset count when both model and resolution are set" );
        QVERIFY2( compressor.modelDataRows() == width,
                  "row count should be equal to width when both model and resolution are set, and "
                  "model row count exceeds widget width");
    }

    void mapToCacheTest()
    {
        CachePosition NullPoint( -1, -1 );
        Match  matches[] = {                                    
            Match( CachePosition( 0, 0 ), model.index( 0, 0 ) ), 
            Match( CachePosition( 0, 0 ), model.index( 1, 0 ) ), 
            Match( CachePosition( 0, 0 ), model.index( 2, 0 ) ), 
            Match( CachePosition( 0, 0 ), model.index( 3, 0 ) ), 
            Match( CachePosition( 0, 0 ), model.index( 4, 0 ) ), 
            Match( CachePosition( 0, 1 ), model.index( 0, 1 ) ),
            Match( CachePosition( 0, 1 ), model.index( 1, 1 ) ),
            Match( CachePosition( 0, 1 ), model.index( 2, 1 ) ),
            Match( CachePosition( 0, 1 ), model.index( 3, 1 ) ),
            Match( CachePosition( 0, 1 ), model.index( 4, 1 ) ),
            Match( CachePosition( 2, 2 ), model.index( 10, 2 ) ),
            Match( CachePosition( 2, 2 ), model.index( 11, 2 ) ),
            Match( CachePosition( 2, 2 ), model.index( 12, 2 ) ),
            Match( CachePosition( 2, 2 ), model.index( 13, 2 ) ),
            Match( CachePosition( 2, 2 ), model.index( 14, 2 ) ),
            // the following are outside the model boundary:
            Match( NullPoint, model.index( 0, ColumnCount ) ),
            Match( NullPoint, model.index( 1, ColumnCount ) ),
            Match( NullPoint, model.index( 2, ColumnCount ) ),
            Match( NullPoint, model.index( 3, ColumnCount ) ),
            Match( NullPoint, model.index( 4, ColumnCount) ),
            Match( NullPoint, model.index( RowCount, 0 ) ),
            // sentinel
            Match( CachePosition( 0, 0 ), QModelIndex() )
        };

        for ( int i = 0; matches[i].index.isValid(); ++i ) {
            QCOMPARE( matches[i].cachePosition, compressor.mapToCache( matches[i].index ) );
        }

        QCOMPARE( NullPoint, compressor.mapToCache( QModelIndex() ) );
    }

    void mapToModelTest()
    {
        // test 1: valid point:
        {
            QModelIndexList indexes;
            CachePosition point( 0, 0 );
            indexes = compressor.mapToModel( point );
            Q_FOREACH( const QModelIndex& index, indexes ) {
                QVERIFY2( compressor.mapToCache( index ) == point,
                          "index mapToModel does not map back to the original cache point" );
            }
        }
        // test 2: invalid point:
        {
            QModelIndexList indexes;
            CachePosition point( 0, ColumnCount ); // just outside column count
            indexes = compressor.mapToModel( point );
            QVERIFY2( indexes.isEmpty(),
                      "index list for a point outside the data space should be empty" );
        }
        {
            QModelIndexList indexes;
            CachePosition point( RowCount, 0 ); // just outside row count
            indexes = compressor.mapToModel( point );
            QVERIFY2( indexes.isEmpty(),
                      "index list for a point outside the data space should be empty" );
        }
    }


   void invalidateTest()
    {
        CachePosition position( 0, 0 );
        // KDChart::CartesianDiagramDataCompressor::DataPoint point;
        QVERIFY( compressor.mapsToModelIndex( position ) );
        compressor.retrieveModelData( position );
        QVERIFY2( compressor.isCached( position ),
                  "datapoint should be valid after retrieveModelData( position )" );
        QModelIndex index( model.index( 0, 0 ) );
        compressor.slotModelDataChanged( index, index );
        QVERIFY2( ! compressor.isCached( position ),
                  "datapoint should be not valid after call to dataChanged slot" );
        CachePosition position2( 1, 0 );
        CachePosition position3( 2, 1 );
        compressor.retrieveModelData( position2 );
        compressor.retrieveModelData( position3 );
        QVERIFY2( ! compressor.isCached( position ),
                  "datapoint should be not valid after call to dataChanged slot" );
        compressor.retrieveModelData( position );
        QVERIFY2( compressor.isCached( position ) && compressor.isCached( position2 ) && compressor.isCached( position3 ),
                  "datapoints should all be valid after retrieveModelData" );
        QModelIndex index2( model.index( 1 * compressor.indexesPerPixel(), 0 ) );
        compressor.slotModelDataChanged( index, index2 );
        QVERIFY2( ! compressor.isCached( position ) && ! compressor.isCached( position2 ) && compressor.isCached( position3 ),
                  "dataChanged needs to invalidate an exact range" );
    }

    void datasetDimensionTest()
    {
        int oldDimension = compressor.m_datasetDimension;
        compressor.setDatasetDimension( 2 );
        QVERIFY2( compressor.modelDataColumns() == ColumnCount / 2,
                  "datasetDimension == 2 should cut the column count in half" );
        compressor.setDatasetDimension( oldDimension );
        QVERIFY2( compressor.modelDataColumns() == ColumnCount,
                  "datasetDimension == 1 should restore the old column count" );
    }

    void cleanupTestCase()
    {
    }

private:
    KDChart::CartesianDiagramDataCompressor compressor;
    QStandardItemModel model;
    static const int RowCount;
    static const int ColumnCount;
    int width;
    int height;
};

const int CartesianDiagramDataCompressorTests::ColumnCount = 10;
const int CartesianDiagramDataCompressorTests::RowCount = 1000;


QTEST_MAIN(CartesianDiagramDataCompressorTests)

#include "CartesianDiagramDataCompressorTests.moc"
