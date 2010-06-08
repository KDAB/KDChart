#ifndef __KDCHART_HEADERFOOTER_PROPERTIES_WIDGET_H__
#define __KDCHART_HEADERFOOTER_PROPERTIES_WIDGET_H__

#include <QWidget>

#include "kdchart_export.h"
#include "ui_KDChartHeaderFooterPropertiesWidget.h"
#include "KDChartGlobal.h"

namespace KDChart {

    class HeaderFooter;

    /**
     * \class HeaderFooterPropertiesWidget KDChartHeaderFooterPropertiesWidget.h
     * \brief
     *
     *
     */

    class UITOOLS_EXPORT HeaderFooterPropertiesWidget
       : public QWidget, private Ui::KDChartHeaderFooterPropertiesWidget
    {
        Q_OBJECT

        Q_DISABLE_COPY( HeaderFooterPropertiesWidget )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( HeaderFooterPropertiesWidget )

    public:
        /**
         * Default Constructor
         *
         * Creates a new widget with all data initialized empty.
         *
         * \param parent the widget parent; passed on to QWidget
         */
        HeaderFooterPropertiesWidget( QWidget* parent = 0 );

        /** Destructor. */
        ~HeaderFooterPropertiesWidget();

        void setHeaderFooter( HeaderFooter* hf );
        void setInstantApply( bool value );
	void readFromHeaderFooter( const HeaderFooter * headerFooter );
        void writeToHeaderFooter( HeaderFooter * headerFooter );

    protected Q_SLOTS:

        void slotTextChanged( const QString& text );
	void slotPositionChanged( int idx );
        void slotTypeChanged( bool toggled );

    Q_SIGNALS:
        // emitted whenever a value in the dialog changes
        void changed();
    };
}

#endif // KDChart_HEADERFOOTER_PROPERTIES_Widget_H
