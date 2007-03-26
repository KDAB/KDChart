#include <QtDebug>
#include <QObject>
#include <QtTest>

#include "KDChartSignalCompressor.h"

class SignalCompressorTest : public QObject
{
    Q_OBJECT

public:
    SignalCompressorTest()
        : QObject()
        , m_count( 0 )
        , m_compressor( this, SIGNAL( theSignal() ), this )
    {   // receive our own signal, once it is emitted
        connect( this, SIGNAL( theSignal() ),
                 SLOT( receiveIt() ) );
    }

private:
    int m_count;
    KDChart::SignalCompressor m_compressor;

signals:
    void theSignal();

public slots: // no tests!
    void receiveIt()
    {
        ++m_count;
    }

private slots:
    void cleanup()
    {
        m_count = 0;
    }

    void compressSignalOnceTest()
    {
        QVERIFY( m_count == 0 );
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        QTest::qWait( 100 );
        QVERIFY( m_count == 1 );
    }

    void compressSignalTwiceTest()
    {
        QVERIFY( m_count == 0 );
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        QVERIFY( m_count == 0 );
        QTest::qWait( 100 );
        QVERIFY( m_count == 1 );
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        QVERIFY( m_count == 1 );
        QTest::qWait( 100 );
        QVERIFY( m_count == 2 );
    }
};

QTEST_MAIN( SignalCompressorTest );

#include "main.moc"
