#ifndef KDCHARTDESIGNEREXTENSION_H
#define KDCHARTDESIGNEREXTENSION_H

#include <../src/kdchart_export.h>

#include <QObject>
#include <QList>

// QtDesigner module includes
#include <taskmenu.h>
#include <default_extensionfactory.h>

namespace KDChart {
  class Widget;
}

class QAction;

// This is for extending the popupmenu on charts, to add "Edit cell properties..."
class KDCHART_PLUGINS_EXPORT KDChartWidgetTaskMenu: public QObject, public QDesignerTaskMenuExtension
{
  Q_OBJECT
  Q_INTERFACES(QDesignerTaskMenuExtension)
  Q_DISABLE_COPY( KDChartWidgetTaskMenu )
public:
  KDChartWidgetTaskMenu( KDChart::Widget * chartWidget, QObject * parentW );
  ~KDChartWidgetTaskMenu();

  /* \reimp */
  QAction *preferredEditAction() const;
  /* \reimp */
  QList<QAction*> taskActions() const;

private slots:
  void editCellProperties();

private:
  KDChart::Widget * mChart;
  QAction * mEditPropertiesAction;

};

class KDCHART_PLUGINS_EXPORT KDChartWidgetTaskMenuFactory: public QExtensionFactory
{
    Q_OBJECT
    Q_DISABLE_COPY( KDChartWidgetTaskMenuFactory )
public:
    explicit KDChartWidgetTaskMenuFactory( QExtensionManager *extMgr = 0 );

protected:
    virtual QObject * createExtension( QObject *object, const QString &iid, QObject *parentW ) const;
};


#endif /* KDCHARTDESIGNEREXTENSION_H */
