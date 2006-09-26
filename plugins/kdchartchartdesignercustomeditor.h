#ifndef __KDCHARTCHART_DESIGNER_CUSTOM_EDITOR_H__
#define __KDCHARTCHART_DESIGNER_CUSTOM_EDITOR_H__

#include <QDialog>

#include "ui_kdchartchartdesignercustomeditor.h"
#include "diagramtype.h"

namespace KDChart {
    class Chart;
    class LegendPropertiesWidget;
    class AxisPropertiesWidget;
    class HeaderFooterPropertiesWidget;
    class DiagramPropertiesWidget;
}

class KDChartChartDesignerCustomEditor
    : public QDialog, private Ui::KDChartChartProperties
{
    Q_OBJECT
    public:
        KDChartChartDesignerCustomEditor( KDChart::Chart *chart );

    public slots:
        void slotLeadingTopChanged( int v );
        void slotLeadingLeftChanged( int v );
        void slotLeadingRightChanged( int v );
        void slotLeadingBottomChanged( int v );


    private slots:
        void slotAddDiagram();
        void slotRemoveDiagram();
        void slotCurrentDiagramChanged( int idx ); 
        void slotAddLegend();
        void slotRemoveLegend();
        void slotCurrentLegendChanged( int idx );
        //void slotAddAxis();
        //void slotRemoveAxis();
        //void slotCurrentAxisChanged( int idx );
        void slotAddHeaderFooter();
        void slotRemoveHeaderFooter();
        void slotCurrentHeaderFooterChanged( int idx );

    private:
        void setupDiagramsTab();
        void setupLegendsTab();
        //void setupAxesTab();
        void setupHeaderFooterTab();

	KDChart::DiagramPropertiesWidget * mDiagramEditor;
        KDChart::LegendPropertiesWidget * mLegendEditor;
        //KDChart::AxisPropertiesWidget * mAxisEditor;
        KDChart::HeaderFooterPropertiesWidget * mHeaderFooterEditor;
        KDChart::Chart * const mChart;

};

#endif
