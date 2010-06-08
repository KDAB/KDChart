#ifndef __KDCHART_AXIS_PROPERTIES_WIDGET_H__
#define __KDCHART_AXIS_PROPERTIES_WIDGET_H__

#include <QWidget>

#include "kdchart_export.h"
#include "ui_KDChartAxisPropertiesWidget.h"
#include "KDChartGlobal.h"

namespace KDChart {

    class CartesianAxis;


    /**
     * \class AxisPropertiesWidget KDChartAxisPropertiesWidget.h
     * \brief
     *
     *
     */
    class UITOOLS_EXPORT AxisPropertiesWidget
	    : public QWidget, private Ui::KDChartAxisPropertiesWidget
    {
        Q_OBJECT

        Q_DISABLE_COPY( AxisPropertiesWidget )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( AxisPropertiesWidget )

    public:
        /**
         * Default Constructor
         *
         * Creates a new widget with all data initialized empty.
         *
         * \param parent the widget parent; passed on to QWidget
         */
        AxisPropertiesWidget( QWidget* parent = 0 );

        /** Destructor. */
        ~AxisPropertiesWidget();

        void setAxis( CartesianAxis* axis );
        void setInstantApply( bool value );
	void readFromAxis( const CartesianAxis * axis );
        void writeToAxis( CartesianAxis * axis );

    protected Q_SLOTS:

        void slotTitleChanged( const QString& text );
	void slotPositionChanged( int idx );

    Q_SIGNALS:
        // emitted whenever a value in the dialog changes
        void changed();
    };
}

#endif // KDChart_AXIS_PROPERTIES_Widget_H
