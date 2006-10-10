#include <qapplication.h>
#include <QWidget>
#include <QDebug>
#include <QBoxLayout>
#include <QtTest/QtTest>

#define KDAB_REIMP

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget( QWidget* parent ) : QWidget( parent ) {

    }

protected:
    KDAB_REIMP void resizeEvent( QResizeEvent* ) {
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
        QBoxLayout* vLayout = new QVBoxLayout(); // leak
        MyWidget* widget1 = new MyWidget( 0 ); // leak
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
    }
};

QTEST_MAIN(TestQLayout)

#include "main.moc"
