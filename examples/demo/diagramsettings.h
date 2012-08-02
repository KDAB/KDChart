#ifndef DIAGRAMSETTINGS_H
#define DIAGRAMSETTINGS_H

#include <QtGui/QWidget>

namespace KDChart{
    class Chart;
}

class DiagramSettings : public QWidget
{
    Q_OBJECT

public:
    explicit DiagramSettings( KDChart::Chart* chart, QWidget *parent = 0 );
    ~DiagramSettings();

public Q_SLOTS:
    void refreshSettings();

private:
    class Private;
    Private *d;
};

#endif // DIAGRAMSETTINGS_H
