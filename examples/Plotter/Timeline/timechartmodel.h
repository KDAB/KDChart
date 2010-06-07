#ifndef TIMECHARTMODEL_H
#define TIMECHARTMODEL_H

#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QPair>

class TimeChartModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TimeChartModel( QObject* parent = 0 );

    QPair< QDateTime, QDateTime > visibleRange() const;
    
public Q_SLOTS:
    void setVisibleRange( const QDateTime& start, const QDateTime& end );
    void setVisibleStart( const QDateTime& start );
    void setVisibleEnd( const QDateTime& end );

    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

protected:
    bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const;

private:
    QPair< QDateTime, QDateTime > range;
};

#endif
