#ifndef COLORSLIDER_H
#define COLORSLIDER_H

#include <QtGui/QAbstractSlider>

class ColorSlider : public QAbstractSlider
{
    Q_OBJECT
    Q_PROPERTY( QColor startColor READ startColor WRITE setStartColor NOTIFY startColorChanged )
    Q_PROPERTY( QColor endColor READ endColor WRITE setStartColor NOTIFY endColorChanged )
public:
    explicit ColorSlider( QWidget *parent = 0 );
    ~ColorSlider();
    QColor startColor() const;
    void setStartColor( const QColor &color );
    QColor endColor() const;
    void setEndColor( const QColor &color );
    QSize sizeHint() const;

protected:
    void paintEvent( QPaintEvent *event );
    void mousePressEvent(QMouseEvent *event );
    void mouseReleaseEvent(QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    
Q_SIGNALS:
    void startColorChanged();
    void endColorChanged();
    
public Q_SLOTS:
private:
    class Private;
    Private *d;
    
};

#endif // COLORSLIDER_H
