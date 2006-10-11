#include <qapplication.h>
#include <QWidget>
#include <QDebug>
#include <QBoxLayout>
#include <QStyle>
#include <QtTest/QtTest>
#include <QGridLayout>
#include <QLineEdit>

#define KDAB_REIMP

class MyLegendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyLegendWidget( QWidget* parent ) : QWidget( parent ) {
    }

    void makeSizeFixed() {
        setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ); // like Legend
        setFixedSize( 50, 50 ); // hmm Legend has a layout instead.
        //QGridLayout* layout = new QGridLayout( this );
        // ...
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
    void testBoxLayoutHiddenWidget() {
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

    void testBoxLayoutChildWidget() {
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

    void testSubGridLayout() {
        QWidget* topLevelWidget = new QWidget( 0 );
        QBoxLayout* vLayout = new QVBoxLayout( topLevelWidget );
        QGridLayout* gridLayout = new QGridLayout();

        QLineEdit* lineEdit = new QLineEdit( topLevelWidget );
        MyWidgetItem* lineEditWidgetItem = new MyWidgetItem( lineEdit );
        gridLayout->addItem( lineEditWidgetItem, 0, 0 );

        MyLegendWidget* widget1 = new MyLegendWidget( topLevelWidget );
        widget1->makeSizeFixed();

        MyWidgetItem* widgetItem = new MyWidgetItem( widget1 );
        gridLayout->addItem( widgetItem, 1, 1 );
        vLayout->addLayout( gridLayout );

        QRect geom( 100, 100, 800, 800 );
        vLayout->setGeometry( geom );
        qDebug() << "widget1: " << widget1->geometry();
        QVERIFY( widget1->width() > 0 );

        delete topLevelWidget;
    }
};

QTEST_MAIN(TestQLayout)

#include "main.moc"
