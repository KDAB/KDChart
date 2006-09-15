/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
 
#include <KDChartWidget>

#include "kdchartwidgetdesignercustomeditor.h"


KDChartWidgetDesignerCustomEditor::KDChartWidgetDesignerCustomEditor( KDChart::Widget * chart )
    :mChart( chart )
{
    setupUi( this );
    mTypeCombo->setCurrentIndex( mChart->type() - 1 );
    slotTypeChanged( mChart->type() - 1 );
    mGlobalLeadingTopSB->setValue( mChart->globalLeadingTop() );
    mGlobalLeadingLeftSB->setValue( mChart->globalLeadingLeft() );
    mGlobalLeadingRightSB->setValue( mChart->globalLeadingRight() );
    mGlobalLeadingBottomSB->setValue( mChart->globalLeadingBottom() );
    connect( mCloseButton, SIGNAL( clicked() ),
             this, SLOT( accept() ));
    connect( mTypeCombo, SIGNAL( activated( int ) ),
             this, SLOT( slotTypeChanged( int ) ) );
    connect( mSubTypeCombo, SIGNAL( activated( int ) ),
             this, SLOT( slotSubTypeChanged( int ) ) );
    connect( mGlobalLeadingTopSB, SIGNAL( valueChanged( int ) ),
             this, SLOT( slotLeadingTopChanged( int ) ) );
    connect( mGlobalLeadingLeftSB, SIGNAL( valueChanged( int ) ),
             this, SLOT( slotLeadingLeftChanged( int ) ) );
    connect( mGlobalLeadingRightSB, SIGNAL( valueChanged( int ) ),
             this, SLOT( slotLeadingRightChanged( int ) ) );
    connect( mGlobalLeadingBottomSB, SIGNAL( valueChanged( int ) ),
             this, SLOT( slotLeadingBottomChanged( int ) ) );
}

static QStringList barSubtypeItems()
{
    QStringList items;
    items << "Normal" << "Stacked" << "Percent";// << "Rows";
    return items; 
}

static QStringList lineSubtypeItems()
{
    QStringList items;
    items << "Normal" << "Stacked" << "Percent";
    return items;
}

void KDChartWidgetDesignerCustomEditor::slotTypeChanged( int index )
{
    KDChart::Widget::ChartType type = static_cast<KDChart::Widget::ChartType>(index+1);
    mChart->setType( type );
    mSubTypeCombo->clear();
    switch ( type ) {
        case KDChart::Widget::Bar:
            mSubTypeCombo->addItems( barSubtypeItems() );
            break;
        case KDChart::Widget::Line:
            mSubTypeCombo->addItems( lineSubtypeItems() );
            break;
        case KDChart::Widget::Pie:
            break;
        case KDChart::Widget::Ring:
            break;
        case KDChart::Widget::Polar:
            break;
      case KDChart::Widget::NoType:
        default:
            break;
    }
    mSubTypeCombo->setCurrentIndex( mChart->subType() );
}

void KDChartWidgetDesignerCustomEditor::slotSubTypeChanged( int index )
{
    KDChart::Widget::SubType type = static_cast<KDChart::Widget::SubType>(index);
    mChart->setSubType( type );
}

void KDChartWidgetDesignerCustomEditor::slotLeadingTopChanged( int v )
{
    mChart->setGlobalLeadingTop( v );
}

void KDChartWidgetDesignerCustomEditor::slotLeadingLeftChanged( int v )
{
    mChart->setGlobalLeadingLeft( v );
}

void KDChartWidgetDesignerCustomEditor::slotLeadingRightChanged( int v )
{
    mChart->setGlobalLeadingRight( v );
}

void KDChartWidgetDesignerCustomEditor::slotLeadingBottomChanged( int v )
{
    mChart->setGlobalLeadingBottom( v );
}
