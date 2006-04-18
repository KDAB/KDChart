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
#include <KDChartMeasure.h>
#include <KDChartTextAttributes.h>
#include <KDChartFrameAttributes.h>
#include <KDChartBackgroundAttributes.h>

#define d d_func()

class KDChartRelativePosition::Private
{
    friend class ::KDChartRelativePosition;
public:
    Private();
private:
    Qt::Alignment alignment;
    KDChartMeasure horizontalPadding;
    KDChartMeasure verticalPadding;
    int rotation;
};

KDChartRelativePosition::Private::Private() :
    alignment( Qt::AlignCenter ),
    horizontalPadding(),
    verticalPadding(),
    rotation( 0 )
{
}


KDChartRelativePosition::KDChartRelativePosition()
    : _d( new Private() )
{
}

KDChartRelativePosition::KDChartRelativePosition( const KDChartRelativePosition& r )
    : _d( new Private( *r.d ) )
{
}

KDChartRelativePosition & KDChartRelativePosition::operator=( const KDChartRelativePosition& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

KDChartRelativePosition::~KDChartRelativePosition()
{
    delete _d; _d = 0;
}


bool KDChartRelativePosition::operator==( const KDChartRelativePosition& r )
{
    if( alignment() == r.alignment() &&
        horizontalPadding() == r.horizontalPadding() &&
        verticalPadding() == r.verticalPadding() &&
        rotation() == r.rotation() )
        return true;
    else
        return false;
}




QDomDocumentFragment KDChartRelativePosition::toXML() const
{
    // PENDING(kalle) Implement this
    return QDomDocumentFragment();
}

void KDChartRelativePosition::setAlignment( Qt::Alignment flags )
{
    d->alignment = flags;
}

Qt::Alignment KDChartRelativePosition::alignment() const
{
    return d->alignment;
}

void KDChartRelativePosition::setHorizontalPadding( const KDChartMeasure& padding )
{
    d->horizontalPadding = padding;
}

KDChartMeasure KDChartRelativePosition::horizontalPadding() const
{
    return d->horizontalPadding;
}

void KDChartRelativePosition::setVerticalPadding( const KDChartMeasure& padding )
{
    d->verticalPadding = padding;
}

KDChartMeasure KDChartRelativePosition::verticalPadding() const
{
    return d->verticalPadding;
}

void KDChartRelativePosition::setRotation( int rotation )
{
    d->rotation = rotation;
}

int KDChartRelativePosition::rotation() const
{
    return d->rotation;
}

