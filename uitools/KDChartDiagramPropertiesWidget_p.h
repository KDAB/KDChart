#ifndef __KDCHART_DIAGRAM_PROPERTIES_WIDGET_P_H__
#define __KDCHART_DIAGRAM_PROPERTIES_WIDGET_P_H__

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

#include <KDChartDiagramPropertiesWidget.h>

namespace KDChart {
    class LineDiagram;
    class BarDiagram;
    class PieDiagram;

    /**
    * \internal
    */
    class DiagramPropertiesWidget::Private
    {
        friend class DiagramPropertiesWidget;
    public:
        explicit Private();
        virtual ~Private();

    protected:
        void init();
        LineDiagram* linediagram;
        BarDiagram* bardiagram;
        PieDiagram* piediagram;
        bool instantApply;
    };
}


#endif // KDChart_DIAGRAM_PROPERTIES_Widget_p_H
