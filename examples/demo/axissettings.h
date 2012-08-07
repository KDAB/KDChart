#ifndef AXISSETTINGS_H
#define AXISSETTINGS_H

#include <QtGui/QWidget>

namespace KDChart
{
    class Chart;
}

class AxisSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AxisSettings( KDChart::Chart *chart, QWidget *parent = 0 );
    ~AxisSettings();

public Q_SLOTS:
    void diagramTypeChanged();

private:
    class Private;
    Private *d;
};

#endif // AXISSETTINGS_H
