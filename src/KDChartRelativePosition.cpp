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

#include "KDChartRelativePosition.h"
#include <QtXml/QDomDocumentFragment>
#include <KDChartAbstractArea.h>
#include <KDChartMeasure.h>
#include <KDChartTextAttributes.h>
#include <KDChartFrameAttributes.h>
#include <KDChartBackgroundAttributes.h>


namespace KDChart {


#define d d_func()

class RelativePosition::Private
{
    friend class RelativePosition;
public:
    Private();
private:
    AbstractArea* area;
    Position position;
    Qt::Alignment alignment;
    Measure horizontalPadding;
    Measure verticalPadding;
    int rotation;
};

RelativePosition::Private::Private() :
    area( 0 ),
    position(),
    alignment( Qt::AlignCenter ),
    horizontalPadding(),
    verticalPadding(),
    rotation( 0 )
{
    //this line left empty intentionally
}


RelativePosition::RelativePosition()
    : _d( new Private() )
{
    //this line left empty intentionally
}

RelativePosition::RelativePosition( const RelativePosition& r )
    : _d( new Private( *r.d ) )
{
    //this line left empty intentionally
}

RelativePosition & RelativePosition::operator=( const RelativePosition& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

RelativePosition::~RelativePosition()
{
    delete _d; _d = 0;
}


bool RelativePosition::operator==( const RelativePosition& r )
{
    if( referenceArea()     == r.referenceArea() &&
        referencePosition() == r.referencePosition() &&
        alignment()         == r.alignment() &&
        horizontalPadding() == r.horizontalPadding() &&
        verticalPadding()   == r.verticalPadding() &&
        rotation()          == r.rotation() )
        return true;
    else
        return false;
}




QDomDocumentFragment RelativePosition::toXML() const
{
    // PENDING(kalle) Implement this
    return QDomDocumentFragment();
}


void RelativePosition::setReferenceArea( AbstractArea* area )
{
    d->area = area;
}

AbstractArea* RelativePosition::referenceArea() const
{
    return d->area;
}

void RelativePosition::setReferencePosition( const Position& position )
{
    d->position = position;
}

Position RelativePosition::referencePosition() const
{
    return d->position;
}

void RelativePosition::setAlignment( Qt::Alignment flags )
{
    d->alignment = flags;
}

Qt::Alignment RelativePosition::alignment() const
{
    return d->alignment;
}

void RelativePosition::setHorizontalPadding( const Measure& padding )
{
    d->horizontalPadding = padding;
}

Measure RelativePosition::horizontalPadding() const
{
    return d->horizontalPadding;
}

void RelativePosition::setVerticalPadding( const Measure& padding )
{
    d->verticalPadding = padding;
}

Measure RelativePosition::verticalPadding() const
{
    return d->verticalPadding;
}

void RelativePosition::setRotation( int rotation )
{
    d->rotation = rotation;
}

int RelativePosition::rotation() const
{
    return d->rotation;
}

}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::RelativePosition& rp)
{
    dbg << "KDChart::RelativePosition("
	<< "referencearea="<<rp.referenceArea()
	<< "referenceposition="<<rp.referencePosition()
	<< "alignment="<<rp.alignment()
	<< "horizontalpadding="<<rp.horizontalPadding()
	<< "verticalpadding="<<rp.verticalPadding()
	<< "rotation="<<rp.rotation()
	<< ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
