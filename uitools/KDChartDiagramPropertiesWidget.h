#ifndef __KDCHART_DIAGRAM_PROPERTIES_WIDGET_H__
#define __KDCHART_DIAGRAM_PROPERTIES_WIDGET_H__

#include <QWidget>

#include "kdchart_export.h"
#include "ui_KDChartDiagramPropertiesWidget.h"
#include "KDChartGlobal.h"

namespace KDChart {

    class LineDiagram;
    class BarDiagram;
    class PieDiagram;

    /**
     * \class DiagramPropertiesWidget KDChartDiagramPropertiesWidget.h
     * \brief
     *
     *
     */

    class UITOOLS_EXPORT DiagramPropertiesWidget
       : public QWidget, private Ui::KDChartDiagramPropertiesWidget
    {
        Q_OBJECT

        Q_DISABLE_COPY( DiagramPropertiesWidget )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( DiagramPropertiesWidget )

    public:
        /**
         * Default Constructor
         *
         * Creates a new widget with all data initialized empty.
         *
         * \param parent the widget parent; passed on to QWidget
         */
        DiagramPropertiesWidget( QWidget* parent = 0 );

        /** Destructor. */
        ~DiagramPropertiesWidget();

        void setLineDiagram( LineDiagram* ld );
        void setBarDiagram( BarDiagram* bd );
        void setPieDiagram( PieDiagram* pd );
        void setInstantApply( bool value );
	void readFromLineDiagram( const LineDiagram * ld );
        void readFromBarDiagram( const BarDiagram* bd );
        void readFromPieDiagram( const PieDiagram* pd );
        void writeToLineDiagram( LineDiagram * ld );
        void writeToBarDiagram( BarDiagram * bd );
        void writeToPieDiagram( PieDiagram * pd );

    protected Q_SLOTS:
      
        void slotTypeChanged( int type );

    Q_SIGNALS:
        // emitted whenever a value in the dialog changes
        void changed();
    };
}

#endif // KDChart_HEADERFOOTER_PROPERTIES_Widget_H
