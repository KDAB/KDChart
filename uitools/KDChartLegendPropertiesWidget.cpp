
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

#include <QWidget>
#include <QDebug>
#include <QLabel>

#include <KDChartPosition.h>
#include <KDChartLegend.h>

#include <KDChartLegendPropertiesWidget.h>
#include <KDChartLegendPropertiesWidget_p.h>


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
    : QWidget( parent ), _d( new Private )
{
    setupUi( this );

    mPositionCombo->insertItems( 0, Position::printableNames( true ) );
    connect( mPositionCombo, SIGNAL( activated( int ) ),
             this, SLOT( slotPositionChanged( int ) ) );
    setEnabled( false );
}

LegendPropertiesWidget::~LegendPropertiesWidget()
{

}

void LegendPropertiesWidget::setLegend( Legend * legend )
{
    d->legend = legend;
    if ( legend ) {
        readFromLegend( legend );
        setEnabled( true );
    } else {
        setEnabled( false);
    }
}

void LegendPropertiesWidget::setInstantApply( bool value )
{
    d->instantApply = value;
}

void LegendPropertiesWidget::readFromLegend( const Legend * legend )
{
    mPositionCombo->setCurrentIndex( legend->position().value() );
}

void LegendPropertiesWidget::writeToLegend( Legend * legend )
{
    if ( !legend ) return;
    legend->setPosition( Position( mPositionCombo->currentIndex() ) );
}

void LegendPropertiesWidget::slotPositionChanged( int idx )
{
    if ( d->legend && d->instantApply ) {
        d->legend->setPosition( Position( idx ) );
    } else {
        emit changed();
    }
}
