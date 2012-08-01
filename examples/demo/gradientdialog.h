#ifndef GRADIENTDIALOG_H
#define GRADIENTDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QGradient>

class GradientDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY( QGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged() )

public:
    explicit GradientDialog( QWidget *parent = 0 );
    ~GradientDialog();
    QGradient gradient() const;
    void setGradient( const QGradient &gradient );
    static QGradient getGradient( const QGradient &gradient, QWidget *parent = 0, const QString &title = QString() );

Q_SIGNALS:
    void gradientChanged();

private:
    class Private;
    Private *d;
};

#endif // GRADIENTDIALOG_H
