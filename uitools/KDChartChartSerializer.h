#ifndef __KDCHART_CHART_SERIALIZER_H__
#define __KDCHART_CHART_SERIALIZER_H__

#include "kdchart_export.h"

class DomWidget;

namespace KDChart
{
    class Chart;
    class UITOOLS_EXPORT ChartSerializer
    {
    public:
        ChartSerializer();
        ~ChartSerializer();

        static bool saveExtraInfo( const Chart* widget, DomWidget *ui_widget );
        static bool loadExtraInfo( Chart* widget, DomWidget *ui_widget );
    };

}

#endif
