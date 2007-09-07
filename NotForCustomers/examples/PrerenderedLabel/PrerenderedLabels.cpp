#include <QWidget>
#include <QPainter>
#include <QApplication>

#include "PrerenderedElements/KDChartTextLabelCache.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget() : QWidget() {
        m_label1.setText( "Test 1" );
        m_label1.setAngle( -90.0 );
        m_label2.setText( "Test 2" );
        m_label2.setAngle( 45.0 );
    }

    void resizeEvent( QResizeEvent* ) {
        m_label1.setPosition( QPointF( 0.0, 0.0 ) );
        m_label2.setPosition( QPointF( width() / 2, 0.0 ) );
    }

    void paintEvent( QPaintEvent* ) {
        QPainter painter( this );
        painter.drawPixmap( m_label1.position(), m_label1.pixmap() );
        painter.drawPixmap( m_label2.position(), m_label2.pixmap() );
    }

private:
    PrerenderedLabel m_label1;
    PrerenderedLabel m_label2;
};

int main( int argc, char** argv )
{
    QApplication app( argc, argv );
    Widget widget;
    widget.resize( 400, 400 );
    widget.show();
    return app.exec();
}

#include <PrerenderedLabels.moc>
