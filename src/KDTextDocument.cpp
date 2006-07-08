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

#include "KDTextDocument.h"
#include <QRect>
#include <QAbstractTextDocumentLayout>
#include <QtDebug>

//FIXME(khz): use an internal libfakes library instead of this internal haeder file
#include "kdchart_platformdef.h"

// This is an internal class that mimicks some of the behavior of a
// QLabel with rich text assigned, this is mostly a workaround around
// QTextDocumentLayout not being a public class.

QSize KDTextDocument::sizeHint()
{
    if( !mHintValid )
        (void)minimumSizeHint();
    return mSizeHint;
}

QSize KDTextDocument::minimumSizeHint()
{
    if( mHintValid )
        return mMinimumSizeHint;

    mHintValid = true;
    mSizeHint = sizeForWidth( -1 );
    QSize sz(-1, -1);

    // PENDING(kalle) Cache
    sz.rwidth() = sizeForWidth( 0 ).width();
    sz.rheight() = sizeForWidth( 32000 ).height();
    if( mSizeHint.height() < sz.height())
        sz.rheight() = mSizeHint.height();

    mMinimumSizeHint = sz;
    return sz;
}


QSize KDTextDocument::sizeForWidth(int w)
{
    Q_UNUSED( w );

    setPageSize(QSize(0, 100000));

    return documentLayout()->documentSize().toSize();
}
