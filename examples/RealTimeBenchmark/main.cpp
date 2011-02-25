//------------------------------------------------------------------------
// COPYRIGHT (C) 2010
// THIS SOFTWARE IS THE PROPERTY OF ROCHE DIAGNOSTICS GRAZ GMBH, AUSTRIA:
// ALL RIGHTS RESERVED NO PART OF THIS SOFTWARE MAY BE USED OR COPIED IN
// ANY WAY WITHOUT THE PRIOR WRITTEN PERMISSION OF ROCHE DIAGNOSTICS GRAZ
// GMBH, AUSTRIA
//------------------------------------------------------------------------

/*!
 * \file %FILENAME%
 * \brief $MODULE$ Class
 * \author $AUTHOR$ <$EMAIL$>
 */

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartPlotter>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartCartesianAxis>
#include <KDChartGridAttributes>
#include <KDChartBackgroundAttributes>
#include <KDChartLegend>
#include <KDChartCartesianCoordinatePlane>

#include <QVariant>
#include <QTimer>
#include <QVector>
#include <QDebug>

#include <cmath>

const int maxDataValues = 96;

class TestDataModelDynamic : public QAbstractTableModel
{
Q_OBJECT
public:
    TestDataModelDynamic( int initialDataValues = 0, QObject * parent = NULL ) : QAbstractTableModel( parent ), _dataValues( maxDataValues ), _count( 0 )
    {
        _pTimer = new QTimer(this);
        connect(_pTimer, SIGNAL(timeout()), this, SLOT(addData()));
        _pTimer->setInterval(50);
        for ( int amount = 0; amount < initialDataValues; ++ amount )
        {
            _timeValues.append(double(_count));
            for ( int i = 0; i < maxDataValues; ++i)
            {
                double value;
                if (_count > i)
                    value = 10.0 + 40.0 * (0.5 + (cos(M_PI  + M_PI *(double(_count-i)/100.0))/2.0));
                else
                    value = 10.0;

                _dataValues[i].append(value);
            }
            ++_count;
        }
    }

    //! \reimpl
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const
    { Q_UNUSED(parent); return maxDataValues*2; }
    //! \reimpl
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const
    { Q_UNUSED(parent); return _timeValues.count();}

    //! \reimpl
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const
    {
        if (index.isValid()
                && index.row() < rowCount()
                && index.column() < columnCount() )
        {
            switch(role)
            {
            case Qt::DisplayRole:
            {
                if (index.column() % 2 == 0)
                {

                    if (index.row() < _timeValues.size())
                        return QVariant(_timeValues[index.row()]);
                }
                else
                {
                    if (index.row() < _dataValues[0].size())
                    {
                        return QVariant(_dataValues[index.column()/2][index.row()]);
                    }
                }
            }
            break;
            default:
            break;
            }
        }
        return QVariant();
    }

signals:
    void stopped();

public slots:
    void onStart()
    {
        _timeValues.clear();
        for (int i=0; i < maxDataValues; ++i)
            _dataValues[i].clear();

        _count = 0;
        _pTimer->start();
    }
    void onStop()
    {
        _count = 0;
        _pTimer->stop();
        emit stopped();
    }

public slots:
    void addData()
    {
        //qDebug() << "addData()";
        if (_count > 99)
        {
            onStop();
            return;
        }

        beginInsertRows(QModelIndex(),_count, _count);
        _timeValues.append(double(_count));
        for (int i=0; i < maxDataValues; ++i)
        {
            double value;
            if (_count > i)
                value = 10.0 + 40.0 * (0.5 + (cos(M_PI  + M_PI *(double(_count-i)/100.0))/2.0));
            else
                value = 10.0;

            _dataValues[i].append(value);
        }
        endInsertRows();
        emit dataChanged(index(_count, 0), index(_count, 2*96-1));
        ++_count;
        //qDebug() << "addedStuff";
    }

private:
    QVector<double>             _timeValues;
    QVector<QVector<double> >   _dataValues;
    int                         _count;
    QTimer*                     _pTimer;
};

class MySmallBenchTest : public QObject
{
    Q_OBJECT

public:
    MySmallBenchTest();
    void bench( int dataPoint, bool compress = false );

private Q_SLOTS:
    void benchTen();
    void benchTwentyFive();
    void benchFifty();
    void benchNinetyNine();
    void benchTwoHundredFifty();
    void benchFiveHundred();
    void benchThousand();
    void benchTwoThousandFiveHundred();
    void benchFiveThousand();
    void benchTenThousand();

    void benchCompressedTen();
    void benchCompressedTwentyFive();
    void benchCompressedFifty();
    void benchCompressedNinetyNine();
    void benchCompressedTwoHundredFifty();
    void benchCompressedFiveHundred();
    void benchCompressedThousand();
    void benchCompressedTwoThousandFiveHundred();
    void benchCompressedFiveThousand();
    void benchCompressedTenThousand();
};

MySmallBenchTest::MySmallBenchTest()
{
}

void MySmallBenchTest::bench( int dataPoint, bool compress )
{
    KDChart::Chart *chart = new KDChart::Chart;
    KDChart::Plotter *plotter = new KDChart::Plotter();
    if ( compress )
        plotter->setUseDataCompression( KDChart::Plotter::SLOPE );

    KDChart::CartesianAxis *leftAxis = new KDChart::CartesianAxis( plotter );
    KDChart::CartesianAxis *bottomAxis = new KDChart::CartesianAxis( plotter );

    leftAxis->setPosition( KDChart::CartesianAxis::Left );
    bottomAxis->setPosition( KDChart::CartesianAxis::Bottom );

    plotter->addAxis( leftAxis );
    plotter->addAxis( bottomAxis );
    plotter->setAntiAliasing( false );

    chart->coordinatePlane()->replaceDiagram( plotter );
    TestDataModelDynamic model( dataPoint );
    plotter->setModel( &model );


    KDChart::CartesianCoordinatePlane *plane = dynamic_cast <KDChart::CartesianCoordinatePlane*>( plotter->coordinatePlane() );
    if ( plane )
    {
        plane->setVerticalRange( QPair< qreal, qreal >( 0, 65 ) );
        plane->setHorizontalRange( QPair< qreal, qreal >( 0, dataPoint + 110 ) );
    }
    chart->show();
    QEventLoop endMeasure;
    connect( chart, SIGNAL( finishedDrawing() ), &endMeasure, SLOT( quit() ) );
    QBENCHMARK{
        QMetaObject::invokeMethod( &model, "addData" );
        endMeasure.exec();
    }
    delete chart;
}

void MySmallBenchTest::benchTen()
{
    bench( 9 );
}

void MySmallBenchTest::benchTwentyFive()
{
    bench( 24 );
}

void MySmallBenchTest::benchFifty()
{
    bench( 49 );
}

void MySmallBenchTest::benchNinetyNine()
{
    bench( 99 );
}

void MySmallBenchTest::benchTwoHundredFifty()
{
    bench( 249 );
}

void MySmallBenchTest::benchFiveHundred()
{
    bench( 499 );
}

void MySmallBenchTest::benchThousand()
{
    bench( 1000 );
}

void MySmallBenchTest::benchTwoThousandFiveHundred()
{
    bench( 2500 );
}

void MySmallBenchTest::benchFiveThousand()
{
    bench( 4999 );
}

void MySmallBenchTest::benchTenThousand()
{
    bench( 9999 );
}

void MySmallBenchTest::benchCompressedTen()
{
    bench( 9 , true );
}

void MySmallBenchTest::benchCompressedTwentyFive()
{
    bench( 24 , true );
}

void MySmallBenchTest::benchCompressedFifty()
{
    bench( 49 , true );
}

void MySmallBenchTest::benchCompressedNinetyNine()
{
    bench( 99 , true );
}

void MySmallBenchTest::benchCompressedTwoHundredFifty()
{
    bench( 249 , true );
}

void MySmallBenchTest::benchCompressedFiveHundred()
{
    bench( 499 , true );
}

void MySmallBenchTest::benchCompressedThousand()
{
    bench( 1000 , true );
}

void MySmallBenchTest::benchCompressedTwoThousandFiveHundred()
{
    bench( 2500 , true );
}

void MySmallBenchTest::benchCompressedFiveThousand()
{
    bench( 4999 , true );
}

void MySmallBenchTest::benchCompressedTenThousand()
{
    bench( 9999, true );
}

QTEST_MAIN(MySmallBenchTest);

#include "main.moc"
