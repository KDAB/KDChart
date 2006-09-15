#ifndef __KDCHART_PLUGINS_PLUGINS_H__
#define __KDCHART_PLUGINS_PLUGINS_H__

#include "../src/kdchart_export.h"

#include <QDesignerCustomWidgetCollectionInterface>

///\brief Qt Designer plug-in widget.
///
///See Qt Designer documentation for details on the following methods.
class KDCHART_PLUGINS_PLUGIN_EXPORT KDChartWidgetPlugin : public QObject, public QDesignerCustomWidgetCollectionInterface {
  Q_OBJECT
  Q_INTERFACES( QDesignerCustomWidgetCollectionInterface )
public:
  explicit KDChartWidgetPlugin( QObject * parent=0 );
  QList<QDesignerCustomWidgetInterface*> customWidgets() const { return mCustomWidgets; }
private:
  QList<QDesignerCustomWidgetInterface*> mCustomWidgets;
};

#endif // __KDCHART_PLUGINS_PLUGINS_H__
