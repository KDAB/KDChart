#include <qapplication.h>
#include <QWidget>
#include <QDebug>
#include <QBoxLayout>
#include <QStyle>
#include <QtTest/QtTest>

#define KDAB_REIMP

class MyLegendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyLegendWidget( QWidget* parent ) : QWidget( parent ) {
    }

protected:
    KDAB_REIMP void resizeEvent( QResizeEvent* ) {
        // Note that this is never called unless the widget is shown.
        qDebug() << "resizeEvent " << size();
    }
};

class MyWidgetItem : public QWidgetItem
{
public:
    explicit MyWidgetItem(QWidget *w) : QWidgetItem(w) { }

    KDAB_REIMP bool isEmpty() const { return false; }
};

class TestQLayout : public QObject
{
    Q_OBJECT

private slots:

    // This is very much like KDChart::Chart does with legends
    void testLayoutHiddenWidget() {
        QBoxLayout* vLayout = new QVBoxLayout;
        MyLegendWidget* widget1 = new MyLegendWidget( 0 );
        widget1->resize( 10, 10 );

        // Adding a hidden widget doesn't work, the layout ignores it
        // This is why we created MyWidgetItem above, then the layout works
        //vLayout->addWidget( widget1 );
        MyWidgetItem* widgetItem = new MyWidgetItem( widget1 );
        vLayout->addItem( widgetItem );

        QRect geom( 100, 100, 800, 800 );
        vLayout->setGeometry( geom );

        //vLayout->activate(); // not needed

        QCOMPARE( vLayout->geometry(), geom );
        qDebug() << "widget1: " << widget1->geometry();
        QCOMPARE( widget1->geometry(), geom );

        delete widget1;
        delete vLayout;
    }

    void testRelayoutChildWidget() {
        QWidget* topLevelWidget = new QWidget( 0 );
        // This time the layout is associated with a widget, like d->layout in KDChart::Chart.
        QBoxLayout* vLayout = new QVBoxLayout( topLevelWidget );
        MyLegendWidget* widget1 = new MyLegendWidget( topLevelWidget );
        MyWidgetItem* widgetItem = new MyWidgetItem( widget1 );
        vLayout->addItem( widgetItem );
        //vLayout->activate();

        QRect geom( 100, 100, 800, 800 );
        vLayout->setGeometry( geom );
        qDebug() << "widget1: " << widget1->geometry();
        int marg = topLevelWidget->style()->pixelMetric( QStyle::PM_DefaultTopLevelMargin );
        QCOMPARE( widget1->geometry(), geom.adjusted(marg,marg,-marg,-marg) );

        geom = QRect( 10, 10, 80, 80 );
        vLayout->setGeometry( geom );
        qDebug() << "widget1: " << widget1->geometry();
        QCOMPARE( widget1->geometry(), geom.adjusted(marg,marg,-marg,-marg) );

        // And now let's show the widget for real
        geom = QRect( 0, 0, 50, 50 );
        topLevelWidget->resize( geom.size() );
        topLevelWidget->show();
        QApplication::sendPostedEvents();
        qDebug() << "widget1: " << widget1->geometry();
        QCOMPARE( widget1->geometry(), geom.adjusted(marg,marg,-marg,-marg) );
        QVERIFY( widget1->isVisible() );

        delete topLevelWidget;
    }

    void testSublayout() {
        QWidget* topLevelWidget = new QWidget( 0 );
        QBoxLayout* vLayout = new QVBoxLayout( topLevelWidget );
        QBoxLayout* hLayout = new QHBoxLayout();
        MyLegendWidget* widget1 = new MyLegendWidget( topLevelWidget );
        MyWidgetItem* widgetItem = new MyWidgetItem( widget1 );
        hLayout->addItem( widgetItem );
        vLayout->addLayout( hLayout );

        QRect geom( 100, 100, 800, 800 );
        vLayout->setGeometry( geom );
        qDebug() << "widget1: " << widget1->geometry();
        int marg = topLevelWidget->style()->pixelMetric( QStyle::PM_DefaultTopLevelMargin );
        QCOMPARE( widget1->geometry(), geom.adjusted(marg,marg,-marg,-marg) );

        delete topLevelWidget;
    }
};

QTEST_MAIN(TestQLayout)

#include "main.moc"
