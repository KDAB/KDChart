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
    class AbstractDiagram; 
}

class KDChartChartDesignerCustomEditor
    : public QDialog, private Ui::KDChartChartProperties
{
    Q_OBJECT
    public:
    KDChartChartDesignerCustomEditor( KDChart::Chart *chart );

    
      enum ChartType {
        Bar =  0,
        Line = 1,
        Pie = 2
      };
      enum SubType {
	Normal = 0,
	Stacked = 1,
	Percent = 2
      };


      void setSubType( const SubType subtype );

    public slots:
        void slotTypeChanged( int index );
        void slotLeadingTopChanged( int v );
        void slotLeadingLeftChanged( int v );
        void slotLeadingRightChanged( int v );
        void slotLeadingBottomChanged( int v );


    private slots:
        void slotAddLegend();
        void slotRemoveLegend();
        void slotCurrentLegendChanged( int idx );
        void slotAddAxis();
        void slotRemoveAxis();
        void slotCurrentAxisChanged( int idx ); 
        void slotAddHeaderFooter();
        void slotRemoveHeaderFooter();
        void slotCurrentHeaderFooterChanged( int idx );

    private:
        void setupCombos();
        void setupLegendsTab();
        void setupAxesTab();
        void setupHeaderFooterTab();
        ChartType typeFromDiagram() ;
        SubType subTypeFromDiagram();
        
	//KDChart::DiagramPropertiesWidget * mDiagramEditor;
        KDChart::LegendPropertiesWidget * mLegendEditor;
        KDChart::AxisPropertiesWidget * mAxisEditor;
        KDChart::HeaderFooterPropertiesWidget * mHeaderFooterEditor;
        KDChart::Chart * const mChart;

};

#endif
