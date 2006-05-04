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

#include "derivedaddlegenddialog.h"

#include <KDChartPosition>


DerivedAddLegendDialog::DerivedAddLegendDialog( QWidget* parent ) :
    QWidget( parent )
{
    // this bloc left empty intentionally
}


void DerivedAddLegendDialog::setupUi(QDialog* dialog)
{
    AddLegendDialog::setupUi( dialog );
    connect(positionCO, SIGNAL(currentIndexChanged( int )), SLOT(positionChanged( int )));
    connect(useHorzSpaceCB, SIGNAL(stateChanged( int )), SLOT(horizChanged( int )));
    connect(useVertSpaceCB, SIGNAL(stateChanged( int )), SLOT(vertChanged( int )));
}


void DerivedAddLegendDialog::positionChanged( int index )
{
    Q_UNUSED(index);
    const KDChart::Position pos (KDChart::Position::fromPrintableName( positionCO->currentText() ));
    if( pos.isCorner() ){
        if( ! useHorzSpaceCB->isChecked() &&  ! useVertSpaceCB->isChecked() ){
            // at least one of the use space CBs
            // must be checked for corner positions
            useHorzSpaceCB->setChecked( true );
            useVertSpaceCB->setChecked( false );
        }
    }else{
        useHorzSpaceCB->setChecked( ! pos.isPole() );
        useVertSpaceCB->setChecked( pos.isPole() );
    }
    useHorzSpaceCB->setEnabled( pos.isCorner() );
    useVertSpaceCB->setEnabled( pos.isCorner() );
}

void DerivedAddLegendDialog::horizChanged( int state )
{
    horizVertChanged( true, state );
}

void DerivedAddLegendDialog::vertChanged( int state )
{
    horizVertChanged( false, state );
}

void DerivedAddLegendDialog::horizVertChanged( bool horiz, int state )
{
    // Of course we only need to handle cases, when the CBs are enabled,
    // so we can safely ignore the non-Corner positions here.

    // For all Corners, there is the same rule:
    // Legend must either use horizontal space, or use vertical space,
    // or use both spaces.

    // So we make sure, not both CBs are unchecked at the same time:
    if( state == Qt::Unchecked ){
        if( horiz )
            useVertSpaceCB->setChecked( true );
        else
            useHorzSpaceCB->setChecked( true );
    }
}
