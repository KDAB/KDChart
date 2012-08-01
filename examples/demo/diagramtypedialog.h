#ifndef DIAGRAMTYPEDIALOG_H
#define DIAGRAMTYPEDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QHash>

class QAbstractItemModel;
namespace KDChart
{
    class Chart;
}

class DiagramTypeDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY( DiagramType type READ type WRITE setType NOTIFY diagramTypeChanged )
    Q_PROPERTY( Subtype subType READ subType WRITE setSubType NOTIFY diagramTypeChanged )
public:
    enum DiagramType{ Bar, LyingBar, Line, Plotter, Pie };
    enum Subtype{ Normal, Percent, Stacked };
    explicit DiagramTypeDialog( KDChart::Chart *chart, QWidget *parent = 0 );
    ~DiagramTypeDialog();

    void setDefaultModels( QHash< DiagramType, QAbstractItemModel* > models );

    DiagramType type() const;
    Subtype subType() const;
    void setType( DiagramType type );
    void setSubType( Subtype type );

Q_SIGNALS:
    void diagramTypeChanged( DiagramType, Subtype );

private:
    class Private;
    Private *d;
};

#endif // DIAGRAMTYPEDIALOG_H
