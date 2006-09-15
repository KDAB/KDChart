#ifndef __KDCHARTWIDGET_DESIGNER_CUSTOM_EDITOR_H__
#define __KDCHARTWIDGET_DESIGNER_CUSTOM_EDITOR_H__

#include "../src/kdchart_export.h"

#include <QDialog>

#include "ui_kdchartwidgetdesignercustomeditor.h"

namespace KDChart {
    class Widget;
}

class KDCHART_PLUGINS_EXPORT KDChartWidgetDesignerCustomEditor 
    : public QDialog, private Ui::KDChartWidgetProperties
{
    Q_OBJECT
    public:
        KDChartWidgetDesignerCustomEditor( KDChart::Widget *chart );

    public slots:
        void slotTypeChanged( int index );
        void slotSubTypeChanged( int index );
        void slotLeadingTopChanged( int v );
        void slotLeadingLeftChanged( int v );
        void slotLeadingRightChanged( int v );
        void slotLeadingBottomChanged( int v );

    private:
        KDChart::Widget * const mChart;

};

#endif
