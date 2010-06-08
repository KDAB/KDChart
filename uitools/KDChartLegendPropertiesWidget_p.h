#ifndef __KDCHART_LEGEND_PROPERTIES_WIDGET_P_H__
#define __KDCHART_LEGEND_PROPERTIES_WIDGET_P_H__

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QPointer>

#include <KDChartLegendPropertiesWidget.h>

namespace KDChart {
    class Legend;

    /**
    * \internal
    */
    class LegendPropertiesWidget::Private
    {
        friend class LegendPropertiesWidget;
    public:
        explicit Private();
        virtual ~Private();

    protected:
        void init();
        QPointer<Legend> legend;
        bool instantApply;
    };
}


#endif // KDChart_LEGEND_PROPERTIES_Widget_p_H
