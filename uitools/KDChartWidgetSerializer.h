#ifndef __KDCHART_WIDGET_SERIALIZER_H__
#define __KDCHART_WIDGET_SERIALIZER_H__

#include "kdchart_export.h"

class DomWidget;

namespace KDChart
{
    class Widget;
    class UITOOLS_EXPORT WidgetSerializer
    {
    public:
        WidgetSerializer();
        ~WidgetSerializer();

        static bool saveExtraInfo( const Widget* widget, DomWidget *ui_widget );
        static bool loadExtraInfo( Widget* widget, DomWidget *ui_widget );
    };

}

#endif
