#include <KDXMLTools>

#include <QtTest/QtTest>
#include <QVariant>
#include <QRect>

class XmlToolsTest : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase()
    {
    }

    void testBool()
    {
        runTests( QVariantList() << QVariant( true ) << QVariant( false ) );
    }

    void testQString()
    {
        runTests( QVariantList()
                  << QVariant( QString() ) // an empty one
                  << QVariant( QString( "some text" ) ) );
    }

    void testQRect()
    {
        runTests( QVariantList()
                  << QRect()
                  << QRect( 0, 0, 0, 0 )
                  << QRect( 0, 0, -10, 30 )
                  << QRect( 10, 20, 30, 40 ) );
    }

    void cleanupTestCase()
    {
    }

private:
    void runTests( const QVariantList& variants )
    {
        static const QString name( "varianttests" );
        QDomDocument doc( "test" );
        QDomElement root = doc.createElement( "properties" );
        doc.appendChild( root );

        Q_FOREACH( QVariant value, variants ) {
            // serialize value, deserialize it, and compare the results
            KDXML::createQVariantNode( doc, root, name, value );
        }

        QDomNodeList elements = root.childNodes();
        QCOMPARE( variants.size(), elements.size() );

        int at = 0;
        Q_FOREACH( QVariant value, variants ) {
            QDomElement element = elements.at( at ).toElement();
            ++at;
            QVariant result;
            QString nameResult;
            bool ok = KDXML::readQVariantNode( element, result, nameResult );
            QVERIFY2( ok, "deserializing failed" );
            QCOMPARE( nameResult, name );
            QCOMPARE( value, result );
        }
    }

    QVariantList mTestcases;
    QString mDocstart;
};

QTEST_MAIN(XmlToolsTest)

#include "main.moc"
