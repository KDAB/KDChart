/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef __KDCHARTCHART_DESIGNER_CUSTOM_EDITOR_H__
#define __KDCHARTCHART_DESIGNER_CUSTOM_EDITOR_H__

#include "kdchart_export.h"

#include <QDialog>

#include "ui_kdchartchartdesignercustomeditor.h"

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

namespace KDChart {
    class Chart;
    class LegendPropertiesWidget;
    class AxisPropertiesWidget;
    class HeaderFooterPropertiesWidget;
    class DiagramPropertiesWidget;
    class AbstractDiagram;
    class AbstractCartesianDiagram;
}

class KDCHART_PLUGIN_EXPORT KDChartChartDesignerCustomEditor
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
        void switchToCartesianPlaneAndMoveStuffToDiagram(
                KDChart::AbstractCartesianDiagram *diagram );
        void moveLegendsToDiagram( KDChart::AbstractDiagram* diagram );

	//KDChart::DiagramPropertiesWidget * mDiagramEditor;
        KDChart::LegendPropertiesWidget * mLegendEditor;
        KDChart::AxisPropertiesWidget * mAxisEditor;
        KDChart::HeaderFooterPropertiesWidget * mHeaderFooterEditor;
        KDChart::Chart * const mChart;
        QStandardItemModel * mModel;
};

#endif
