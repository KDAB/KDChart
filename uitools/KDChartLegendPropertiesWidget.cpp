
/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <KDChartLegendPropertiesWidget.h>
#include <KDChartLegendPropertiesWidget_p.h>

#include <QWidget>
#include <QDebug>
#include <QLabel>

#define d d_func()

using namespace KDChart;

LegendPropertiesWidget::Private::Private()
    :legend(0), instantApply(true)
{

}

LegendPropertiesWidget::Private::~Private()
{
}

LegendPropertiesWidget::LegendPropertiesWidget( QWidget *parent )
    :QWidget( parent )
{
    new QLabel( "Legend Editor Widget", this );
    setupUi( this );
}

LegendPropertiesWidget::~LegendPropertiesWidget()
{

}

void LegendPropertiesWidget::setLegend( Legend * legend )
{
    d->legend = legend;
    readFromLegend( legend );
}

void LegendPropertiesWidget::setInstantApply( bool value )
{
    d->instantApply = value;
}

void LegendPropertiesWidget::readFromLegend( const Legend * legend )
{
    Q_UNUSED( legend )
}

void LegendPropertiesWidget::writeToLegend( Legend * legend )
{
    Q_UNUSED( legend )
}
