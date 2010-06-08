#ifndef __KDCHART_AXIS_PROPERTIES_WIDGET_P_H__
#define __KDCHART_AXIS_PROPERTIES_WIDGET_P_H__

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

#include <KDChartAxisPropertiesWidget.h>

namespace KDChart {
    class CartesianAxis;

    /**
    * \internal
    */
    class AxisPropertiesWidget::Private
    {
        friend class AxisPropertiesWidget;
    public:
        explicit Private();
        virtual ~Private();

    protected:
        void init();
        CartesianAxis* axis;
        bool instantApply;
    };
}


#endif // KDChart_AXIS_PROPERTIES_Widget_p_H
