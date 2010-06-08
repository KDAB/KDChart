#ifndef __KDCHART_LEGEND_PROPERTIES_WIDGET_H__
#define __KDCHART_LEGEND_PROPERTIES_WIDGET_H__

#include <QWidget>

#include "kdchart_export.h"
#include "ui_KDChartLegendPropertiesWidget.h"
#include "KDChartGlobal.h"

namespace KDChart {

    class Legend;

    /**
     * \class LegendPropertiesWidget KDChartLegendPropertiesWidget.h
     * \brief
     *
     *
     */
    class UITOOLS_EXPORT LegendPropertiesWidget
    : public QWidget, private Ui::KDChartLegendPropertiesWidget
    {
        Q_OBJECT

        Q_DISABLE_COPY( LegendPropertiesWidget )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( LegendPropertiesWidget )

    public:
        LegendPropertiesWidget( QWidget* parent = 0 );

        ~LegendPropertiesWidget();

        void setLegend( Legend* legend );
        void setInstantApply( bool value );
        void readFromLegend( const Legend * legend );
        void writeToLegend( Legend * legend );

    protected Q_SLOTS:
        void slotPositionChanged( int idx );
        void slotOrientationChanged( bool toggled );
        void slotAlignmentChanged( bool toggled );
        void slotTitleTextChanged( const QString& text );
        void slotShowLineChanged( int state );

    Q_SIGNALS:
        // emitted whenever a value in the dialog changes
        void changed();
    };
}

#endif // KDChart_LEGEND_PROPERTIES_Widget_H
