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

#include "framewidget.h"

#include <KDChartChart>

#include <QDebug>
#include <QPainter>

FrameWidget::FrameWidget( QWidget* parent, Qt::WFlags f )
    : QWidget( parent, f )
    , mChart( 0 )
{
    // this bloc left empty intentionally
}

void FrameWidget::paintEvent( QPaintEvent* e )
{
    if( !mChart ){
        QWidget::paintEvent( e );
    }else{
        QPainter painter( this );
        const QPen   oldPen(   painter.pen() );
        const QBrush oldBrush( painter.brush() );
        painter.setPen( QPen(Qt::darkBlue, 1) );
        painter.setBrush( QBrush(Qt::yellow, Qt::DiagCrossPattern ));
        QRect r( rect() );
        const int wid=64;
        painter.drawEllipse(r.left(),                r.top(),                 wid,wid);
        painter.drawEllipse(r.left()+r.width()-wid-1,r.top(),                 wid,wid);
        painter.drawEllipse(r.left(),                r.top()+r.height()-wid-1,wid,wid);
        painter.drawEllipse(r.left()+r.width()-wid-1,r.top()+r.height()-wid-1,wid,wid);
        painter.setBrush( oldBrush );
        painter.setPen(   oldPen );

        mChart->paint(
            &painter,
            QRect( r.left()+wid/2, r.top()+wid/2,
                   r.width()-wid, r.height()-wid ) );
    }
}
