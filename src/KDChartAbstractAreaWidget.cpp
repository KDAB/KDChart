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

#include "KDChartAbstractAreaWidget.h"
#include "KDChartAbstractAreaWidget_p.h"

#include <KDABLibFakes>


using namespace KDChart;


AbstractAreaWidget::Private::Private()
{
    // this bloc left empty intentionally
}

AbstractAreaWidget::Private::~Private()
{
    // this bloc left empty intentionally
}

/*
void AbstractAreaWidget::Private::resizeLayout( const QSize& size )
{
    currentLayoutSize = size;
    //qDebug() << "AbstractAreaWidget::resizeLayout(" << currentLayoutSize << ")";
    layout()->setGeometry( size );
    //qDebug() << "AbstractAreaWidget::resizeLayout done";
}
*/

AbstractAreaWidget::AbstractAreaWidget( QWidget* parent )
    : QWidget( parent )
    , AbstractAreaBase( new Private() )
{
    init();
}

AbstractAreaWidget::~AbstractAreaWidget()
{
    // this bloc left empty intentionally
}

void AbstractAreaWidget::init()
{
    // this bloc left empty intentionally
}

#define d d_func()

void AbstractAreaWidget::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );
    QPainter painter( this );
    paintAll( painter );
}

void AbstractAreaWidget::paintIntoRect( QPainter& painter, const QRect& rect )
{
    qDebug() << "AbstractAreaWidget::paintIntoRect() called";
/*

    if( rect.isEmpty() ) return;

    if( rect.size() != d->currentLayoutSize ){
        d->resizeLayout( rect.size() );
    }

    const QPoint translation = rect.topLeft();
    painter.translate( translation );
    d->paintAll( painter );
    painter.translate( -translation.x(), -translation.y() );



*/
    const QRect oldGeometry( layout()->geometry() );
    const QRect newGeo( QRect( QPoint(0,0), rect.size() ) );
    const bool mustChangeGeo = layout() && oldGeometry != newGeo;
    if( mustChangeGeo )
        layout()->setGeometry( newGeo );
qDebug() << "       called with rect left/top:" << rect.left() << rect.top();
    painter.translate( rect.left(), rect.top() );
    paintAll( painter );
    painter.translate( -rect.left(), -rect.top() );
    if( mustChangeGeo )
        layout()->setGeometry( oldGeometry );

}

void AbstractAreaWidget::forceRebuild()
{
    //bloc left empty intentionally
}

void AbstractAreaWidget::paintAll( QPainter& painter )
{
     qDebug() << "AbstractAreaWidget::paintAll() called";

    // Paint the background and frame
    paintBackground( painter, QRect(QPoint(0, 0), size()) );
    paintFrame(      painter, QRect(QPoint(0, 0), size()) );

    // adjust the widget's content margins,
    // to be sure all content gets calculated
    // to fit into the inner rectangle
    const QRect oldGeometry( areaGeometry()  );
    const QRect inner( innerRect() );
    //qDebug() << "areaGeometry():" << oldGeometry
    //         << "  contentsRect():" << contentsRect() << "  inner:" << inner;
    if( contentsRect() != inner ){
        //qDebug() << "old contentsRect():" << contentsRect() << "  new innerRect:" << inner;
        setContentsMargins(
            inner.left(),
            inner.top(),
            oldGeometry.width() -inner.width()-1,
            oldGeometry.height()-inner.height()-1 );
        //forceRebuild();
    }
    paint( &painter );
     //qDebug() << "AbstractAreaWidget::paintAll() done.";
}

QRect AbstractAreaWidget::areaGeometry() const
{
    return geometry();
}

void AbstractAreaWidget::positionHasChanged()
{
    emit positionChanged( this );
}
/*
void AbstractAreaWidget::setGeometry( const QRect & rect )
{
    qDebug() << "AbstractAreaWidget::setGeometry("<< rect << ") called";
    const bool bChanged = rect != geometry();
    QWidget::setGeometry( rect );
    if( bChanged )
        forceRebuild();
}
*/
