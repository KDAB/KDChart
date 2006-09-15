#include "kdchartdesignerextension.h"

#include "kdchartwidgetdesignercustomeditor.h"

#include <KDChartWidget>

#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>

#include <QAction>

KDChartWidgetTaskMenu::KDChartWidgetTaskMenu( KDChart::Widget * chartWidget, QObject * parentW ):
    QObject( parentW ),
    mChart( chartWidget ),
    mEditPropertiesAction( 0 )
{
  mEditPropertiesAction = new QAction( tr( "Edit chart properties..." ), this );
  connect( mEditPropertiesAction, SIGNAL( triggered() ), this, SLOT( editCellProperties() ) );
}

KDChartWidgetTaskMenu::~KDChartWidgetTaskMenu()
{
}

QAction * KDChartWidgetTaskMenu::preferredEditAction() const
{
  return mEditPropertiesAction;
}

QList<QAction*> KDChartWidgetTaskMenu::taskActions() const
{
  QList<QAction*> actions;
  actions.append( mEditPropertiesAction );

  return actions;
}

void KDChartWidgetTaskMenu::editCellProperties()
{
  KDChartWidgetDesignerCustomEditor dlg( mChart );
  dlg.exec();
}


KDChartWidgetTaskMenuFactory::KDChartWidgetTaskMenuFactory( QExtensionManager * extMgr ):
    QExtensionFactory( extMgr )
{
}

QObject * KDChartWidgetTaskMenuFactory::createExtension( QObject * object, const QString &iid, QObject * parentW) const
{
    if ( KDChart::Widget * kdchart = qobject_cast<KDChart::Widget*>( object ) ) {
        if ( iid == Q_TYPEID( QDesignerTaskMenuExtension ) ) {
            return new KDChartWidgetTaskMenu( kdchart, parentW );
        }
    }

    return 0;
}
