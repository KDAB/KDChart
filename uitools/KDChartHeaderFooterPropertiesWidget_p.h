#ifndef __KDCHART_HEADERFOOTER_PROPERTIES_WIDGET_P_H__
#define __KDCHART_HEADERFOOTER_PROPERTIES_WIDGET_P_H__

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

#include <KDChartHeaderFooterPropertiesWidget.h>

namespace KDChart {
    class HeaderFooter;

    /**
    * \internal
    */
    class HeaderFooterPropertiesWidget::Private
    {
        friend class HeaderFooterPropertiesWidget;
    public:
        explicit Private();
        virtual ~Private();

    protected:
        void init();
        HeaderFooter* headerfooter;
        bool instantApply;
    };
}


#endif // KDChart_HEADERFOOTER_PROPERTIES_Widget_p_H
