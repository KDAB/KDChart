#ifndef DATASETSETTINGS_H
#define DATASETSETTINGS_H

#include <QWidget>

namespace KDChart{
    class Chart;
}

class DatasetSettings : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( int datasetCount READ datasetCount WRITE setDatasetCount NOTIFY datasetCountChanged() )
    
public:
    explicit DatasetSettings( KDChart::Chart *chart, QWidget *parent = 0 );
    ~DatasetSettings();
    int datasetCount() const;
public Q_SLOTS:
    void setDatasetCount( int value );
    void indexChanged( int index );

Q_SIGNALS:
    void datasetCountChanged();    
    
private:
    class Private;
    Private *d;
};

#endif // DATASETSETTINGS_H
